#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Material {
    float3 ambient;
    float shininess;
    float3 diffuse;
    float padding1;
    float3 specular;
    float padding2;
};

struct Light {
    float3 pos;
    float strength;
    float3 dir;
    float fallOffStart;
    float fallOffEnd;
    float spotPower;
    int type;
    float padding;
};

cbuffer PerObject : register(b0) {
    matrix model;
    matrix viewProj;
    matrix modelInvTr;
    
    Material material;
}

cbuffer PerFrame : register(b1) {
    Light light;
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
float3 BlinnPhong(float3 normal, float3 lightVec, float3 lightStrength, float3 toEye) {
    float3 halfway = normalize(lightVec + toEye);
    float3 specular = material.specular * pow(max(dot(normal, halfway), 0.0f), material.shininess);
    
    return material.ambient + (material.diffuse + specular) * lightStrength;
}

float3 DirectionalLight(float3 normal, float3 toEye) {
    float3 lightVec = -light.dir;
    float3 lightStrength = light.strength * max(dot(lightVec, normal), 0.0f);
    return BlinnPhong(normal, lightVec, lightStrength, toEye);
}

float3 PointLight(float3 normal, float3 pos, float3 toEye) {
    float3 lightVec = light.pos - pos;
    
    float dist = length(lightVec);
    
    if (dist > light.fallOffEnd) {
        return float3(0.0f, 0.0f, 0.0f);
    } else {
        lightVec /= dist;
        
        float3 lightStrength = light.strength * max(dot(lightVec, normal), 0.0f);
        float attenuation = saturate((light.fallOffEnd - dist) / (light.fallOffEnd - light.fallOffStart));
        lightStrength *= attenuation;
        
        return BlinnPhong(normal, lightVec, lightStrength, toEye);
    }
}

float3 SpotLight(float3 normal, float3 pos, float3 toEye) {
    float3 lightVec = light.pos - pos;
    
    float dist = length(lightVec);
    
    if (dist > light.fallOffEnd) {
        return float3(0.0f, 0.0f, 0.0f);
    } else {
        lightVec /= dist;
        
        float3 lightStrength = light.strength * max(dot(lightVec, normal), 0.0f);
        
        float attenuation = saturate((light.fallOffEnd - dist) / (light.fallOffEnd - light.fallOffStart));
        lightStrength *= attenuation;
        
        float spotFactor = pow(max(dot(-lightVec, light.dir), 0.0f), light.spotPower);
        lightStrength *= spotFactor;
        
        return BlinnPhong(normal, lightVec, lightStrength, toEye);
    }
}

float4 main(PSInput input) : SV_TARGET {
    float3 toEye = normalize(eyePos - input.posWorld);
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    if (light.type == DIRECTIONAL) {
        color += DirectionalLight(input.normal, toEye);
    } else if (light.type == POINT) {
        color += PointLight(input.normal, input.posWorld, toEye);
    } else if (light.type == SPOT) {
        color += SpotLight(input.normal, input.posWorld, toEye);
    } else {
        color += float3(1.0f, 0.0f, 0.0f);
    }
    
    return float4(color, 1.0f);
}