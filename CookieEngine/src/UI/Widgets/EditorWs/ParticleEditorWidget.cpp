#include "ParticleEditorWidget.hpp"
#include "Particles/ParticlesSystem.hpp"
#include "MapExplorerHelper.hpp"
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

		ResourceMapExplorer<ParticlesPrefab>("Particles System", "##PARTICLESSYSTEMSELECTOR", manager.particles, selectedParticles);

		if (!selectedParticles)
		{
			ImGui::End();
			return;
		}
			

		for (int i = 0; i < selectedParticles->data.size(); i++)
		{
			std::string name = "Particles : " + std::to_string(i + 1);
			if (TreeNode(name.c_str()))
			{
				data& dataPrefab = selectedParticles->data[i];

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

				TreePop();
			}

		}
		
	}

	ImGui::End();
}