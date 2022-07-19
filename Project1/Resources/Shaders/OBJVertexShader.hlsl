#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//ピクセルシェーダーに渡す値
    float def = 1;
    matrix d_mat = mat;
    d_mat._m33 = def;
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	return output;
}