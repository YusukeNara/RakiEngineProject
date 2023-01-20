#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

//�ʏ��Ԃ����C�g��Ԃɕϊ����Az���r�A�e�e�N�X�`���𐶐�����
float4 MakeShadowTexture(float4x4 lvpm, float4 cvpPos, float4 lvpPos,float4 albedo)
{
    float4 result;
    result.w = albedo.w;
    
    cvpPos = mul(lvpm, cvpPos);
    
    if (cvpPos.z > lvpPos.z)
    {
        result.xyz = 0.5;
    }
    
    return result;
}

PixelOutput main(GSOutput input)
{
    PixelOutput result;

	float4 texcolor = tex.Sample(smp, input.uv);
	
    result.pixel_color = float4(texcolor.rgb, 1.0f );
    result.normal.rgb = float3(input.normal.xyz / 2.0f) + 0.5f;
    result.normal.a = 1.0f;
    result.worldPos = mul(mat, input.worldPos);
    
    //�ʏ�J�������z�l
    float4 cz = float4(input.svpos.z, input.svpos.z, input.svpos.z, 1.0f);
    
    //�s�N�Z���̃��C�g��Ԃ�z�l�i�V���h�E�}�b�v�j
    float4 lvppos = mul(lightMat, input.worldPos);
    float4 lz = float4(lvppos.z, lvppos.z, lvppos.z, 1.0f);
    
    //���C�g���z���Ⴂ�l�̏ꍇ�͉e
    //result.zColor = MakeShadowTexture(lightMat, cz, lvppos, result.pixel_color);
    result.zColor = float4(lz.z, lz.z, lz.z, 1.0f);
    
    return result;
}

