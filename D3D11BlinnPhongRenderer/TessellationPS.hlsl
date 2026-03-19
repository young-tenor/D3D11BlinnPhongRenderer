#include "Common.hlsli"

struct PSInput {
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
float3 BlinnPhong(float3 normal, float3 lightVec, float3 lightStrength, float3 toEye) {
    float3 halfway = normalize(lightVec + toEye);
    float3 specular = material.specular * pow(max(dot(normal, halfway), 0.0f), material.shininess);
    
    return (material.diffuse + specular) * lightStrength;
}

float3 DirectionalLight(float3 normal, float3 toEye) {
    float3 lightVec = -light.dir;
    float3 lightStrength = light.strength * max(dot(lightVec, normal), 0.0f);
    return BlinnPhong(normal, lightVec, lightStrength, toEye);
}

float4 main(PSInput input) : SV_TARGET {
    float3 toEye = normalize(eyePos - input.posWorld);
    float3 color = material.ambient + DirectionalLight(input.normal, toEye);
    return float4(color, 1.0f);
}