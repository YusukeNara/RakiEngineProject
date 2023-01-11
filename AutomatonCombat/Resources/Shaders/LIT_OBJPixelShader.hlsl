#include "LIT_OBJHeader.hlsli"

//	2パス目出力用シェーダー（予定）
//	現時点では、GBuffer画像を出力するだけ

//アルベドテクスチャ
Texture2D<float4> albedoTex : register(t0);
//法線テクスチャ
Texture2D<float4> normalTex : register(t1);
//ワールド座標テクスチャ
Texture2D<float4> worldTex  : register(t2);
//サンプラーは変更なし
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    //アルベド情報を取得
    float4 albedo = albedoTex.Sample(smp, input.uv);
    //法線情報取得
    float3 normal = normalTex.Sample(smp, input.uv).xyz;
    //ワールド座標取得
    float3 worldPos = worldTex.Sample(smp, input.uv).xyz;
    
    //法線情報を復元
    normal = (normal * 2.0f) - 1.0f;
    
    //ライト計算
    float3 lightDir = dirLight; //右下奥向きライト
    float diffuse = saturate(dot(normal, -lightDir)); //ディフューズ計算x
    float3 lightColor = float3(0.5, 0.5, 0.5);
    
    //結果保存
    float3 lig = 0.0f;
    float t = max(0.0f, dot(normal, lightDir) * -1.0f);
    
    //ディフューズを合成した値
    lig = lightColor * diffuse;
    
    //ハーフベクトルを求める
    float3 toEye = normalize(eyePos - worldPos);
    //反射計算
    float3 r = reflect(lightDir, normal);
    t = max(0.0f, dot(toEye, r));
    t = pow(t, 4.0f);
    //スペキュラを合成
    lig += lightColor * t;

    //ADS合成
    float4 resultColor = albedo;
    resultColor.xyz *= lig;
    return resultColor;
}