#include "LIT_OBJHeader.hlsli"

//	2�p�X�ڏo�͗p�V�F�[�_�[�i�\��j
//	�����_�ł́AGBuffer�摜���o�͂��邾��

//�A���x�h�e�N�X�`��
Texture2D<float4> albedoTex : register(t0);
//�@���e�N�X�`��
Texture2D<float4> normalTex : register(t1);
//�T���v���[�͕ύX�Ȃ�
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    //�A���x�h�����擾
    float4 albedo = albedoTex.Sample(smp, input.uv);
    //�@�����擾
    float3 normal = normalTex.Sample(smp, input.uv).xyz;
    
    normal = (normal * 2.0f) - 1.0f;
    
    //���C�g�v�Z
    float3 lightDir = normalize(float3(1, -1, 1)); //�E�����������C�g
    float diffuse = saturate(dot(normal,-lightDir)); //�f�B�t���[�Y�v�Z
    
    //���_���W
    float3 eye = eyePos.xyz;
    
    //�X�y�L�����v�Z
    
    float4 resultColor;
    
    //�g�U����
    resultColor.rgb = albedo.rgb * diffuse;
    resultColor.a = 1.0f;
    
    return resultColor;
}