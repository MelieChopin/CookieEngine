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
			inline void SystemGravity(ComponentRigidBody& rb) { rb.AddForce({ 0, -9.81f * rb.mass, 0 }); }
			//void SystemCollision(const ComponentTransform&, const ComponentRigidBody&, const ComponentCollider&);
			inline void SystemVelocity(ComponentTransform& trs, const ComponentRigidBody& rb) {trs.localTRS.translation += rb.linearVelocity /* * deltaTime*/; };
			inline void SystemDraw(const ComponentTransform& trs, ComponentModel& model, Render::RendererRemote& remote, const Core::Math::Mat4& viewProj) 
			{
				Core::Math::Mat4 mat4 = { -0.7289686799049377,
					0.0,
					-0.6845470666885376,
					0.0,
					-0.4252049028873444,
					0.7836934328079224,
					0.4527972936630249,
					0.0,
					0.5364750623703003,
					0.6211478114128113,
					-0.571287989616394,
					0.0,
					400.1130065917969,
					463.2640075683594,
					-431.0780334472656,
					1.0 };
				
				model.Draw(remote,viewProj,mat4);
			}
			//void SystemRemoveBelowLevel(const ComponentTransform&);
			//void SystemRemoveNeeded();

		}
	}
}

#endif