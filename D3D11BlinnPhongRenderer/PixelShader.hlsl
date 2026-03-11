struct Material {
    float3 ambient;
    float shininess;
    float3 diffuse;
    float padding1;
    float3 specular;
    float padding2;
};

cbuffer PerObject : register(b0) {
    matrix model;
    matrix viewProj;
    matrix modelInvTr;
    
    Material material;
}

cbuffer PerFrame : register(b1) {
    float3 lightPos;
    float lightStrength;
    float3 eyePos;
    int useTexture;
}

struct PSInput {
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
float4 main(PSInput input) : SV_TARGET {
    float3 N = normalize(input.normal);
    float3 L = normalize(lightPos - input.posWorld);
    float3 V = normalize(eyePos - input.posWorld);
    float3 H = normalize(L + V);
    
    float3 diffuse;
    if (useTexture) {
        diffuse = g_texture.Sample(g_sampler, input.texcoord).rgb;
    } else {
        diffuse = material.diffuse;
    }
    
    float3 specular = material.specular * pow(max(dot(N, H), 0.0f), material.shininess);
    
    return float4(material.ambient + (diffuse + specular) * lightStrength * max(dot(L, N), 0.0f), 1.0f);
}