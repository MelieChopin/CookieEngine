#ifndef __SYSTEM_HANDLER_HPP__
#define __SYSTEM_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentRigidBody.hpp"
#include "ComponentModel.hpp"
#include "ComponentScript.hpp"

#include "Core/Math/Mat4.hpp"
#include "Render/Camera.hpp"

namespace Cookie
{
	namespace Render
	{
		class RendererRemote;
	}

	namespace ECS
	{
		namespace System
		{
			//void SystemCollision(const ComponentTransform&, const ComponentRigidBody&, const ComponentCollider&);
			inline void SystemPhysics(ComponentTransform& trs, ComponentRigidBody& rb, float factor)
			{
				trs.physTransform = rb.physBody->getTransform();

				trs.Update(factor);

			}
			inline void SystemDraw(const ComponentTransform& trs, ComponentModel& model, Render::RendererRemote& remote, const Core::Math::Mat4& viewProj) 
			{
				model.Draw(remote, viewProj, trs.localTRS.ToTRS());
			}
			//void SystemRemoveBelowLevel(const ComponentTransform&);
			//void SystemRemoveNeeded();

			inline void SystemScriptStart(const ComponentScript& script)
			{
				for (int i = 0; i < script.scripts.size(); ++i)
					script.scripts[i]->Start();
			}
			inline void SystemScriptUpdate(ComponentScript& script)
			{
				for (int i = 0; i < script.scripts.size(); ++i)
					script.scripts[i]->Update();
			}

		}
	}
}

#endif
