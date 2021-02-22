#include <DirectXTex/initguid.h>
#include <system_error>
#include <DirectXTex/DDSTextureLoader11.h>
#include <DirectXTex/WICTextureLoader11.h>

#include "Render/Renderer.hpp"
#include "Resources/Texture.hpp"

using namespace Cookie::Resources;

/*===================================== CONSTRUCTORS/DESTRUCTORS =====================================*/

Texture::Texture(Render::Renderer& renderer, const std::string& texPath) :
	name{texPath}
{
	std::wstring wString = std::wstring(texPath.begin(),texPath.end());

	if (name.find(".dss") != std::string::npos)
	{
		HRESULT result = DirectX::CreateDDSTextureFromFile(renderer.GetDevice(), wString.c_str(), &texture, &shaderResourceView);
		if (FAILED(result))
		{
			printf("Failing Loading Texture %s: %s\n", name.c_str(), std::system_category().message(result).c_str());
		}
	}
	else
	{
		HRESULT result = DirectX::CreateWICTextureFromFile(renderer.GetDevice(), wString.c_str(), &texture, &shaderResourceView);
		if (FAILED(result))
		{
			printf("Failing Loading Texture %s: %s\n", name.c_str(), std::system_category().message(result).c_str());
		}
	}
		
}

Texture::~Texture()
{
	if (texture)
		texture->Release();
	if (shaderResourceView)
		shaderResourceView->Release();
}

void Texture::Set(Render::RendererRemote& remote)
{
	remote.context->PSSetShaderResources(0, 1, &shaderResourceView);
}