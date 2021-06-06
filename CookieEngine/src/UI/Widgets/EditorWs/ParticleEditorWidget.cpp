#include "ParticleEditorWidget.hpp"
#include "Particles/ParticlesSystem.hpp"
#include "MapExplorerHelper.hpp"
#include "Serialization.hpp"
#include <imgui.h>
#include <string>

using namespace Cookie::UIwidget;
using namespace Cookie::Resources::Particles;
using namespace Cookie::Resources;
using namespace ImGui;

void ParticleEditor::WindowDisplay()
{
	TryBeginWindow()
	{
		static ParticlesPrefab* selectedParticles = nullptr;

		static std::string name;

		static std::vector<std::string> haveAlreadyAdd;

		//Create new particles
		{
			InputText("Particles name", &name);
			if (Button("Create") && manager.particles.find(name) == manager.particles.end())
			{
				std::unique_ptr<ParticlesPrefab> prefParticles = std::make_unique<ParticlesPrefab>();
				ParticlesPrefab& pref = *prefParticles.get();
				pref.name = name;
				pref.data.push_back(data());
				pref.emitter.push_back(ParticlesEmitter());
				pref.emit.push_back(std::vector<emit>());

				manager.particles[pref.name] = std::move(prefParticles);
				Cookie::Resources::Serialization::Save::SaveParticles(pref);
				selectedParticles = manager.particles[pref.name].get();
				name.clear();
			}
			else if (name.empty())
			{
				NewLine();
				Text("Your texture has no name!");
			}
			else if (manager.particles.find(name) != manager.particles.end())
			{
				NewLine();
				Text("Your texture's name is invalid. (already in use)");
			}
		}

		ResourceMapExplorer<ParticlesPrefab>("Particles System", "##PARTICLESSYSTEMSELECTOR", manager.particles, selectedParticles);

		if (!selectedParticles)
		{
			ImGui::End();
			return;
		}
			
		for (int j = 0; j < selectedParticles->data.size(); j++)
		{
			std::string name = "Particles : " + std::to_string(j + 1);
			if (TreeNode(name.c_str()))
			{
				//Data
				{
					data& dataPrefab = selectedParticles->data[j];

					Text("Mesh :");
					SameLine(100);
					ResourceMapExplorer<Mesh>("Mesh", "##MESHSELECT", manager.meshes, dataPrefab.mesh);

					Text("Texture :");
					SameLine(100);
					ResourceMapExplorer<Texture>("Texture", "##TEXTURESELECT", manager.textures2D, dataPrefab.texture);

					Text("Is billboarded ?");
					SameLine(200);
					if (RadioButton("##BILLBOARD", dataPrefab.isBillboard))
						dataPrefab.isBillboard = !dataPrefab.isBillboard;

					Text("Number Of  Particles :");
					SameLine(200.f);
					DragInt("##NUMBEROFPARTICLES", &dataPrefab.size, 1, 0, 10000);

					Text("Number Of  Particles Per Frame:");
					SameLine(250.f);
					DragInt("##NUMBEROFPARTICLESPERFRAME", &dataPrefab.countFrame, 1, 0, 10000);

					Text("Number Of  Particles Alive:");
					SameLine(250.f);
					DragInt("##NUMBEROFPARTICLESALIVE", &dataPrefab.countAlive, 1, 0, 10000);

					dataPrefab.countFrame = dataPrefab.countFrame < dataPrefab.size ? dataPrefab.countFrame : dataPrefab.size;
					dataPrefab.countAlive = dataPrefab.countAlive < dataPrefab.size ? dataPrefab.countAlive : dataPrefab.size;
				}

				if (TreeNode("Emitter"))
				{
					ParticlesEmitter& emitter = selectedParticles->emitter[j];
					for (int i = 0; i < emitter.generators.size(); i++)
					{
						std::string name = Cookie::Resources::Particles::GetName(emitter.generators[i].get()->type);
						if (TreeNode(name.c_str()))
						{
							switch (emitter.generators[i].get()->type)
							{
							case (POINTPOSITIONGEN):
								break;
							case (BOXPOSITIONGEN):
								break;
							case (CIRCLEPOSITIONGEN):
								break;
							case (TYPEGEN::SCALECONSTGEN): {
								ScaleConstGenerate& scaleC = dynamic_cast<ScaleConstGenerate&>(*emitter.generators[i].get());
								Text("Scale :");
								SameLine(200);
								DragFloat3("##SCALECONST", scaleC.scale.e);
								scaleC.scale.Debug();
								break;
							}
							case (TYPEGEN::SCALERANDGEN): {
								ScaleRandGenerate& scaleR = dynamic_cast<ScaleRandGenerate&>(*emitter.generators[i].get());
								Text("Scale min :");
								SameLine(200);
								DragFloat3("##SCALERANDMIN", scaleR.scaleMin.e);
								Text("Scale max :");
								SameLine(200);
								DragFloat3("##SCALERANDMAX", scaleR.scaleMax.e);
								break;
							}
							case (TYPEGEN::ROTATERANDGEN): {
								RotateRandGenerate& rot = dynamic_cast<RotateRandGenerate&>(*emitter.generators[i].get());
								Text("Rotation min :");
								SameLine(200);
								DragFloat3("##ROTRANDMIN", rot.rotMin.e);
								Text("Rotation max :");
								SameLine(200);
								DragFloat3("##ROTRANDMAX", rot.rotMax.e);
								break;
							}
							case (TYPEGEN::VELCONSTGEN): {
								VelocityConstGenerate& velC = dynamic_cast<VelocityConstGenerate&>(*emitter.generators[i].get());
								Text("Velocity :");
								SameLine(200);
								DragFloat3("##VELCONST", velC.vel.e);
								break;
							}
							case (TYPEGEN::VELRANDGEN): {
								VelocityRandGenerate& velR = dynamic_cast<VelocityRandGenerate&>(*emitter.generators[i].get());
								Text("Velocity min :");
								SameLine(200);
								DragFloat3("##VELMIN", velR.velMin.e);
								Text("Velocity max:");
								SameLine(200);
								DragFloat3("##VELMAX", velR.velMax.e);
								break;
							}
							case (TYPEGEN::MASSCONSTGEN): {
								MassConstGenerate& mass = dynamic_cast<MassConstGenerate&>(*emitter.generators[i].get());
								Text("Mass :");
								SameLine(200);
								DragFloat("##Mass", &mass.mass, 0.01, -50, 50);
								break;
							}
							case (TYPEGEN::TIMECONSTGEN): {
								TimeConstGenerate& timeC = dynamic_cast<TimeConstGenerate&>(*emitter.generators[i].get());
								Text("Time :");
								SameLine(200);
								DragFloat("##TIME", &timeC.time, 0.01, 0, 50);
								break;
							}
							case (TYPEGEN::TIMERANDGEN): {
								TimeRandGenerate& timeR = dynamic_cast<TimeRandGenerate&>(*emitter.generators[i].get());
								Text("Time min :");
								SameLine(200);
								DragFloat("##TIMEMin", &timeR.timeMin, 0.01, 0, 50);
								Text("Time max :");
								SameLine(200);
								DragFloat("##TIMEMax", &timeR.timeMax, 0.01, 0, 50);
								break;
							}
							case (TYPEGEN::COLORCONSTGEN): {
								ColorConstGenerate& colorC = dynamic_cast<ColorConstGenerate&>(*emitter.generators[i].get());
								Text("Color :");
								SameLine(200);
								DragFloat3("##COLORCONST", colorC.col.e);
								break;
							}
							case (TYPEGEN::COLORRANDGEN): {
								ColorRandGenerate& colorR = dynamic_cast<ColorRandGenerate&>(*emitter.generators[i].get());
								Text("Color min :");
								SameLine(200);
								DragFloat3("##COLORCONST", colorR.minCol.e);
								Text("Color max :");
								SameLine(200);
								DragFloat3("##COLORCONST", colorR.maxCol.e);
								break;
							}
							case (TYPEGEN::INITVELWITHPOINT):
								break;
							}
							TreePop();
						}
					}
					for (int i = 0; i < emitter.updates.size(); i++)
					{
						std::string name = Cookie::Resources::Particles::GetName(emitter.updates[i].get()->type);
						if (TreeNode(name.c_str()))
						{
							switch (emitter.updates[i].get()->type)
							{
							case (TYPEUP::UPDATEVEL):
								Text("Velocity is update each frame");
								break;
							case (TYPEUP::UPDATESCALE): {
								UpdateScale& upScale = dynamic_cast<UpdateScale&>(*emitter.updates[i].get());
								Text("Scale end : ");
								SameLine(200);
								DragFloat3("##SCALEEND", upScale.scaleEnd.e);
								break;
							}
							case (TYPEUP::UPDATEALPHA): {
								UpdateAlpha& upAlpha = dynamic_cast<UpdateAlpha&>(*emitter.updates[i].get());
								Text("Alpha end : ");
								SameLine(200);
								DragFloat("##ALPHAEND", &upAlpha.alphaEnd, 0.01, -1, 2);
								break;
							}
							case (TYPEUP::COLOROVERLIFE): {
								ColorOverLife& upColor = dynamic_cast<ColorOverLife&>(*emitter.updates[i].get());
								Text("Color end : ");
								SameLine(200);
								DragFloat3("##COLOREND", upColor.colorEnd.e);
								break;
							}
							case (TYPEUP::ENABLEGRAVITY): {
								EnabledGravity& upGravity = dynamic_cast<EnabledGravity&>(*emitter.updates[i].get());
								Text("Gravity : ");
								SameLine(200);
								DragFloat("##GRAVITY", &upGravity.gravity, 0.01, -50, 50);
								break;
							}
							case (TYPEUP::UPDATETIME):
								Text("Time is update each frame");
								break;
							case (TYPEUP::LOOP):
								break;
							case (TYPEUP::COLLISIONWITHPLANE): {
								CollisionWithPlane& collision = dynamic_cast<CollisionWithPlane&>(*emitter.updates[i].get());
								Text("Which particles system ?");
								SameLine();
								Text("Current : %s", collision.namePrefab.c_str());
								ParticlesPrefab* temp = nullptr;
								ResourceMapExplorer<ParticlesPrefab>("Particles System", "##PARTICLESSYSTEMSELECTOR", manager.particles, temp);
								if (temp)
									collision.namePrefab = temp->name;
								break;
							}
							case (TYPEUP::CREATEPARTICLES): {
								break;
							}
							case (TYPEUP::SHADOW): {
								break;
							}
							case (TYPEUP::SPAWNEND): {
								break;
							}
							}
							TreePop();
						}
					}
					for (int i = 0; i < selectedParticles->emit[j].size(); i++)
					{
						std::string name = selectedParticles->emit[j][i].name;
						if (TreeNode(name.c_str()))
						{
							if (name == "PointPositionGen")
							{
								emit& temp = selectedParticles->emit[j][i];
								Text("Position :");
								SameLine(300);
								DragFloat3("##POSPOINT", temp.data[0].e);
							}
							else if (name == "Loop")
							{
								Text("Particles will loop");
							}
							else if (name == "BoxPositionGen")
							{
								emit& temp = selectedParticles->emit[j][i];
								Text("Position :");
								SameLine(300);
								DragFloat3("##POSBOX", temp.data[0].e);
								Text("Size Box :");
								SameLine(300);
								DragFloat3("##SIZEBOX", temp.data[1].e);
							}
							else if (name == "CirclePositionGen")
							{
								emit& temp = selectedParticles->emit[j][i];
								Text("Position :");
								SameLine(300);
								DragFloat3("##POSCIRCLE", temp.data[0].e);
								Text("Radius :");
								SameLine(200);
								DragFloat("##RADIUS", &temp.data[1].x, 0.01, 0, 15);
							}
							else if (name == "CreateParticles")
							{
								emit& temp = selectedParticles->emit[j][i];
								Text("Wich particles data ? :");
								SameLine(300);
								DragFloat("##DATANUMBER", &temp.data[0].x, 1, 0, selectedParticles->data.size() - 1);
								if (j == temp.data[0].x)
									temp.data[0].x--;
								Text("CoefScale :");
								SameLine(300);
								DragFloat("##COEFSCALE", &temp.data[1].x, 0.01, 0, 1);
								Text("Time :");
								SameLine(300);
								DragFloat("##TIME", &temp.data[1].z, 0.01, 0, 1);
								Text("CoefPos :");
								SameLine(300);
								DragFloat("##COEFPOS", &temp.data[1].y, 0.01, 0, 1);

							}
							else if (name == "Shadow")
							{
								Text("Shadow will appear");
							}
							else if (name == "SpawnEnd")
							{
								Text("Prefab :");
								SameLine(200);
								Text("curren prefab name :");
								SameLine(400);
								Text("%s", selectedParticles->emit[j][i].nameData.c_str());
								NewLine();
								ParticlesPrefab* temp = nullptr;
								ResourceMapExplorer<ParticlesPrefab>("Particles System", "##PARTICLESSYSTEMSELECTOR", manager.particles, temp);
								if (temp)
									selectedParticles->emit[j][i].nameData = temp->name != selectedParticles->name ? temp->name: selectedParticles->emit[j][i].nameData;
							}
							else if (name == "InitVelWithPoint")
							{
								Text("Velocity will be init to go in the point to attack");
							}
							TreePop();
						}
					}

					TreePop();
				}
				TreePop();
			}



		}



		NewLine();

		for (int j = 0; j < selectedParticles->data.size(); j++)
		{
			std::string name = "Remove particles : " + std::to_string(j + 1);
			if (Button(name.c_str()))
			{
				selectedParticles->data.erase(selectedParticles->data.begin() + j);
				selectedParticles->emitter.erase(selectedParticles->emitter.begin() + j);
				selectedParticles->emit.erase(selectedParticles->emit.begin() + j);
			}
		}

		NewLine();
		
		if (Button("Add particles"))
		{
			selectedParticles->data.push_back(data());
			selectedParticles->emitter.push_back(ParticlesEmitter());
			selectedParticles->emit.push_back(std::vector<emit>());
		}

		NewLine();

		if (Button("Save particles"))
		{
			Cookie::Resources::Serialization::Save::SaveParticles(*selectedParticles);
		}
	}

	ImGui::End();
}