cbuffer PerObject : register(b0) {
    matrix viewProj;
};

cbuffer PerFrame : register(b1) {
    float3 eyePos;
    float padding;
}

struct GSInput {
    float4 pos : POSITION;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

[maxvertexcount(4)]
void main(point GSInput input[1], inout TriangleStream<PSInput> outStream) {
    float size = 0.5f;
    
    float4 look = float4(eyePos, 1.0f) - input[0].pos;
    look.y = 0.0f;
    look = normalize(look);
    float4 up = float4(0.0f, 1.0f, 0.0f, 0.0f);
    float4 right = float4(cross(up.xyz, look.xyz), 0.0f);
    
    float4 v[4];
    v[0] = input[0].pos + size * right + size * up;
    v[1] = input[0].pos - size * right + size * up;
    v[2] = input[0].pos + size * right - size * up;
    v[3] = input[0].pos - size * right - size * up;
    
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