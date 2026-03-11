cbuffer Constants : register(b0) {
    matrix model;
    matrix view;
    matrix proj;
}

struct VSInput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

PSInput main(VSInput input) {
    PSInput output;
    
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.pos = pos;
    output.normal = input.normal;
    output.texcoord = input.texcoord;
    
    return output;
}