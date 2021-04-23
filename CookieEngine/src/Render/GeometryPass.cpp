#include "Resources/Shader/GeometryShader.hpp"
#include "Render/GeometryPass.hpp"

using namespace Cookie::Render;

/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

GeometryPass::GeometryPass(int width, int height):
	gShader{std::make_unique<Resources::GeometryShader>("Geometry_Shader")},
	posFBO		{ width,height},
	normalFBO	{ width,height },
	albedoFBO	{ width,height }
{
}

GeometryPass::~GeometryPass()
{

}

/*=========================== REALTIME METHODS ===========================*/

void GeometryPass::Set(ID3D11DepthStencilView* depthStencilView)
{
	gShader->Set();

	ID3D11RenderTargetView* fbos[3] = {posFBO.renderTargetView,normalFBO.renderTargetView,albedoFBO.renderTargetView};

	Render::RendererRemote::context->OMSetRenderTargets(3, fbos, depthStencilView);
}

void GeometryPass::Clear(const Core::Math::Vec4& clearColor)
{
	float black[4] = { 0.0f,0.0f,0.0f,0.0f };

	Render::RendererRemote::context->ClearRenderTargetView(posFBO.renderTargetView,		black);
	Render::RendererRemote::context->ClearRenderTargetView(normalFBO.renderTargetView,  black);
	Render::RendererRemote::context->ClearRenderTargetView(albedoFBO.renderTargetView,	clearColor.e);
}