#include "D3D11Helper.hpp"
#include "Vec3.hpp"
#include "Light.hpp"
#include "RenderPass/DirLightPass.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

struct PS_DIRLIGHT_BUFFER
{
    Vec3 dir;
    float padding = 0.0f;
    Vec3 color = { 1.0f,1.0f,1.0f };
    float padding2 = 0.0f;
};

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

DirLightPass::DirLightPass()
{
	InitShader();
}

DirLightPass::~DirLightPass()
{
    if (VShader)
        VShader->Release();
    if (PShader)
        PShader->Release();
    if (CBuffer)
        CBuffer->Release();
}

/*======================= INIT METHODS =======================*/

void DirLightPass::InitShader()
{
    ID3DBlob* blob = nullptr;

    std::string source = (const char*)R"(#line 30
    struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV;
    };
    
    VOut main(uint vI :SV_VertexId )
    {
        VOut output;
    
        float2 uv = float2((vI << 1) & 2, vI & 2);
        output.uv = float2(1-uv.x,uv.y);
        output.position = float4(-uv.x * 2 + 1, -uv.y * 2 + 1, 0, 1);
    
        return output;

    }
    )";

    CompileVertex(source, &blob, &VShader);

    source = std::string(PBR) + std::string((const char*)R"(

    Texture2D	positionTex : register(t0);
    Texture2D	normalTex   : register(t1);
    Texture2D   albedoTex   : register(t2);

    cbuffer DirLight : register(b0)
    {
        float3 lightDir;
        float3 lightColor;
    };

    SamplerState ClampSampler : register(s0);

    POut main(float4 position : SV_POSITION, float2 uv : UV)
    {
        POut output;
            
        float3  fragPos     = positionTex.Sample(ClampSampler,uv).xyz;
        float3  normal      = normalize(normalTex.Sample(ClampSampler,uv).xyz);
        float3  albedo      = pow(albedoTex.Sample(ClampSampler,uv).xyz,2.2);
        float   metallic    = positionTex.Sample(ClampSampler,uv).w;
        float   roughness   = normalTex.Sample(ClampSampler,uv).w;
        float   ao          = albedoTex.Sample(ClampSampler,uv).w;

        output          = compute_lighting(normal,fragPos,normalize(-lightDir),lightColor,albedo,metallic,roughness);
		output.diffuse += (0.03 * ao + 0.01) *  float4(albedo,1.0);
        output.diffuse  = pow(output.diffuse,0.45454545);
        output.specular = pow(output.specular,0.45454545);

        return output;
    })");

    CompilePixel(source, &PShader);

    PS_DIRLIGHT_BUFFER buffer = {};

    CreateBuffer(&buffer, sizeof(PS_DIRLIGHT_BUFFER), &CBuffer);
}

/*======================= REALTIME METHODS =======================*/

void DirLightPass::Set(ID3D11Buffer** lightCBuffer)const 
{

    RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    RendererRemote::context->PSSetShader(PShader, nullptr, 0);

    ID3D11Buffer* buffer[] = { CBuffer, *lightCBuffer };

    Render::RendererRemote::context->PSSetConstantBuffers(0, 2, buffer);
}

void DirLightPass::Write(const DirLight& dirLight)
{

    PS_DIRLIGHT_BUFFER buffer = { dirLight.dir,0.0f, dirLight.color,0.0f };
    Render::WriteCBuffer(&buffer, sizeof(PS_DIRLIGHT_BUFFER), 0, &CBuffer);
}