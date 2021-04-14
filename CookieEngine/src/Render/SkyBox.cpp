#include <d3d11.h>

#include "Render/RendererRemote.hpp"

#include "Resources/ResourcesManager.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Shader/SkyBoxShader.hpp"

#include "Skybox.hpp"

using namespace Cookie::Render;

/*==================== CONSTRUCTORS/DESTRUCTORS ====================*/

SkyBox::SkyBox(Resources::ResourcesManager& _resources):
	cube {_resources.meshes["Cube"]}, shader {_resources.shaders["SkyBox_Shader"]}
{

}

SkyBox::~SkyBox()
{

}

/*==================== REALTIME METHODS ====================*/

void SkyBox::Draw(const Core::Math::Mat4& proj, const Core::Math::Mat4& view)
{
	if (shader)
		shader->Set(proj, view);
	if (texture)
		texture->Set();
	if (cube)
	{
		cube->Set();
		cube->Draw();
	}
}