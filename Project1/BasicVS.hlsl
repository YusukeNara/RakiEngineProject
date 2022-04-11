#include "Basic.hlsli"

VSOutput main( float4 pos : POSITION,float3 normal : NORMAL,float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(mat, pos);//À•W‚És—ñ‚ğŒvZ
	output.normal = normal;
	output.uv = uv;
	return output;
}