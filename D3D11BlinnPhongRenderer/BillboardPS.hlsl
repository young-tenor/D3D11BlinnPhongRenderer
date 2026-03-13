struct PSInput {
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET {
    return float4(input.texcoord, 0.0f, 1.0f);

}