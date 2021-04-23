#ifndef __DEBUG_RENDERER_HPP__
#define __DEBUG_RENDERER_HPP__


#include "Resources/Shader/PhysicsShader.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
	namespace Core
	{
		namespace Primitives
		{
			struct DebugVertex;
		}
	}

	namespace Render
	{
		class DebugRenderer
		{
			private:
				ID3D11Buffer*										VBuffer			{ nullptr };
				D3D11_BUFFER_DESC									bDesc			= {};
				Resources::PhysicsShader							physShader		= {"Physics_Shader"};
				ID3D11RasterizerState*								rasterState		{ nullptr };
				rp3d::DebugRenderer&								physDbgRenderer;
				std::vector<Core::Primitives::DebugVertex>			debugElement;

				void InitRasterizerState();
				void AllocateVBuffer(size_t vBufferSize);
				void UpdateVBuffer(size_t vBufferSize, void* data);

			public:
				bool showDebug = true;

				void Draw(const Core::Math::Mat4& viewProj);
				void AddDebugElement(const std::vector<Core::Primitives::DebugVertex>& dbgElement);

				inline void Reset()noexcept { physDbgRenderer.reset(); };

				DebugRenderer();
				~DebugRenderer();

		};
	}
}

#endif // !__DEBUG_RENDERER_HPP__