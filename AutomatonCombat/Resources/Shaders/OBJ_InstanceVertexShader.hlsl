#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, matrix insmat : INSTANCE_WORLD_MAT)
{
    VSOutput output;
	
    output.svpos = mul(insmat, pos);
    output.normal = normal;
    output.uv = uv;
    output.worldPos = pos;
	
    return output;
}