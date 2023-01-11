#include "OBJShaderHeader.hlsli"

SkinOutput CompureSkin(VSInput input)
{
    SkinOutput output;
    
    output.normal = float3(0, 0, 0);
    output.pos = float4(0, 0, 0, 0);
	
    uint iBone;
    float weight;
    matrix m;
	
    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    return output;
}

VSOutput main(VSInput input)
{
    SkinOutput skined = CompureSkin(input);
	
    float4 wn = normalize(mul(mat, float4(skined.normal, 0)));
	
	VSOutput output;//�s�N�Z���V�F�[�_�[�ɓn���l
    output.worldPos = input.svpos;
	output.svpos = mul(mat, skined.pos);
    output.normal = wn.xyz;
	output.uv = input.uv;
	return output;
}

