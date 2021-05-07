#ifndef __DRAW_DATA_HANDLER_HPP__
#define __DRAW_DATA_HANDLER_HPP__

#include <Mat4.hpp>
#include <array>
#include <vector>

namespace Cookie
{
	class Game;

	namespace ECS
	{
		class ComponentModel;
	}

	namespace Render
	{
		class Camera cam;

		struct Frustrum
		{
			std::array<Core::Math::Vec4, 6> planes;
			Core::Math::Vec3				centroid;
			std::array<Core::Math::Vec3, 2> AABB;
		};
		
		class DrawDataHandler
		{
			public:
				Frustrum frustrum;
				std::vector<ECS::ComponentModel>	models;
				std::vector<Core::Math::Mat4>		matrices;

			private:
				void MakeFrustrum(const Camera& cam);

			public:
				DrawDataHandler();
				~DrawDataHandler();

				void SetDrawData(const Camera& cam, const Game& game);
				void Draw();
				void Clear();
		};
	}
}

#endif // !__DRAW_DATA_HANDLER_HPP__
