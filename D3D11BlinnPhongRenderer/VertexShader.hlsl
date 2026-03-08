cbuffer Constants : register(b0) {
    matrix model;
    matrix view;
    matrix proj;
}

struct VSInput {
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

PSInput main(VSInput input) {
    PSInput output;
    
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.pos = pos;
    output.uv = input.uv;
    
    return output;
}