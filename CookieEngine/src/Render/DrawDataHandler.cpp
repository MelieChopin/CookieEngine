//#include "D3D11Helper.hpp"
#include "Camera.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Resources/Mesh.hpp"
#include "DrawDataHandler.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

DrawDataHandler::DrawDataHandler()
{
}

DrawDataHandler::~DrawDataHandler()
{
}

/*========================= REALTIME METHODS =========================*/

void DrawDataHandler::MakeFrustrum(const Camera& cam)
{
	Mat4 viewProj = cam.GetViewProj();

	//left plane
	frustrum.planes[0].x = viewProj.c[0].e[3] + viewProj.c[0].e[0];
	frustrum.planes[0].y = viewProj.c[1].e[3] + viewProj.c[1].e[0];
	frustrum.planes[0].z = viewProj.c[2].e[3] + viewProj.c[2].e[0];
	frustrum.planes[0].w = viewProj.c[3].e[3] + viewProj.c[3].e[0];

	//right plane
	frustrum.planes[1].x = viewProj.c[0].e[3] - viewProj.c[0].e[0];
	frustrum.planes[1].y = viewProj.c[1].e[3] - viewProj.c[1].e[0];
	frustrum.planes[1].z = viewProj.c[2].e[3] - viewProj.c[2].e[0];
	frustrum.planes[1].w = viewProj.c[3].e[3] - viewProj.c[3].e[0];

	//top plane
	frustrum.planes[2].x = viewProj.c[0].e[3] - viewProj.c[0].e[1];
	frustrum.planes[2].y = viewProj.c[1].e[3] - viewProj.c[1].e[1];
	frustrum.planes[2].z = viewProj.c[2].e[3] - viewProj.c[2].e[1];
	frustrum.planes[2].w = viewProj.c[3].e[3] - viewProj.c[3].e[1];

	//bottom plane
	frustrum.planes[3].x = viewProj.c[0].e[3] + viewProj.c[0].e[1];
	frustrum.planes[3].y = viewProj.c[1].e[3] + viewProj.c[1].e[1];
	frustrum.planes[3].z = viewProj.c[2].e[3] + viewProj.c[2].e[1];
	frustrum.planes[3].w = viewProj.c[3].e[3] + viewProj.c[3].e[1];

	//near plane
	frustrum.planes[4].x = viewProj.c[0].e[2];
	frustrum.planes[4].y = viewProj.c[1].e[2];
	frustrum.planes[4].z = viewProj.c[2].e[2];
	frustrum.planes[4].w = viewProj.c[3].e[2];

	frustrum.planes[5].x = viewProj.c[0].e[3] + viewProj.c[0].e[2];
	frustrum.planes[5].y = viewProj.c[1].e[3] + viewProj.c[1].e[2];
	frustrum.planes[5].z = viewProj.c[2].e[3] + viewProj.c[2].e[2];
	frustrum.planes[5].w = viewProj.c[3].e[3] + viewProj.c[3].e[2];

	for (int i = 0; i < frustrum.planes.size(); i++)
	{
		frustrum.planes[i] = frustrum.planes[i].Normalize();
	}

	Mat4 invProj = Mat4::Inverse(viewProj);

	frustrum.centroid = {invProj.c[0].w,invProj.c[1].w ,invProj.c[2].w };
}

void DrawDataHandler::SetDrawData(const Camera& cam, const Game& game)
{
}

void DrawDataHandler::Draw()
{
}

void DrawDataHandler::Clear()
{
	models.clear();
	matrices.clear();
}