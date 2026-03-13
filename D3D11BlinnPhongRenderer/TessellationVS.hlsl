struct Material {
    float3 ambient;
    float shininess;
    float3 diffuse;
    float padding1;
    float3 specular;
    float padding2;
};

cbuffer PerObject : register(b0) {
    matrix viewProj;
    Material material;
}

struct VSInput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

PSInput main(VSInput input) {
    PSInput output;
    
    output.posWorld = input.pos;
    output.pos = mul(float4(output.posWorld, 1.0f), viewProj);
    output.normal = float4(input.normal, 0.0f);
    output.texcoord = input.texcoord;
    
    return output;
}