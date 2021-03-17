#ifndef __SYSTEM_HANDLER_HPP__
#define __SYSTEM_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentRigidBody.hpp"
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

		}
	}
}

#endif