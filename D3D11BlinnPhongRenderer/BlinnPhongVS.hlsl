#include "Common.hlsli"

cbuffer PerFrame : register(b1) {
    matrix viewProj;
    Light light;
    float3 eyePos;
    float padding;
}

struct VSInput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOutput {
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.posWorld = mul(float4(input.pos, 1.0f), model);
    output.pos = mul(float4(output.posWorld, 1.0f), viewProj);
    output.normal = normalize(mul(float4(input.normal, 0.0f), modelInvTr));
    output.texcoord = input.texcoord;
    
    return output;
}