cbuffer PerObject : register(b0) {
    matrix mvp;
    matrix modelInvTr;
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
    
    output.pos = mul(float4(input.pos, 1.0f), mvp);
    output.normal = mul(float4(input.normal, 0.0f), modelInvTr);
    output.texcoord = input.texcoord;
    
    return output;
}