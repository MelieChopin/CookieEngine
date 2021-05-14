#include "ECS/EntityHandler.hpp"

using namespace Cookie::ECS;



//////////////////ENTITY/////////////

Entity::Entity(unsigned int _id) : id{ _id } {}
Entity::Entity(unsigned int _id, int _signature, std::string _name, std::string _namePrefab) : id{ _id }, signature{ _signature }, name{ _name }, namePrefab{ _namePrefab } {}
Entity::Entity(const Entity& entity): id{ entity.id }, signature{ entity.signature }, signatureGameplay{ entity.signatureGameplay }, name{ entity.name }, tag{ entity.tag }, needToBeRemoved{ entity.needToBeRemoved } {}


void Entity::operator= (const Entity& entity)
{
	id = entity.id;
	name = entity.name;
	tag = entity.tag;
	signature = entity.signature;
	signatureGameplay = entity.signatureGameplay;
	needToBeRemoved = entity.needToBeRemoved;
}
void Entity::Swap(Entity& entity)
{
	Entity temp{ entity };
	entity = *this;
	*this = temp;
}



///////////////ENTITYHANDLER/////////////

EntityHandler::EntityHandler()
{
	for (unsigned int id = 0; id < MAX_ENTITIES; ++id)
		entities.push_back(Entity(id));
}
