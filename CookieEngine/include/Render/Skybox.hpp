#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__


namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
		class Mesh;
		class Texture;
		class Shader;
	}

	namespace Core::Math
	{
		union Mat4;
	}

	namespace Render
	{
		class SkyBox
		{
			public:
				std::shared_ptr<Cookie::Resources::Mesh>			cube{ nullptr };
				std::shared_ptr<Cookie::Resources::Shader>	shader{ nullptr };
				std::shared_ptr<Cookie::Resources::Texture>			texture{ nullptr };

				/* CONSTRUCTORS/DESTRUCTORS */
				SkyBox(Resources::ResourcesManager& _resources);
				~SkyBox();

				void Draw(const Core::Math::Mat4& proj, const Core::Math::Mat4& view);

		};
	}
}


#endif // !__SKYBOX_HPP__