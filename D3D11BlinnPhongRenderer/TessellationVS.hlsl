struct VSInput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOutput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.pos = input.pos;
    output.normal = input.normal;
    return output;
}