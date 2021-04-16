#include <d3d11.h>

#include "Render/RendererRemote.hpp"

#include "Resources/ResourcesManager.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Shader/SkyBoxShader.hpp"

#include "Skybox.hpp"

using namespace Cookie::Render;

/*==================== CONSTRUCTORS/DESTRUCTORS ====================*/

SkyBox::SkyBox(Resources::ResourcesManager& _resources):
	cube {_resources.meshes["Cube"]}, shader {_resources.shaders["SkyBox_Shader"]}
{
    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = false;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    RendererRemote::device->CreateRasterizerState(&rasterDesc, &rasterizerState);
}

SkyBox::~SkyBox()
{
    if (rasterizerState)
        rasterizerState->Release();
}

/*==================== REALTIME METHODS ====================*/

void SkyBox::Draw(const Core::Math::Mat4& proj, const Core::Math::Mat4& view)
{
    RendererRemote::context->RSSetState(rasterizerState);
	if (shader)
		shader->Set(proj, view);
	if (texture)
		texture->Set();
	if (cube)
	{
		cube->Set();
		cube->Draw();
	}
}