#include "Gameplay/ArmyCoordinator.hpp"
#include "ECS/ComponentGameplay.hpp"
#include "Resources/Map.hpp"
#include "Resources/Prefab.hpp"
#include "ArmyHandler.hpp"


using namespace Cookie::Gameplay;
using namespace Cookie::Core::Math;
using namespace Cookie::Resources;
using namespace Cookie::ECS;


void ArmyCoordinator::Analysis()
{
	//Economic
	if(army->workers.size() + nbOfWorkerInProduction < behavior.stepGoals.nbOfWorker)
		goals.push_back(E_GOALS::E_DEVELOP_WORKER);
	if(behavior.stepGoals.listOfBuildings.size() > 0)
		goals.push_back(E_GOALS::E_DEVELOP_BASE);
	if (army->units.size() + nbOfUnitInProduction < behavior.stepGoals.nbOfUnits)
		goals.push_back(E_GOALS::E_DEVELOP_ARMY);

	//Military
	//if base under attack
	//goals.push_back(E_GOALS::E_DEFEND);
	if (army->workers.size() >= behavior.stepGoals.nbOfWorker &&
		behavior.stepGoals.listOfBuildings.empty() &&
		army->units.size() >= behavior.stepGoals.nbOfUnits)
	{
		goals.push_back(E_GOALS::E_ATTACK);
		behavior.AddNextStep();
	}

}

void ArmyCoordinator::ResourceAllocation(Map& map)
{
	for (int i = 0; i < goals.size(); ++i)
	{
		switch (goals[i])
		{
		case E_GOALS::E_DEVELOP_WORKER:
			DevelopWorker();
			break;
		case E_GOALS::E_DEVELOP_BASE:
			DevelopBase(map);
			break;

		case E_GOALS::E_DEVELOP_ARMY:
			DevelopArmy();
			break;

		case E_GOALS::E_DEFENSE:
			Defense(map);
			break;

		case E_GOALS::E_ATTACK:
			Attack(map);
			break;

		case E_GOALS::E_RETREAT:
			Retreat();
			break;

		default:
			break;
		}
	}

	goals.clear();
}

void ArmyCoordinator::DevelopWorker()
{
	std::cout << "AI DevelopWorker\n";

	for (int i = 0; i < army->buildings.size(); ++i)
	{
		CGPProducer& producer = army->buildings[i]->componentProducer;

		for (int j = 0; j < producer.possibleUnits.size(); ++j)
		{
			if (producer.possibleUnits[j]->gameplay.signatureGameplay & CGP_SIGNATURE::WORKER &&
				producer.AddUnitToQueue(j))
			{
				nbOfWorkerInProduction++;
				return;
			}
		}
	}

}
void ArmyCoordinator::DevelopBase(Map& map)
{
	std::cout << "AI DevelopBase\n";
	//CLEAN later on 

	for (int i = 0; i < army->workers.size(); ++i)
	{
		CGPWorker& worker = army->workers[i]->componentWorker;

		if(!worker.BuildingInConstruction)
			for (int j = 0; j < worker.possibleBuildings.size(); ++j)
			{
				std::string name = worker.possibleBuildings[j]->name;

				for (int k = 0; k < behavior.stepGoals.listOfBuildings.size(); ++k)
					if (name == behavior.stepGoals.listOfBuildings[k])
					{
						//Calculate Pos
						Vec3 pos {INFINITY, INFINITY, INFINITY};
						Vec2 newBuildingTileNb = worker.possibleBuildings[j]->gameplay.componentProducer.tileSize;
						int tempNbOfBuildingInProduction = nbOfBuildingInProduction;
						for (int l = 0; l < army->buildings.size(); ++l)
						{
							//make a function for pos check later on 

							//Check Right pos
							Vec3 posToCheck = army->buildings[l]->trs->pos + Vec3{(NB_TILES_BETWEEN_BUILDINGS + newBuildingTileNb.x) * map.tilesSize.x + ((int)newBuildingTileNb.x % 2) * map.tilesSize.x / 2, 0, 0};
							
							if (!map.GetTile(posToCheck).isObstacle)
							{
								if (tempNbOfBuildingInProduction <= 0)
								{
									pos = posToCheck;
									break;
								}
								tempNbOfBuildingInProduction--;
							}

							//Check Bottom pos
							posToCheck = army->buildings[l]->trs->pos + Vec3{0, 0, (NB_TILES_BETWEEN_BUILDINGS + newBuildingTileNb.y) * map.tilesSize.y + ((int)newBuildingTileNb.y % 2) * map.tilesSize.y / 2 };
							if (!map.GetTile(posToCheck).isObstacle)
							{
								if (tempNbOfBuildingInProduction <= 0)
								{
									pos = posToCheck;
									break;
								}
								tempNbOfBuildingInProduction--;
							}

							//Check Right-Bottom pos
							posToCheck = army->buildings[l]->trs->pos + Vec3{ (NB_TILES_BETWEEN_BUILDINGS + newBuildingTileNb.x) * map.tilesSize.x + ((int)newBuildingTileNb.x % 2) * map.tilesSize.x / 2, 0, (NB_TILES_BETWEEN_BUILDINGS + newBuildingTileNb.y) * map.tilesSize.y + ((int)newBuildingTileNb.y % 2) * map.tilesSize.y / 2 };
							if (!map.GetTile(posToCheck).isObstacle)
							{
								if (tempNbOfBuildingInProduction <= 0)
								{
									pos = posToCheck;
									break;
								}
								tempNbOfBuildingInProduction--;
							}
						}

						if (pos == Vec3{ INFINITY, INFINITY, INFINITY })
							return;

						if (worker.StartBuilding(pos, j))
						{
							behavior.stepGoals.listOfBuildings.erase(behavior.stepGoals.listOfBuildings.begin() + k);
							nbOfBuildingInProduction++;
						}
						return;
					}

			}
	}
}
void ArmyCoordinator::DevelopArmy()
{
	std::cout << "AI DevelopArmy\n";

	for (int i = 0; i < army->buildings.size(); ++i)
	{
		CGPProducer& producer = army->buildings[i]->componentProducer;

		for (int j = 0; j < producer.possibleUnits.size(); ++j)
		{
			if (producer.possibleUnits[j]->gameplay.signatureGameplay & CGP_SIGNATURE::ATTACK &&
				producer.AddUnitToQueue(j))
			{
				nbOfUnitInProduction++;
				return;
			}
		}
	}

}
void ArmyCoordinator::Attack(Map& map)
{
	std::cout << "AI Attack\n";

	for (int i = 0; i < army->units.size(); ++i)
	{
		ComponentGameplay* gameplay = army->units[i];
		ComponentTransform* trs = gameplay->trs;
		
		if (map.ApplyPathfinding(map.GetTile(trs->pos), map.tiles[map.tiles.size() - 1]))
			gameplay->componentMove.SetPath(map.tiles[map.tiles.size() - 1]);
		else
			std::cout << "No Path Find\n";
	}
}
void ArmyCoordinator::Defense(Map& map)
{
	std::cout << "AI Defend\n";

	for (int i = 0; i < army->units.size(); ++i)
	{
		ComponentGameplay* gameplay = army->units[i];
		ComponentTransform* trs = gameplay->trs;

		if (map.ApplyPathfinding(map.GetTile(trs->pos), map.tiles[map.tiles.size() - 1]))
			gameplay->componentMove.SetPath(map.tiles[0]);
		else
			std::cout << "No Path Find\n";
	}
}
void ArmyCoordinator::Retreat()
{
	std::cout << "AI Retreat\n";
}