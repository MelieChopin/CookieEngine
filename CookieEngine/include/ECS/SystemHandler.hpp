#ifndef __SYSTEM_HANDLER_HPP__
#define __SYSTEM_HANDLER_HPP__

#include "Physics/PhysicsHandle.hpp"

#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentPhysics.hpp"
#include "ComponentScript.hpp"


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
			inline void SystemPhysics(ComponentPhysics& ph, float factor);
			inline void SystemDraw(ComponentTransform& trs, ComponentModel& model, const Core::Math::Mat4& viewProj, ID3D11Buffer** CBuffer);
			inline void SystemScriptStart(const ComponentScript& script);
			inline void SystemScriptUpdate(ComponentScript& script);

		}
	}
}


#include "ECS/SystemHandler.inl"


#endif
