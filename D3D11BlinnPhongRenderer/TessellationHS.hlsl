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

HSConstOutput PatchConstFunc(InputPatch<VSOutput, 4> patch, uint patchId : SV_PrimitiveID) {
    float tess = 16.0f;
    
    HSConstOutput output;
    output.edges[0] = output.edges[1] = output.edges[2] = output.edges[3] = tess;
    output.inside[0] = output.inside[1] = tess;
    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstFunc")]
HSOutput main(InputPatch<VSOutput, 4> patch, uint i : SV_OutputControlPointID) {
    HSOutput output;
    output.pos = patch[i].pos;
    return output;
}