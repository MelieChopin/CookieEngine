#include <system_error>
#include <string>
#include <memory>


#include "Render/RendererRemote.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Render/FrameBuffer.hpp"


using namespace Cookie::Render;

FrameBuffer::FrameBuffer(Resources::ResourcesManager& _resources, int width, int height):
	quad{ _resources.meshes["Quad"] }, shader{ _resources.shaders.at("Texture_Shader") }
{
    if (CreateTexture(width,height))
    {
        CreateShaderResource();
        CreateRenderTargetView();
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

bool FrameBuffer::CreateTexture(int width, int height)
{
    D3D11_TEXTURE2D_DESC desc = {};

    desc.Width              = width;
    desc.Height             = height;
    desc.MipLevels          = 1;
    desc.ArraySize          = 1;
    desc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count   = 1;
    desc.Usage              = D3D11_USAGE_DEFAULT;
    desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags     = 0;

    HRESULT result = Render::RendererRemote::device->CreateTexture2D(&desc, nullptr, &texBuffer);
    if (FAILED(result))
    {
        printf("Failing Creating FrameBuffer Texture: %s\n", std::system_category().message(result).c_str());
        return false;
    }


    return true;
}

bool FrameBuffer::CreateShaderResource()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format                      = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension               = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels         = 1;
    srvDesc.Texture2D.MostDetailedMip   = 0;

    HRESULT result = Render::RendererRemote::device->CreateShaderResourceView(texBuffer, &srvDesc, &shaderResource);
    if (FAILED(result))
    {
        printf("Failing Creating FrameBuffer ShaderResource: %s\n", std::system_category().message(result).c_str());
        return false;
    }

    return true;
}

bool FrameBuffer::CreateRenderTargetView()
{
    D3D11_RENDER_TARGET_VIEW_DESC desc = {};

    desc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;

    HRESULT result = Render::RendererRemote::device->CreateRenderTargetView(texBuffer, &desc, &renderTargetView);
    if (FAILED(result))
    {
        printf("Failing Creating FrameBuffer ShaderResource: %s\n", std::system_category().message(result).c_str());
        return false;
    }

    return true;
}

void FrameBuffer::Resize(int width, int height)
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

    if (CreateTexture(width,height))
    {
        CreateShaderResource();
        CreateRenderTargetView();
    }
}

void FrameBuffer::Draw()
{
    if (shader)
    {
        shader->Set(Core::Math::Mat4::Identity(), Core::Math::Mat4::TRS(Cookie::Core::Math::Vec3(0.0f,0.0f,0.99f),Cookie::Core::Math::Vec3(Core::Math::PI,0.0f,0.0f),Core::Math::Vec3(1.0f,1.0f,1.0f)));
    }
    if (shaderResource)
    {
        Render::RendererRemote::context->PSSetShaderResources(0,1,&shaderResource);
    }
    if (quad)
    {
        quad->Set();
        quad->Draw();
    }
}