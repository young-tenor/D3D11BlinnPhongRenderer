cbuffer PerFrame : register(b0) {
    int useTexture;
    //float3 padding;
}

struct PSInput {
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET {
    float4 color;
    
    if (useTexture) {
        color = g_texture.Sample(g_sampler, input.texcoord);
    } else {
        color = float4(input.normal, 1.0f);
    }
    
    return color;
}