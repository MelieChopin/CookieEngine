#include "Resources/Shader.hpp"
#include "Render/RendererRemote.hpp"

using namespace Cookie::Resources;
using namespace Cookie::Render;

Shader*					RendererRemote::currentShader = nullptr;
ID3D11Device*			RendererRemote::device = nullptr;
ID3D11DeviceContext*	RendererRemote::context = nullptr;


/*========================= CONSTRUCTORS/DESTRUCTORS ===========================*/

RendererRemote::RendererRemote()
{
}

RendererRemote::~RendererRemote()
{
}