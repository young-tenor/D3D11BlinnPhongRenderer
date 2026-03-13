cbuffer PerObject : register(b0) {
    matrix viewProj;
};

struct GSInput {
    float4 pos : SV_POSITION;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

[maxvertexcount(4)]
void main(point GSInput input[1], inout TriangleStream<PSInput> outStream) {
    float size = 0.5f;
    
    float4 v[4];
    v[0] = input[0].pos + float4(-size, size, 0.0f, 0.0f);
    v[1] = input[0].pos + float4(size, size, 0.0f, 0.0f);
    v[2] = input[0].pos + float4(-size, -size, 0.0f, 0.0f);
    v[3] = input[0].pos + float4(size, -size, 0.0f, 0.0f);
    
    float2 texcoord[4];
    texcoord[0] = float2(0.0f, 0.0f);
    texcoord[1] = float2(1.0f, 0.0f);
    texcoord[2] = float2(0.0f, 1.0f);
    texcoord[3] = float2(1.0f, 1.0f);

    PSInput output;
    for (int i = 0; i < 4; i++) {
        output.pos = mul(v[i], viewProj);
        output.texcoord = texcoord[i];
        outStream.Append(output);
    }
}