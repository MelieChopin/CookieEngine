#ifndef __DEBUG_RENDERER_HPP__
#define __DEBUG_RENDERER_HPP__

#include "Physics/PhysicsHandle.hpp"
#include "Resources/Shader/PhysicsShader.hpp"

namespace Cookie
{
	namespace Render
	{
		class DebugRenderer
		{
			private:
				Physics::PhysicsHandle		physHandle;
				ID3D11Buffer*				VBuffer			{ nullptr };
				D3D11_BUFFER_DESC			bDesc			= {};
				Resources::PhysicsShader	physShader		= {"Physics_Shader"};
				ID3D11RasterizerState*		rasterState		{ nullptr };

				void InitRasterizerState();
				void AllocateVBuffer(size_t vBufferSize);
				void UpdateVBuffer(size_t vBufferSize, void* data);

			public:
				bool showDebug = true;

				void SetPhysicsRendering();

				void Draw(const Core::Math::Mat4& viewProj);

				DebugRenderer();
				~DebugRenderer();

		};
	}
}

#endif // !__DEBUG_RENDERER_HPP__