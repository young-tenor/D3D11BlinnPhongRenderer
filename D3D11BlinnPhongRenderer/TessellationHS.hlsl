#include "Common.hlsli"

cbuffer PerFrame : register(b1) {
    matrix viewProj;
    Light light;
    float3 eyePos;
    int tess;
}

struct VSOutput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct HSOutput {
    float3 pos : POSITION;
};

struct HSConstOutput {
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

HSConstOutput PatchConstFunc(InputPatch<VSOutput, 4> patch, uint patchId : SV_PrimitiveID)
{
    HSConstOutput output;

    float3 n0 = normalize(patch[0].pos);
    float3 n1 = normalize(patch[1].pos);
    float3 n2 = normalize(patch[2].pos);
    float3 n3 = normalize(patch[3].pos);

    float3 v0 = normalize(eyePos - patch[0].pos);
    float3 v1 = normalize(eyePos - patch[1].pos);
    float3 v2 = normalize(eyePos - patch[2].pos);
    float3 v3 = normalize(eyePos - patch[3].pos);
    
    float dot0 = dot(n0, v0);
    float dot1 = dot(n1, v1);
    float dot2 = dot(n2, v2);
    float dot3 = dot(n3, v3);
    
    float maxDot = max(max(dot0, dot1), max(dot2, dot3));
    
    if (maxDot < -0.1f) {
        output.edges[0] = output.edges[1] = output.edges[2] = output.edges[3] = 0.0f;
        output.inside[0] = output.inside[1] = 0.0f;
    } else {
        output.edges[0] = output.edges[1] = output.edges[2] = output.edges[3] = tess;
        output.inside[0] = output.inside[1] = tess;
    }

    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstFunc")]
HSOutput main(InputPatch<VSOutput, 4> patch, uint i : SV_OutputControlPointID)
{
    HSOutput output;
    output.pos = patch[i].pos;
    return output;
}