#ifndef __SYSTEM_HANDLER_HPP__
#define __SYSTEM_HANDLER_HPP__

#include "Physics/PhysicsHandle.hpp"

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
			inline void SystemPhysics(ComponentPhysics& ph, float factor)
			{
				ph.Update(factor);

			}
			inline void SystemDraw(ComponentTransform& trs, ComponentModel& model, const Core::Math::Mat4& viewProj)
			{
				trs.ComputeTRS();
				model.Draw(viewProj, trs.TRS);
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
