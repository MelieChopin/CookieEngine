#include <d3d11.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Render/Renderer.hpp"
#include "Render/FrameBuffer.hpp"

using namespace Cookie::Render;


/*========================= CONSTRUCTORS/DESTRUCTORS ===========================*/

Renderer::Renderer(Core::Window& window)
{
    bool result = true;
    result = InitDevice(window);
    if (result)
        result = CreateDrawBuffer(window.width,window.height);
    if (result)
        result = InitState(window.width, window.height);

}

Renderer::~Renderer()
{
    if (swapchain)
        swapchain->Release();
    if (backbuffer)
        backbuffer->Release();
    if (depthBuffer)
        depthBuffer->Release();
    if (device)
        device->Release();
}

/*========================= INIT METHODS =========================*/

bool Renderer::InitDevice(Core::Window& window)
{
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
    if (D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
#if 0
        D3D11_CREATE_DEVICE_DEBUG,
#else
        NULL,
#endif
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &device,
        NULL,
        &remote.context))
        return false;

    return true;
}

bool Renderer::CreateDrawBuffer(int width, int height)
{
    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer = nullptr;
    ID3D11Texture2D* depthTexture = nullptr;

    if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        return false;

    // use the back buffer address to create the render target
    if (FAILED(device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer)))
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

    if (FAILED(device->CreateTexture2D(&depthBufferDesc, NULL, &depthTexture)))
        return false;

    // Initialize the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    if (FAILED(device->CreateDepthStencilView(depthTexture, &depthStencilViewDesc, &depthBuffer)))
        return false;

    pBackBuffer->Release();
    depthTexture->Release();

    return true;
}

bool Renderer::InitState(int width, int height)
{
    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    device->CreateDepthStencilState(&depthStencilDesc, &state.depthStencilState);

    // Set the depth stencil state.
    remote.context->OMSetDepthStencilState(state.depthStencilState, 1);

    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    device->CreateRasterizerState(&rasterDesc, &state.rasterizerState);

    // Now set the rasterizer state.
    remote.context->RSSetState(state.rasterizerState);

    state.viewport.TopLeftX = 0;
    state.viewport.TopLeftY = 0;
    state.viewport.Width = width;
    state.viewport.Height = height;
    state.viewport.MinDepth = 0.0f;
    state.viewport.MaxDepth = 1.0f;

    remote.context->RSSetViewports(1, &state.viewport);

    return true;
}

/*========================= CREATE METHODS =========================*/

bool Renderer::CreateBuffer(D3D11_BUFFER_DESC bufferDesc, D3D11_SUBRESOURCE_DATA data, ID3D11Buffer** buffer)
{
    if (FAILED(device->CreateBuffer(&bufferDesc, &data, buffer)))
    {
        printf("Failed Creating Buffer: %p of size %llu \n", (*buffer), sizeof(data.pSysMem));
        return false;
    }

    return true;
}

bool Renderer::CreateVertexBuffer(ID3D11VertexShader** vertexShader, ID3DBlob** VS)
{
    if (FAILED(device->CreateVertexShader((*VS)->GetBufferPointer(), (*VS)->GetBufferSize(), NULL, vertexShader)))
    {
        printf("Failed Creating Vertex Shader \n");
        return false;
    }

    return true;
}

bool Renderer::CreatePixelBuffer(ID3D11PixelShader** pixelShader, ID3DBlob** PS)
{
    if (FAILED(device->CreatePixelShader((*PS)->GetBufferPointer(), (*PS)->GetBufferSize(), NULL, pixelShader)))
    {
        printf("Failed Creating Pixel Shader \n");
        return false;
    }

    return true;
}

/*========================= CALLBACK METHODS =========================*/

void Renderer::ResizeBuffer(int width, int height)
{
    remote.context->OMSetRenderTargets(0, 0, 0);

    backbuffer->Release();
    depthBuffer->Release();

    HRESULT result = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    if (FAILED(result))
    {
        printf("%s", (std::string("Failing Resizing SwapChain Buffer : ") + std::system_category().message(result)).c_str());
    }

    CreateDrawBuffer(width,height);

    remote.context->OMSetRenderTargets(1, &backbuffer, depthBuffer);

    state.viewport.Width = width;
    state.viewport.Height = height;
    remote.context->RSSetViewports(1, &state.viewport);

}

/*========================= RENDER METHODS =========================*/

void Renderer::Clear()
{
    remote.context->ClearRenderTargetView(backbuffer, state.clearColor.e);
    remote.context->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    remote.context->RSSetState(state.rasterizerState);
    remote.context->OMSetDepthStencilState(state.depthStencilState, 1);
    remote.context->RSSetViewports(1, &state.viewport);
}

void Renderer::SetBackBuffer()
{
    remote.context->OMSetRenderTargets(1, &backbuffer, depthBuffer);
}

void Renderer::Render()
{
    // switch the back buffer and the front buffer
    remote.context->OMSetRenderTargets(1, &backbuffer, nullptr);
    swapchain->Present(1, 0);
}

void Renderer::SetFrameBuffer(const FrameBuffer& frameBuffer)
{
    remote.context->OMSetRenderTargets(1, frameBuffer.GetRenderTarget(), depthBuffer);
}

void Renderer::ClearFrameBuffer(const FrameBuffer& frameBuffer)
{
    remote.context->ClearRenderTargetView(*frameBuffer.GetRenderTarget(), state.clearColor.e);
}