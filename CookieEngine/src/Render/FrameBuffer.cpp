#include <system_error>
#include <string>
#include <memory>
#include <d3d11.h>

#include "Core/Math/Calc.hpp"
#include "Render/RendererRemote.hpp"
#include "Render/Renderer.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Mesh.hpp"
#include "Render/FrameBuffer.hpp"
#include "Core/Primitives.hpp"


using namespace Cookie::Render;

FrameBuffer::FrameBuffer(Resources::ResourcesManager& _resources, Renderer& _renderer):
	quad{ _resources.HasMesh("Quad") ? _resources.GetMesh("Quad") : _resources.AddMesh(Core::Primitives::CreateQuad(_renderer))}, shader{_resources.GetDefaultShader() == nullptr? _resources.AddShader(std::move(std::make_shared<Resources::Shader>(_renderer))): _resources.GetDefaultShader() }
{
    if (CreateTexture(_renderer))
    {
        CreateShaderResource(_renderer);
        CreateRenderTargetView(_renderer);
    }
}

FrameBuffer::~FrameBuffer()
{
    if (texBuffer)
    {
        texBuffer->Release();
        texBuffer = nullptr;
    }
    if (shaderResource)
    {
        shaderResource->Release();
        shaderResource = nullptr;
    }
    if (renderTargetView)
    {
        renderTargetView->Release();
        renderTargetView = nullptr;
    }
}

bool FrameBuffer::CreateTexture(Renderer& _renderer)
{
    D3D11_TEXTURE2D_DESC desc = {};

    desc.Width              = _renderer.state.viewport.Width;
    desc.Height             = _renderer.state.viewport.Height;
    desc.MipLevels          = 1;
    desc.ArraySize          = 1;
    desc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count   = 1;
    desc.Usage              = D3D11_USAGE_DEFAULT;
    desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags     = 0;

    HRESULT result = _renderer.GetDevice()->CreateTexture2D(&desc, nullptr, &texBuffer);
    if (FAILED(result))
    {
        printf("Failing Creating FrameBuffer Texture: %s\n", std::system_category().message(result).c_str());
        return false;
    }


    return true;
}

bool FrameBuffer::CreateShaderResource(Renderer& _renderer)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format                      = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension               = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels         = 1;
    srvDesc.Texture2D.MostDetailedMip   = 0;

    HRESULT result = _renderer.GetDevice()->CreateShaderResourceView(texBuffer, &srvDesc, &shaderResource);
    if (FAILED(result))
    {
        printf("Failing Creating FrameBuffer ShaderResource: %s\n", std::system_category().message(result).c_str());
        return false;
    }

    return true;
}

bool FrameBuffer::CreateRenderTargetView(Renderer& _renderer)
{
    D3D11_RENDER_TARGET_VIEW_DESC desc = {};

    desc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;

    HRESULT result = _renderer.GetDevice()->CreateRenderTargetView(texBuffer, &desc, &renderTargetView);
    if (FAILED(result))
    {
        printf("Failing Creating FrameBuffer ShaderResource: %s\n", std::system_category().message(result).c_str());
        return false;
    }

    return true;
}

void FrameBuffer::Resize(Renderer& _renderer)
{
    if (texBuffer)
    {
        texBuffer->Release();
        texBuffer = nullptr;
    }
    if (shaderResource)
    {
        shaderResource->Release();
        shaderResource = nullptr;
    }
    if (renderTargetView)
    {
        renderTargetView->Release();
        renderTargetView = nullptr;
    }

    if (CreateTexture(_renderer))
    {
        CreateShaderResource(_renderer);
        CreateRenderTargetView(_renderer);
    }
}

void FrameBuffer::Draw(Render::RendererRemote& _remote)
{
    if (shader)
    {
        shader->Set(_remote, Core::Math::Mat4::Identity(), Core::Math::Mat4::TRS(Cookie::Core::Math::Vec3(0.0f,0.0f,0.99f),Cookie::Core::Math::Vec3(Core::Math::PI,0.0f,0.0f),Core::Math::Vec3(1.0f,1.0f,1.0f)));
    }
    if (shaderResource)
    {
        _remote.context->PSSetShaderResources(0,1,&shaderResource);
    }
    if (quad)
    {
        quad->Set(_remote);
        quad->Draw(_remote);
    }
}