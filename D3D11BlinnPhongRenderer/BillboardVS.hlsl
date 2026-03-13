struct VSInput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};


struct GSInput {
    float4 pos : SV_POSITION;
};

GSInput main(VSInput input) {
    GSInput output;
    output.pos = float4(input.pos, 1.0f);
    return output;
}