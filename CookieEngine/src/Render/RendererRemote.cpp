#include <d3d11.h>
#include "Render/RendererRemote.hpp"

using namespace Cookie::Render;
ID3D11Device* RendererRemote::device = nullptr;
ID3D11DeviceContext* RendererRemote::context = nullptr;

/*========================= CONSTRUCTORS/DESTRUCTORS ===========================*/

RendererRemote::RendererRemote()
{
}

RendererRemote::~RendererRemote()
{
}