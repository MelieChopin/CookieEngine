#ifndef __SYSTEM_HANDLER_HPP__
#define __SYSTEM_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentPhysics.hpp"
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
			inline void SystemPhysics(ComponentTransform& trs, ComponentPhysics& ph, float factor)
			{
				trs.physTransform = ph.physBody->getTransform();

				trs.Update(factor);

			}
			inline void SystemDraw(ComponentTransform& trs, ComponentModel& model, Render::RendererRemote& remote, const Core::Math::Mat4& viewProj) 
			{
				trs.localTRS.ComputeTRS();
				model.Draw(remote, viewProj, trs.localTRS.TRS);
			}
			//void SystemRemoveBelowLevel(const ComponentTransform&);
			//void SystemRemoveNeeded();

			inline void SystemScriptStart(const ComponentScript& script)
			{
				for (int i = 0; i < script.scripts.size(); ++i)
					script.scripts[i].Start();
			}
			inline void SystemScriptUpdate(ComponentScript& script)
			{
				for (int i = 0; i < script.scripts.size(); ++i)
					script.scripts[i].Update();
			}

		}
	}
}

#endif
