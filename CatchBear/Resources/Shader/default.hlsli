#ifndef _DEFAULT_HLSLI_
#define _DEFULAT_HLSLI_

#include "params.hlsli"
#include "utils.hlsli"


struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    // 당장 투영 좌표계로 넘어가는게 아니라
    // 빛 연산을 해주기 위해 view까지만 끊기 위해 matWV까지 끊어서 연산
    output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;
    output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = tex_0.Sample(g_sam_0, input.uv);
    return color;
}