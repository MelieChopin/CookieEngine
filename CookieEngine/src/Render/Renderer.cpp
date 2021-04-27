#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Game.hpp"
#include "Resources/Scene.hpp"
#include "Render/Renderer.hpp"
#include "imgui.h"


using namespace Cookie::Render;
using namespace Cookie::Core::Math;

/*========================= CONSTRUCTORS/DESTRUCTORS ===========================*/

Renderer::Renderer():
    remote {InitDevice(window)},
    viewport {0.0f,0.0f,static_cast<float>(window.width),static_cast<float>(window.height),0.0f,1.0f},
    gPass{window.width,window.height}
{
    CreateDrawBuffer(window.width,window.height);
    remote.context->RSSetViewports(1, &viewport);
}

Renderer::~Renderer()
{
    remote.context->ClearState();

    ID3D11RenderTargetView* nullViews[] = { nullptr,nullptr,nullptr,nullptr };

    remote.context->OMSetRenderTargets(4, nullViews, nullptr);

    ID3D11SamplerState* null[] = { nullptr};

    remote.context->PSSetSamplers(0, 1, null);

    remote.context->VSSetShader(nullptr,0,0);
    remote.context->PSSetShader(nullptr,0,0);

    if (swapchain)
        swapchain->Release();
    if (backbuffer)
        backbuffer->Release();
    if (depthBuffer)
        depthBuffer->Release();
    if (remote.context)
    {
        remote.context->Release();
    }
    if (remote.device)
        remote.device->Release();

    remote.context->Flush();
}

/*========================= INIT METHODS =========================*/

RendererRemote Renderer::InitDevice(Core::Window& window)
{
    RendererRemote _remote;

    DXGI_SWAP_CHAIN_DESC scd = {};

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = glfwGetWin32Window(window.window);   // the window to be used
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.SampleDesc.Count = 1;                               // how many multisamples
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // create a device, device context and swap chain using the information in the scd struct
    HRESULT result = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
#if 1
        D3D11_CREATE_DEVICE_DEBUG,
#else
        NULL,
#endif
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &_remote.device,
        NULL,
        &_remote.context);

    if (FAILED(result))
    {
        printf("Failed Creating Device: %s\n", std::system_category().message(result).c_str());
        return _remote;
    }

    return _remote;
}

bool Renderer::CreateDrawBuffer(int width, int height)
{
    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer = nullptr;
    ID3D11Texture2D* depthTexture = nullptr;

    if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        return false;

    // use the back buffer address to create the render target
    if (FAILED(remote.device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer)))
        return false;

    D3D11_TEXTURE2D_DESC depthBufferDesc = {};

    // Set up the description of the depth buffer.
    depthBufferDesc.Width               = width;
    depthBufferDesc.Height              = height;
    depthBufferDesc.MipLevels           = 1;
    depthBufferDesc.ArraySize           = 1;
    depthBufferDesc.Format              = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count    = 1;
    depthBufferDesc.SampleDesc.Quality  = 0;
    depthBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags           = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags      = 0;
    depthBufferDesc.MiscFlags           = 0;

    if (FAILED(remote.device->CreateTexture2D(&depthBufferDesc, NULL, &depthTexture)))
        return false;

    // Initialize the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    if (FAILED(remote.device->CreateDepthStencilView(depthTexture, &depthStencilViewDesc, &depthBuffer)))
        return false;

    pBackBuffer->Release();
    depthTexture->Release();

    return true;
}

/*========================= CALLBACK METHODS =========================*/

void Renderer::ResizeBuffer(int width, int height)
{
    remote.context->ClearState();

    ID3D11RenderTargetView* nullViews[] = { nullptr,nullptr,nullptr,nullptr };

    remote.context->OMSetRenderTargets(4, nullViews, nullptr);

    backbuffer->Release();
    depthBuffer->Release();

    HRESULT result = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    if (FAILED(result))
    {
        printf("%s", (std::string("Failing Resizing SwapChain Buffer : ") + std::system_category().message(result)).c_str());
    }

    CreateDrawBuffer(width,height);

    remote.context->OMSetRenderTargets(1, &backbuffer, depthBuffer);

    viewport.Width = width;
    viewport.Height = height;
    remote.context->RSSetViewports(1, &viewport);

    gPass.posFBO.Resize(width,height);
    gPass.normalFBO.Resize(width, height);
    gPass.albedoFBO.Resize(width, height);

    remote.context->ClearState();
    remote.context->Flush();
}

/*========================= RENDER METHODS =========================*/

void Renderer::Draw(const Camera* cam, Game& game, FrameBuffer& framebuffer)
{

    remote.context->OMSetRenderTargets(1, &gPass.albedoFBO.renderTargetView, nullptr);

    Core::Math::Mat4 viewProj = cam->GetViewProj();
    
    game.skyBox.Draw(cam->GetProj(), cam->GetView());

    gPass.Set(depthBuffer);

    game.scene->map.Draw(viewProj,&gPass.CBuffer);

    gPass.Draw(viewProj,game.coordinator);

    remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);

    if (ImGui::GetIO().KeysDownDuration[GLFW_KEY_F1] >= 0.0f)
    {
        game.renderer.DrawFrameBuffer(game.renderer.gPass.posFBO);
    }
    else if (ImGui::GetIO().KeysDownDuration[GLFW_KEY_F2] >= 0.0f)
    {
        game.renderer.DrawFrameBuffer(game.renderer.gPass.normalFBO);
    }
    else
    {
        game.renderer.DrawFrameBuffer(game.renderer.gPass.albedoFBO);
    }
    
    remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, depthBuffer);
}

void Renderer::DrawFrameBuffer(FrameBuffer& fbo)
{
    fboDrawer.Set();
    remote.context->PSSetShaderResources(0, 1, &fbo.shaderResource);
    remote.context->Draw(3, 0);
}

void Renderer::Clear()
{
    Core::Math::Vec4 clearColor = {0.0f,0.0f,0.0f,1.0f};

    remote.context->ClearRenderTargetView(backbuffer, clearColor.e);
    remote.context->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    ID3D11ShaderResourceView* null = nullptr;

    remote.context->PSSetShaderResources(0, 1, &null);

    gPass.Clear(clearColor);
}

void Renderer::ClearFrameBuffer(FrameBuffer& fbo)
{
    Core::Math::Vec4 clearColor = { 0.0f,0.0f,0.0f,1.0f };
    remote.context->ClearRenderTargetView(fbo.renderTargetView, clearColor.e);
}

void Renderer::SetBackBuffer()
{
    remote.context->OMSetRenderTargets(1, &backbuffer, depthBuffer);
}

void Renderer::Render()const
{
    // switch the back buffer and the front buffer
    remote.context->OMSetRenderTargets(1, &backbuffer, nullptr);
    swapchain->Present(1, 0);
}