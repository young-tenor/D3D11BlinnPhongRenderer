#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__

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
    matrix modelInvTr;
    
    Material material;
}

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
float3 BlinnPhong(float3 normal, float3 lightVec, float3 lightStrength, float3 toEye) {
    float3 halfway = normalize(lightVec + toEye);
    float3 specular = material.specular * pow(max(dot(normal, halfway), 0.0f), material.shininess);
    
    return (material.diffuse + specular) * lightStrength;
}

#endif