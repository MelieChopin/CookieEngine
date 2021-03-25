#ifndef __DEBUG_RENDERER_HPP__
#define __DEBUG_RENDERER_HPP__

#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
	namespace Render
	{
		class DebugRenderer
		{
			private:
			public:
				bool showDebug = true;

				rp3d::DebugRenderer* physicsDebug;

				DebugRenderer() {}
				~DebugRenderer() {}

		};
	}
}

#endif // !__DEBUG_RENDERER_HPP__