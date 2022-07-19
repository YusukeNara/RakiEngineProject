#include "OBJShaderHeader.hlsli"

//	2パス目出力用シェーダー（予定）
//	現時点では、GBuffer画像を出力するだけ

//ディファードでは、このレジスタにGBufferが入る
Texture2D<float4> tex : register(t0);
//サンプラーは変更なし
SamplerState smp : register(s0);
//光源情報の配列が入るレジスタを追加予定

float4 main(GSOutput input) : SV_TARGET
{
    //現時点では、ただレンダリング結果をだすだけ
    //このシェーダーはLit相当なので、後に光源計算が入る
    
    
    float4 texColor = float4(tex.Sample(smp, input.uv));

    float3 light = normalize(float3(1, -1, 1)); //右下奥向きライト
    float diffuse = saturate(dot(-light, input.normal));
	
    float3 shade_color;
    shade_color = m_ambient;
    shade_color += m_diffuse * diffuse;
	
    float4 texcolor = tex.Sample(smp, input.uv);
	
    //通常の結果を出力
    return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha) * color;
}