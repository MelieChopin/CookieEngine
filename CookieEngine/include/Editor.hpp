#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "Game.hpp"
#include "UI/UIeditor.hpp"
#include "Render/Camera.hpp"
#include "DebugRenderer.hpp"

#include "ECS/ComponentEditor.hpp"
#include "ECS/ComponentHandler.hpp"

#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"

namespace Cookie
{
	namespace ECS
	{
		class Entity;
	}
	namespace Resources
	{
		class Scene;
	}


	struct FocusEntity
	{
		int						toChangeEntityId{ -1 };
		ECS::Entity*			focusedEntity	{ nullptr };
		ECS::ComponentHandler*	componentHandler{ nullptr };
		ECS::ComponentEditor*	editComp		{ nullptr };

	};

	class Editor : public rp3d::RaycastCallback
	{
		private:
		public:


			UI::UIeditor			editorUI;
			Game					game;
			Render::FreeFlyCam		cam;
			Render::FrameBuffer		editorFBO;

			std::array<ECS::ComponentEditor, MAX_ENTITIES> editingComponent;
			Render::DebugRenderer	dbgRenderer;

			FocusEntity			selectedEntity	= {};
			Resources::Scene*	currentScene	= nullptr;

			bool				isPlaying = false;
			
		private:
		public:
			Editor();
			~Editor();


			void InitEditComp();
			void ModifyEditComp();
			void TryResizeWindow();

			inline void PopulateFocusedEntity()
			{
				if (selectedEntity.editComp && selectedEntity.editComp->body)
				{
					if ((selectedEntity.focusedEntity->signature & SIGNATURE_MODEL) && game.coordinator.componentHandler->GetComponentModel(selectedEntity.focusedEntity->id).mesh != nullptr)
					{
						selectedEntity.editComp->AABBMin = game.coordinator.componentHandler->GetComponentModel(selectedEntity.focusedEntity->id).mesh->AABBMin;
						selectedEntity.editComp->AABBMax = game.coordinator.componentHandler->GetComponentModel(selectedEntity.focusedEntity->id).mesh->AABBMax;
						selectedEntity.editComp->MakeCollider();
					}
					selectedEntity.editComp->editTrs = &game.coordinator.componentHandler->GetComponentTransform(selectedEntity.focusedEntity->id);
					selectedEntity.editComp->Update();
				}

				selectedEntity.focusedEntity = &game.coordinator.entityHandler->entities[selectedEntity.toChangeEntityId];
				selectedEntity.editComp = &editingComponent[selectedEntity.toChangeEntityId];
				selectedEntity.toChangeEntityId = -1;
			}
			inline virtual float notifyRaycastHit(const rp3d::RaycastInfo& info)
			{
				for (int i = 0; i < MAX_ENTITIES; i++)
				{
					if (editingComponent[i].body == info.body)
					{
						selectedEntity.toChangeEntityId = i;
						PopulateFocusedEntity();
					}
				}

				return 0.0f;
			}

			void Loop();

	};
}

#endif // !__EDITOR_HPP__

#ifndef USING_EDITOR
#define USING_EDITOR
#endif // !USING_EDITOR
