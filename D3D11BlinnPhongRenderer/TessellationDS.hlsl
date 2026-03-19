#include "Common.hlsli"

cbuffer PerFrame : register(b1) {
    matrix viewProj;
    Light light;
    float3 eyePos;
    int tess;
}

struct HSOutput {
    float3 pos : POSITION;
};

struct HSConstOutput {
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct DSOutput {
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
};

[domain("quad")]
DSOutput main(HSConstOutput input, float2 uv : SV_DomainLocation, const OutputPatch<HSOutput, 4> patch) {
    DSOutput output;
    
    float3 v1 = lerp(patch[0].pos, patch[1].pos, uv.x);
    float3 v2 = lerp(patch[3].pos, patch[2].pos, uv.x);
    float3 pos = lerp(v1, v2, uv.y);
    
    float radius = length(patch[0].pos);
    float3 normal = normalize(pos);
    pos = normal * radius;
    
    output.posWorld = mul(float4(pos, 1.0f), model).xyz;
    output.pos = mul(float4(output.posWorld, 1.0f), viewProj);
    output.normal = normalize(mul(float4(normal, 0.0f), modelInvTr)).xyz;

    return output;
}