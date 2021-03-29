#include <DirectXTex/initguid.h>
#include <system_error>
#include <DirectXTex/DDSTextureLoader11.h>
#include <DirectXTex/WICTextureLoader11.h>

#include "Render/Renderer.hpp"
#include "Resources/Texture.hpp"

using namespace Cookie::Resources;

/*===================================== CONSTRUCTORS/DESTRUCTORS =====================================*/

Texture::Texture(const std::string& texPath) :
	name{texPath}
{
	std::wstring wString = std::wstring(texPath.begin(),texPath.end());

	if (name.find(".dss") != std::string::npos)
	{
		HRESULT result = DirectX::CreateDDSTextureFromFile(Render::RendererRemote::device, wString.c_str(), &texture, &shaderResourceView);
		if (FAILED(result))
		{
			printf("Failing Loading Texture %s: %s\n", name.c_str(), std::system_category().message(result).c_str());
		}
	}
	else
	{
		HRESULT result = DirectX::CreateWICTextureFromFile(Render::RendererRemote::device, wString.c_str(), &texture, &shaderResourceView);
		if (FAILED(result))
		{
			printf("Failing Loading Texture %s: %s\n", name.c_str(), std::system_category().message(result).c_str());
		}
	}
		
}

Texture::Texture(const std::string& texName, const Core::Math::Vec4& color):
	name {texName}
{
	if (CreateTextureFromColor(color))
	{
		CreateShaderResource();
	}
}

Texture::~Texture()
{
	if (texture)
		texture->Release();
	if (shaderResourceView)
		shaderResourceView->Release();
}

void Texture::Set()
{
	Render::RendererRemote::context->PSSetShaderResources(0, 1, &shaderResourceView);
}

/*==================== CREATE METHODS ========================*/

bool Texture::CreateTextureFromColor(const Core::Math::Vec4& color)
{
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = color.e;
	subResource.SysMemPitch = 1;
	subResource.SysMemSlicePitch = 0;

	HRESULT result = Render::RendererRemote::device->CreateTexture2D(&desc, &subResource, (ID3D11Texture2D**)&texture);
	if (FAILED(result))
	{
		printf("Failing Creating Texture: %s\n", std::system_category().message(result).c_str());
		return false;
	}
	
	return true;
}

bool Texture::CreateShaderResource()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format						= DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels			= 1;
	srvDesc.Texture2D.MostDetailedMip	= 0;

	HRESULT result = Render::RendererRemote::device->CreateShaderResourceView(texture, &srvDesc, &shaderResourceView);
	if (FAILED(result))
	{
		printf("Failing Creating FrameBuffer ShaderResource: %s\n", std::system_category().message(result).c_str());
		return false;
	}

	return true;
}