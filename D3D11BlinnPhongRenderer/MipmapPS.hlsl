#include "Common.hlsli"

cbuffer PerFrame : register(b1) {
    matrix viewProj;
    float3 eyePos;
    int useMipmap;
}

struct VSOutput {
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET {
    return g_texture.Sample(g_sampler, input.texcoord);
}