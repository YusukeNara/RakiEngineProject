#include "LIT_OBJHeader.hlsli"

//	2パス目出力用シェーダー（予定）
//	現時点では、GBuffer画像を出力するだけ

//アルベドテクスチャ
Texture2D<float4> albedoTex : register(t0);
//法線テクスチャ
Texture2D<float4> normalTex : register(t1);
//サンプラーは変更なし
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    //アルベド情報を取得
    float4 albedo = albedoTex.Sample(smp, input.uv);
    //法線情報取得
    float3 normal = normalTex.Sample(smp, input.uv).xyz;
    
    normal = (normal * 2.0f) - 1.0f;
    
    //ライト計算
    float3 lightDir = normalize(float3(1, -1, 1)); //右下奥向きライト
    float diffuse = saturate(dot(normal,-lightDir)); //ディフューズ計算
    
    //視点座標
    float3 eye = eyePos.xyz;
    
    //スペキュラ計算
    
    float4 resultColor;
    
    //拡散反射
    resultColor.rgb = albedo.rgb * diffuse;
    resultColor.a = 1.0f;
    
    return resultColor;
}