#include "Common.hlsli"

cbuffer PerFrame : register(b1) {
    matrix viewProj;
    Light light;
    float3 eyePos;
    int tess;
}

struct DSOutput {
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float3 DirectionalLight(float3 normal, float3 toEye) {
    float3 lightVec = -light.dir;
    float3 lightStrength = light.strength * max(dot(lightVec, normal), 0.0f);
    return BlinnPhong(normal, lightVec, lightStrength, toEye);
}

float4 main(DSOutput input) : SV_TARGET {
    float3 toEye = normalize(eyePos - input.posWorld);
    float3 color = material.ambient + DirectionalLight(input.normal, toEye);
    return float4(color, 1.0f);
}