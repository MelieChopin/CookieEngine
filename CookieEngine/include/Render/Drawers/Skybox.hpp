#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
		class Mesh;
		class Texture;
	}

	namespace Core::Math
	{
		union Mat4;
	}

	namespace Render
	{
		class SkyBox
		{
			private:
				ID3D11Buffer*		CBuffer{ nullptr };
				ID3D11InputLayout*	ILayout{ nullptr };
				ID3D11VertexShader* VShader{ nullptr };
				ID3D11PixelShader*  PShader{ nullptr };
				ID3D11SamplerState* PSampler{ nullptr };


			public:
				Cookie::Resources::Mesh*	cube			{ nullptr };
				Cookie::Resources::Texture*	texture			{ nullptr };

				ID3D11RasterizerState*		rasterizerState	{ nullptr };


			private:
				void InitShader();
				void InitRasterizer();

			public:

				/* CONSTRUCTORS/DESTRUCTORS */
				SkyBox();
				SkyBox(Resources::ResourcesManager& _resources);
				~SkyBox();

				void Draw(const Core::Math::Mat4& proj, const Core::Math::Mat4& view);

		};
	}
}


#endif // !__SKYBOX_HPP__