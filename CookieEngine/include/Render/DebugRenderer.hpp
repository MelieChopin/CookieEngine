#ifndef __DEBUG_RENDERER_HPP__
#define __DEBUG_RENDERER_HPP__

#include "Physics/PhysicsHandle.hpp"

namespace Cookie
{
	namespace Render
	{
		class DebugRenderer
		{
			private:
				Physics::PhysicsHandle physHandle;

			public:
				bool showDebug = true;

				void SetPhysicsRendering();

				void Draw();

				DebugRenderer();
				~DebugRenderer();

		};
	}
}

#endif // !__DEBUG_RENDERER_HPP__