#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = float4(tex.Sample(smp,input.uv));

	float3 light = normalize(float3(1,-1,1));            //右下奥向きライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;                   //アンビエント項を0.3として計算
	return float4(texColor.rgb * brightness, texColor.a) * color;
}