#include "D3D11Helper.hpp"
#include "Camera.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Game.hpp"
#include "Scene.hpp"
#include "Resources/Mesh.hpp"
#include "DrawDataHandler.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::ECS;
using namespace Cookie::Render;

constexpr float cullEpsilon = -3.0f;

struct VS_CONSTANT_BUFFER
{
	Mat4 proj = Mat4::Identity();
	Mat4 view = Mat4::Identity();
};

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

DrawDataHandler::DrawDataHandler()
{
}

DrawDataHandler::~DrawDataHandler()
{
}

/*========================= INIT METHODS =========================*/

void DrawDataHandler::Init(const Game& game)
{
	coordinator			= &game.coordinator;
	player				= &game.playerData;
}

void DrawDataHandler::SetScene(Resources::Scene& scene)
{
	mapDrawer.Set(scene.map);
	lights = &scene.lights;
}

/*========================= DRAW DATA METHODS =========================*/

bool DrawData::operator==(const ECS::ComponentModel& model)
{
	return (model.mesh == mesh 
			&& model.albedo == albedo 
			&& model.normal == normalMap 
			&& model.metallicRoughness == matMap);
}

/*========================= FRUSTRUM METHODS =========================*/

void Frustrum::MakeFrustrum(const Camera& cam)
{
	Mat4 viewProj = cam.GetViewProj();

	//left plane
	planes[0].x = viewProj.c[3].e[0] + viewProj.c[0].e[0];
	planes[0].y = viewProj.c[3].e[1] + viewProj.c[0].e[1];
	planes[0].z = viewProj.c[3].e[2] + viewProj.c[0].e[2];
	planes[0].w = viewProj.c[3].e[3] + viewProj.c[0].e[3];

	//right plane
	planes[1].x = viewProj.c[3].e[0] - viewProj.c[0].e[0];
	planes[1].y = viewProj.c[3].e[1] - viewProj.c[0].e[1];
	planes[1].z = viewProj.c[3].e[2] - viewProj.c[0].e[2];
	planes[1].w = viewProj.c[3].e[3] - viewProj.c[0].e[3];

	//top plane
	planes[2].x = viewProj.c[3].e[0] - viewProj.c[1].e[0];
	planes[2].y = viewProj.c[3].e[1] - viewProj.c[1].e[1];
	planes[2].z = viewProj.c[3].e[2] - viewProj.c[1].e[2];
	planes[2].w = viewProj.c[3].e[3] - viewProj.c[1].e[3];

	//bottom plane
	planes[3].x = viewProj.c[3].e[0] + viewProj.c[1].e[0];
	planes[3].y = viewProj.c[3].e[1] + viewProj.c[1].e[1];
	planes[3].z = viewProj.c[3].e[2] + viewProj.c[1].e[2];
	planes[3].w = viewProj.c[3].e[3] + viewProj.c[1].e[3];

	//near plane
	planes[4].x = viewProj.c[2].e[0];
	planes[4].y = viewProj.c[2].e[1];
	planes[4].z = viewProj.c[2].e[2];
	planes[4].w = viewProj.c[2].e[3];

	//far plane
	planes[5].x = viewProj.c[3].e[0] - viewProj.c[2].e[0];
	planes[5].y = viewProj.c[3].e[1] - viewProj.c[2].e[1];
	planes[5].z = viewProj.c[3].e[2] - viewProj.c[2].e[2];
	planes[5].w = viewProj.c[3].e[3] - viewProj.c[2].e[3];

	for (int i = 0; i < planes.size(); i++)
	{
		planes[i] = planes[i].Normalize();
	}

	//the vectors of the referential of cam
	Core::Math::Vec3 camFwd		= { viewProj.c[2].x,viewProj.c[2].y,viewProj.c[2].z };
	Core::Math::Vec3 camRight	= { viewProj.c[0].x,viewProj.c[0].y,viewProj.c[0].z };
	Core::Math::Vec3 camUp		= { viewProj.c[1].x,viewProj.c[1].y,viewProj.c[1].z };
	camFwd		= camFwd.Normalize();
	camRight	= camRight.Normalize();
	camUp		= camUp.Normalize();

	//height and width of far plane
	float heightFar = 2.0f * tanf(Core::Math::ToRadians(cam.fov) / 2.0f)*cam.camFar;
	float widthFar = heightFar * cam.aspectRatio;

	//height and width of near plane
	float heightNear = 2.0f * tanf(Core::Math::ToRadians(cam.fov) / 2.0f) * cam.camNear;
	float widthNear = heightNear * cam.aspectRatio;

	//Center of each plane
	Core::Math::Vec3 nearCenter = cam.pos + camFwd * cam.camNear;
	Core::Math::Vec3 farCenter	= cam.pos + camFwd * cam.camFar;
	centroid = cam.pos + camFwd * ((cam.camNear + cam.camFar) / 2.0f);

	//the corners of the frustrum
	corners[0] = nearCenter - (camUp * (heightNear / 2.0f)) - (camRight * (widthNear / 2.0f));
	corners[1] = nearCenter - (camUp * (heightNear / 2.0f)) + (camRight * (widthNear / 2.0f));
	corners[2] = nearCenter + (camUp * (heightNear / 2.0f)) - (camRight * (widthNear / 2.0f));
	corners[3] = nearCenter + (camUp * (heightNear / 2.0f)) + (camRight * (widthNear / 2.0f));
	corners[4] = farCenter - (camUp * (heightFar / 2.0f)) - (camRight * (widthFar / 2.0f));
	corners[5] = farCenter - (camUp * (heightFar / 2.0f)) + (camRight * (widthFar / 2.0f));
	corners[6] = farCenter + (camUp * (heightFar / 2.0f)) - (camRight * (widthFar / 2.0f));
	corners[7] = farCenter + (camUp * (heightFar / 2.0f)) + (camRight * (widthFar / 2.0f));
}

/*========================= REALTIME METHODS =========================*/

void DrawDataHandler::SetDrawData(const Camera* cam)
{
	currentCam			= cam;
	frustrum.MakeFrustrum(*cam);

	const Coordinator&			coord			= *coordinator;
	const ECS::EntityHandler&	entityHandler	= *coord.entityHandler;
	ECS::ComponentHandler&		components		= *coord.componentHandler;

	bool cull = false;

	for (int i = 0; i < entityHandler.livingEntities; ++i)
	{
		const Entity& iEntity = entityHandler.entities[i];
		if ((iEntity.signature & (C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL)) == (C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL))
		{
			ECS::ComponentModel& model = components.GetComponentModel(iEntity.id);

			if (model.mesh == nullptr)
			{
				continue;
			}

			Core::Math::Mat4& trs = components.GetComponentTransform(iEntity.id).TRS;

			Vec4 modelMin = trs * Core::Math::Vec4(model.mesh->AABBMin, 1.0f);
			Vec4 modelMax = trs * Core::Math::Vec4(model.mesh->AABBMax, 1.0f);

			for (int j = 0; j < frustrum.planes.size(); j++)
			{

				if ((frustrum.planes[j].Dot(modelMin) + frustrum.planes[j].w) < cullEpsilon && (frustrum.planes[j].Dot(modelMax) + frustrum.planes[j].w) < cullEpsilon)
				{
					cull = true;
					break;
				}

			}

			if (!cull)
			{
				AABB[0].x = std::min(modelMin.x, AABB[0].x);
				AABB[0].y = std::min(modelMin.y, AABB[0].y);
				AABB[0].z = std::min(modelMin.z, AABB[0].z);

				AABB[1].x = std::max(modelMax.x, AABB[1].x);
				AABB[1].y = std::max(modelMax.y, AABB[1].y);
				AABB[1].z = std::max(modelMax.z, AABB[1].z);
			}

			ECS::ComponentGameplay& iGameplay = components.GetComponentGameplay(iEntity.id);

			if (iGameplay.signatureGameplay & (CGP_SIGNATURE::PRODUCER | CGP_SIGNATURE::WORKER))
			{
				PushDrawData(dynamicDrawData, model, trs, cull);
			}
			else
			{
				PushDrawData(staticDrawData, model, trs, cull);
			}

			cull = false;
		}
	}

	for (int i = 0; i < coord.selectedEntities.size(); ++i)
	{
		Entity& iEntity = *coord.selectedEntities[i];
		if ((iEntity.signature & (C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL)) == (C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL))
		{
			ECS::ComponentModel& model = components.GetComponentModel(iEntity.id);

			if (model.mesh == nullptr)
			{
				continue;
			}

			selectedModels.push_back(model);
			selectedMatrices.push_back(components.GetComponentTransform(iEntity.id).TRS);
			selectedGameplays.push_back(components.GetComponentGameplay(iEntity.id));
		}
	}
}

void DrawDataHandler::PushDrawData(std::vector<DrawData>& drawDatas, const ECS::ComponentModel& model, const Core::Math::Mat4& trs, bool culled)
{
	for (int i = 0; i < drawDatas.size(); i++)
	{
		DrawData& draw = drawDatas[i];
		if (draw == model)
		{
			draw.matrices.push_back(trs);
			if (!culled)
				draw.visibleMatrices.push_back(trs);

			return;
		}
	}

	drawDatas.push_back({ model.mesh,model.albedo,model.normal,model.metallicRoughness });

	DrawData& draw = drawDatas[drawDatas.size() - 1];
	draw.matrices.push_back(trs);

	if (!culled)
		draw.visibleMatrices.push_back(trs);
}

void DrawDataHandler::Draw(bool drawOccluded)
{
	modelDrawer.Draw(staticDrawData,drawOccluded);
	modelDrawer.Draw(dynamicDrawData,drawOccluded);
}

void DrawDataHandler::Clear()
{
	staticDrawData.clear();
	dynamicDrawData.clear();
	selectedModels.clear();
	selectedMatrices.clear();
	selectedGameplays.clear();
	AABB[0] = { std::numeric_limits<float>().max(),std::numeric_limits<float>().max() ,std::numeric_limits<float>().max() };
	AABB[1] = { -std::numeric_limits<float>().max(), -std::numeric_limits<float>().max() , -std::numeric_limits<float>().max() };
}