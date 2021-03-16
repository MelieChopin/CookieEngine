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
			inline void SystemVelocity(ComponentTransform& trs, ComponentRigidBody& rb)
			{
				//if (rb.goTowardTarget)
				//{
				//	//if not reach the target Pos
				//	if((rb.targetPosition - trs.localTRS.translation).Length() > 0.1)
				//		rb.linearVelocity = (rb.targetPosition - trs.localTRS.translation).Normalize() * rb.speed;
				//	//if reached
				//	else
				//	{
				//		rb.goTowardTarget = false;
				//		rb.targetPosition = {0, 0, 0};
				//		rb.linearVelocity = {0, 0, 0};
				//	}
				//}
				//
				//trs.localTRS.translation += rb.linearVelocity * Core::deltaTime; 
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