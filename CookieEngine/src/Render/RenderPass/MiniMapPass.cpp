#include "Core/Math/Mat4.hpp"
#include "D3D11Helper.hpp"
#include "DrawDataHandler.hpp"
#include "ECS/ComponentGameplay.hpp"
#include "Render/RenderPass/MiniMapPass.hpp"
#include "Render/Camera.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

constexpr float zEpsilon = 50.0f;

struct CAM_CONSTANT_BUFFER
{
	Mat4 proj = Mat4::Identity();
	Mat4 view = Mat4::Identity();
};

struct VS_CONSTANT_BUFFER
{
	Vec4 color1 = { PLAYER_ARMY_COLOR,(float)Cookie::Gameplay::E_ARMY_NAME::E_PLAYER };
	Vec4 color2 = { AI1_ARMY_COLOR,(float)Cookie::Gameplay::E_ARMY_NAME::E_AI1 };
};


/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

MiniMapPass::MiniMapPass()
{
	InitShader();
	miniMapView = Mat4::Translate({0.0f,-1.0f,0.0f}) * Mat4::RotateX(ToRadians(90.0f));
}

MiniMapPass::~MiniMapPass()
{
	if (VShader)
		VShader->Release();
	if (PShader)
		PShader->Release();
	if (CBuffer)
		CBuffer->Release();
	if (CamCBuffer)
		CamCBuffer->Release();
	if (ILayout)
		ILayout->Release();
}

/*=========================== INIT METHODS ===========================*/

void MiniMapPass::InitShader()
{
	ID3DBlob* blob = nullptr;

	std::string source = (const char*)R"(#line 27
	struct VOut
	{
		float4 position : SV_POSITION;
		float3 color : COLOR;
	};

	cbuffer COLOR_BUFFER : register(b0)
	{
		float4    firstColor;
		float4    secondColor;
	};

	cbuffer CAM_CONSTANT : register(b1)
	{
		float4x4  proj;
		float4x4  view;
	};

	
	VOut main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL, float4x4 model : WORLD, float colorId : COLORID)
	{
		VOut output;
	
		output.position = mul(mul(mul(float4(position,1.0),model),view), proj);

		if (colorId == firstColor.a)
		{
			output.color = firstColor.rgb;
		}
		else if (colorId == secondColor.a)
		{
			output.color = secondColor.rgb;
		}

		return output;
	}
	)";

	Render::CompileVertex(source, &blob, &VShader);

	source = (const char*)R"(#line 93

	float4 main(float4 position : SV_POSITION, float3 color : COLOR) : SV_TARGET
	{
		return float4(color,1.0);
	})";

	Render::CompilePixel(source, &PShader);

	struct Vertex
	{
		Core::Math::Vec3 position;
		Core::Math::Vec2 uv;
		Core::Math::Vec3 normal;
	};

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,     offsetof(Vertex,position),  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",       0, DXGI_FORMAT_R32G32_FLOAT,    0,     offsetof(Vertex, uv),       D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,     offsetof(Vertex, normal),   D3D11_INPUT_PER_VERTEX_DATA, 0},

		{ "WORLD",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, offsetof(Mat4, c[0]), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",		1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, offsetof(Mat4, c[1]), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",		2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, offsetof(Mat4, c[2]), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",		3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, offsetof(Mat4, c[3]), D3D11_INPUT_PER_INSTANCE_DATA, 1 },


		{ "COLORID",	0, DXGI_FORMAT_R32_FLOAT,			2, offsetof(Mat4, c[4]), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	Render::CreateLayout(&blob, ied, 7, &ILayout);

	VS_CONSTANT_BUFFER buffer = {};

	Render::CreateBuffer(&buffer, sizeof(VS_CONSTANT_BUFFER), &CBuffer);

	CAM_CONSTANT_BUFFER camBuffer = {};

	Render::CreateBuffer(&camBuffer, sizeof(CAM_CONSTANT_BUFFER), &CamCBuffer);

	blob->Release();
}

/*=========================== REALTIME METHODS ===========================*/

void MiniMapPass::Set()
{
	Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
	Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);

	Render::RendererRemote::context->IASetInputLayout(ILayout);
}

void MiniMapPass::Draw(DrawDataHandler& drawData)
{
	float width = drawData.mapDrawer.mapInfo.model.c[0].Length();
	float height = drawData.mapDrawer.mapInfo.model.c[2].Length();

	ortho = Mat4::Ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, -zEpsilon, zEpsilon);

	viewport.Width = width;
	viewport.Height = height;

	//Render::RendererRemote::context->RSSetViewports(1,&viewport);

	ID3D11Buffer* CBuffers[2] = { CBuffer, CamCBuffer };
	Render::RendererRemote::context->VSSetConstantBuffers(0, 2, CBuffers);

	CAM_CONSTANT_BUFFER buffer = { ortho, miniMapView};

	Render::WriteBuffer(&buffer, sizeof(buffer), 0, &CamCBuffer);

	miniMapDrawer.Draw(drawData.staticDrawData);

	drawData.mapDrawer.Draw();
}