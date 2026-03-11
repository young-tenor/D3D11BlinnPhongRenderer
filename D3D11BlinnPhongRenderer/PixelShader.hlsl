struct PSInput {
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET {
    return float4(input.normal, 1.0f);
    return g_texture.Sample(g_sampler, input.uv);
}