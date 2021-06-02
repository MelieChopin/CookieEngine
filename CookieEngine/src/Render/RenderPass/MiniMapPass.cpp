#include "Core/Math/Mat4.hpp"
#include "D3D11Helper.hpp"
#include "DrawDataHandler.hpp"
#include "ECS/ComponentGameplay.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "FrameBuffer.hpp"
#include "Render/RenderPass/MiniMapPass.hpp"
#include "Resources/Scene.hpp"
#include "Render/Camera.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

constexpr float zEpsilon = 50.0f;

struct CAM_CONSTANT_BUFFER
{
	Mat4 proj = Mat4::Identity();
	Mat4 view = Mat4::Identity();
};

struct VS_CONSTANT_BUFFER
{
	Vec4 color1 = { PLAYER_ARMY_COLOR,(float)Cookie::Gameplay::E_ARMY_NAME::E_PLAYER };
	Vec4 color2 = { AI1_ARMY_COLOR,(float)Cookie::Gameplay::E_ARMY_NAME::E_AI1 };
};


/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

MiniMapPass::MiniMapPass()
{
	miniMapView = Mat4::Translate({0.0f,-zEpsilon,0.0f}) * Mat4::RotateX(ToRadians(90.0f));

	InitState();
}

MiniMapPass::~MiniMapPass()
{
	if (depthStencilState)
	{
		depthStencilState->Release();
	}
	if (rasterState)
	{
		rasterState->Release();
	}
}

/*=========================== INIT METHODS ===========================*/

void MiniMapPass::InitState()
{
    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable	= true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable	= false;

    RendererRemote::device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = true;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    RendererRemote::device->CreateRasterizerState(&rasterDesc, &rasterState);
}

/*=========================== REALTIME METHODS ===========================*/

void MiniMapPass::Draw(DrawDataHandler& drawData, const FrameBuffer& framebuffer)
{

	const Camera& cam = *drawData.currentCam;
	const Resources::Map& map = drawData.currentScene->map;

	float width = map.trs.scale.x;
	float height = map.trs.scale.z;

	aspectRatio = 1280.0f / 720.0f;

	ortho = Mat4::Ortho(-width * 0.5f * aspectRatio, width * 0.5f * aspectRatio, -height * 0.5f, height * 0.5f, -zEpsilon, zEpsilon);

	viewport.Width = framebuffer.width;
	viewport.Height = framebuffer.height;

	Render::RendererRemote::context->RSSetViewports(1, &viewport);

	ID3D11Buffer* CBuffers[2] = { miniModelDrawer.CBuffer, miniModelDrawer.CamCBuffer };
	Render::RendererRemote::context->VSSetConstantBuffers(0, 2, CBuffers);

	CAM_CONSTANT_BUFFER buffer = { ortho, miniMapView };

	Render::WriteBuffer(&buffer, sizeof(buffer), 0, &miniModelDrawer.CamCBuffer);

	miniModelDrawer.Set();
	miniModelDrawer.Draw(drawData.staticDrawData);
	miniModelDrawer.Draw(drawData.dynamicDrawData);

	// Now set the rasterizer state.
	Render::RendererRemote::context->RSSetState(rasterState);
	// Set the depth stencil state.
	Render::RendererRemote::context->OMSetDepthStencilState(depthStencilState, 1);

	miniMapDrawer.Set(cam,map);
	miniMapDrawer.Draw();
}