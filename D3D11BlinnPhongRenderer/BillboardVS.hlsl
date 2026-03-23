struct VSInput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOutput {
    float4 pos : POSITION;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = float4(input.pos, 1.0f);
    return output;
}