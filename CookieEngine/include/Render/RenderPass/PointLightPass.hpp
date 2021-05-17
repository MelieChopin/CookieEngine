#ifndef __POINT_LIGHT_PASS_HPP__
#define __POINT_LIGHT_PASS_HPP__

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11InputLayout;

namespace Cookie
{
	namespace Resources
	{
		class Mesh;
	}

	namespace Render
	{
		struct PointLight;
		class DrawDataHandler;

		class PointLightPass
		{
		private:
			ID3D11VertexShader* VShader{ nullptr };
			ID3D11PixelShader*	PShader{ nullptr };

			ID3D11InputLayout*	ILayout{ nullptr };
			ID3D11Buffer*		IBuffer	{ nullptr };

			std::unique_ptr<Resources::Mesh> sphereMesh{ nullptr };

		private:
			void InitShader();

		public:
			PointLightPass();
			~PointLightPass();

			void Set(ID3D11Buffer** lightCBuffer, const LightsArray& lights, const DrawDataHandler& drawData);
			void Draw(const unsigned int instanceNb);
		};
	}
}

#endif /*__POINT_LIGHT_PASS_HPP__*/