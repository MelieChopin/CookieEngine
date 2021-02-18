#ifndef __SYSTEM_HANDLER_HPP__
#define __SYSTEM_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentRigidBody.hpp"
#include "Core/Math/Mat4.hpp"

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
			inline void SystemGravity(ComponentRigidBody& rb) { rb.AddForce({ 0, -9.81f * rb.mass, 0 }); }
			//void SystemCollision(const ComponentTransform&, const ComponentRigidBody&, const ComponentCollider&);
			inline void SystemVelocity(ComponentTransform& trs, const ComponentRigidBody& rb) {trs.localTRS.translation += rb.linearVelocity /* * deltaTime*/; };
			inline void SystemDraw(const ComponentTransform& trs, ComponentModel& model, Render::RendererRemote& remote, const Core::Math::Mat4& viewproj) 
			{
				model.Draw(remote, viewproj * trs.globalTRS);
			}
			//void SystemRemoveBelowLevel(const ComponentTransform&);
			//void SystemRemoveNeeded();

		}
	}
}

#endif