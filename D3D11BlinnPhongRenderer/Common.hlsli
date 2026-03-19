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

cbuffer PerFrame : register(b1) {
    matrix viewProj;
    Light light;
    float3 eyePos;
    int useTexture;
}

#endif