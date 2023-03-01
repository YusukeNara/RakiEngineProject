#include "LIT_OBJHeader.hlsli"

//	2�p�X�ڏo�͗p�V�F�[�_�[�i�\��j
//	�����_�ł́AGBuffer�摜���o�͂��邾��

//�A���x�h�e�N�X�`��
Texture2D<float4> albedoTex : register(t0);
//�@���e�N�X�`��
Texture2D<float4> normalTex : register(t1);
//���[���h���W�e�N�X�`��
Texture2D<float4> worldTex  : register(t2);
//�[�x���e�N�X�`��
Texture2D<float4> LdepthTex : register(t3);
//�T���v���[�͕ύX�Ȃ�
SamplerState smp : register(s0);

//�s�N�Z����z�l�����C�g��Ԃ̍��W�ɕϊ�����

float4 main(VSOutput input) : SV_TARGET
{
    //�A���x�h�����擾
    float4 albedo = albedoTex.Sample(smp, input.uv);
    //�@�����擾
    float3 normal = normalTex.Sample(smp, input.uv).xyz;
    //���[���h���W�擾
    float3 worldPos = worldTex.Sample(smp, input.uv).xyz;
    //�[�x���擾
    float4 depth = LdepthTex.Sample(smp, input.uv);
    
    //�@�����𕜌�
    normal = (normal * 2.0f) - 1.0f;

    float3 color = float3(1.0f, 1.0f, 1.0);
    
    
    //�g�U���˂��v�Z
    float3 lig = 0.0f;
    float t = max(0.0f, dot(normal, dirLight) * -1.0f);
    lig = color * t;
    
    //�X�y�L����
    float3 toEye = normalize(eyePos - worldPos);
    float3 r = reflect(dirLight, normal);
    t = max(0.0f, dot(toEye, r));
    t = pow(t, 5.0f);
    lig += color * t;
    
    float4 resultColor = albedo;
    resultColor.xyz *= lig;

    return resultColor;
}