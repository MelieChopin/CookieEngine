#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Game.hpp"
#include "Resources/Scene.hpp"
#include "Render/Renderer.hpp"
#include "ImGui/imgui.h"


using namespace Cookie::Render;
using namespace Cookie::Core::Math;

/*========================= CONSTRUCTORS/DESTRUCTORS ===========================*/

Renderer::Renderer():
    remote {InitDevice(window)},
    viewport {0.0f,0.0f,static_cast<float>(window.width),static_cast<float>(window.height),0.0f,0.9999999f},
    gPass{window.width,window.height},
    lPass{window.width,window.height}
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
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// use 32-bit color
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
    

    if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        return false;

    // use the back buffer address to create the render target
    if (FAILED(remote.device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer)))
        return false;

    pBackBuffer->Release();

    return true;
}

/*========================= CALLBACK METHODS =========================*/

void Renderer::ResizeBuffer(int width, int height)
{
    remote.context->ClearState();

    ID3D11RenderTargetView* nullViews[] = { nullptr,nullptr,nullptr,nullptr };

    remote.context->OMSetRenderTargets(4, nullViews, nullptr);

    gPass.depthBuffer->Release();
    backbuffer->Release();

    HRESULT result = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    if (FAILED(result))
    {
        printf("%s", (std::string("Failing Resizing SwapChain Buffer : ") + std::system_category().message(result)).c_str());
    }

    gPass.CreateDepth(width,height);
    CreateDrawBuffer(width,height);

    remote.context->OMSetRenderTargets(1, &backbuffer, nullptr);

    gPass.posFBO.Resize(width,height);
    gPass.normalFBO.Resize(width, height);
    gPass.albedoFBO.Resize(width, height);
    lPass.diffuseFBO.Resize(width, height);
    lPass.specularFBO.Resize(width, height);

    remote.context->ClearState();
    remote.context->Flush();


    viewport.Width = width;
    viewport.Height = height;
    remote.context->RSSetViewports(1, &viewport);
}

/*========================= RENDER METHODS =========================*/

void Renderer::Draw(const Camera* cam, Game& game, FrameBuffer& framebuffer)
{
    ID3D11RenderTargetView* nullViews[] = { nullptr,nullptr,nullptr,nullptr };
    remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);

    Core::Math::Mat4 proj = cam->GetProj();
    Core::Math::Mat4 view = cam->GetView();

    gPass.Set();

    game.scene->map.Draw(proj,view,&gPass.CBuffer);

    gPass.Draw(*cam,game.coordinator);

    remote.context->OMSetRenderTargets(4, nullViews, nullptr);

    lPass.Set(gPass.posFBO,gPass.normalFBO,gPass.albedoFBO,cam->pos);
    lPass.Draw();

    remote.context->OMSetRenderTargets(4, nullViews, nullptr);

    if (ImGui::GetIO().KeysDownDuration[GLFW_KEY_F1] >= 0.0f)
    {
        remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);
        DrawFrameBuffer(game.renderer.gPass.posFBO);
    }
    else if (ImGui::GetIO().KeysDownDuration[GLFW_KEY_F2] >= 0.0f)
    {
        remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);
        DrawFrameBuffer(game.renderer.gPass.normalFBO);
    }
    else if (ImGui::GetIO().KeysDownDuration[GLFW_KEY_F3] >= 0.0f)
    {
        remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);
        DrawFrameBuffer(game.renderer.gPass.albedoFBO);
    }
    else if (ImGui::GetIO().KeysDownDuration[GLFW_KEY_F4] >= 0.0f)
    {
        remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);
        DrawFrameBuffer(lPass.diffuseFBO);
    }
    else if (ImGui::GetIO().KeysDownDuration[GLFW_KEY_F5] >= 0.0f)
    {
        remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);
        DrawFrameBuffer(lPass.specularFBO);
    }
    else
    {
        remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, nullptr);
        cPass.Set(lPass.diffuseFBO,lPass.specularFBO);
        cPass.Draw();
    }

    remote.context->OMSetRenderTargets(1, &framebuffer.renderTargetView, gPass.depthBuffer);

    game.skyBox.Draw(cam->GetProj(), cam->GetView());
}

void Renderer::DrawFrameBuffer(FrameBuffer& fbo)
{
    fboDrawer.Set();
    remote.context->PSSetShaderResources(0, 1, &fbo.shaderResource);
    remote.context->Draw(3, 0);
}

void Renderer::Clear()
{
    ID3D11RenderTargetView* nullViews[] = { nullptr,nullptr,nullptr,nullptr };

    remote.context->OMSetRenderTargets(4, nullViews, nullptr);

    ID3D11ShaderResourceView* resources[3] = { nullptr,nullptr,nullptr };

    Render::RendererRemote::context->PSSetShaderResources(0, 3, resources);

    Core::Math::Vec4 clearColor = {0.0f,0.0f,0.0f,1.0f};

    remote.context->ClearRenderTargetView(backbuffer, clearColor.e);
    remote.context->ClearDepthStencilView(gPass.depthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    ID3D11ShaderResourceView* null = nullptr;

    remote.context->PSSetShaderResources(0, 1, &null);

    gPass.Clear();
    remote.context->ClearRenderTargetView(lPass.diffuseFBO.renderTargetView, clearColor.e);
    remote.context->ClearRenderTargetView(lPass.specularFBO.renderTargetView, clearColor.e);
}

void Renderer::ClearFrameBuffer(FrameBuffer& fbo)
{
    Core::Math::Vec4 clearColor = { 0.0f,0.0f,0.0f,1.0f };
    remote.context->ClearRenderTargetView(fbo.renderTargetView, clearColor.e);
}

void Renderer::SetBackBuffer()
{
    remote.context->OMSetRenderTargets(1, &backbuffer, nullptr);
}

void Renderer::Render()const
{
    // switch the back buffer and the front buffer
    remote.context->OMSetRenderTargets(1, &backbuffer, nullptr);
    swapchain->Present(1, 0);
}