//アルベドテクスチャ
Texture2D<float4> albedoTex : register(t0);
//法線テクスチャ
Texture2D<float4> normalTex : register(t1);


//光源情報の配列が入るレジスタを追加予定

//サンプラーは変更なし
SamplerState smp : register(s0);

struct VSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv値
};