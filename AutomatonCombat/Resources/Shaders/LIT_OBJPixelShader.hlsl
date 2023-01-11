#include "LIT_OBJHeader.hlsli"

//	2�p�X�ڏo�͗p�V�F�[�_�[�i�\��j
//	�����_�ł́AGBuffer�摜���o�͂��邾��

//�A���x�h�e�N�X�`��
Texture2D<float4> albedoTex : register(t0);
//�@���e�N�X�`��
Texture2D<float4> normalTex : register(t1);
//���[���h���W�e�N�X�`��
Texture2D<float4> worldTex  : register(t2);
//�T���v���[�͕ύX�Ȃ�
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    //�A���x�h�����擾
    float4 albedo = albedoTex.Sample(smp, input.uv);
    //�@�����擾
    float3 normal = normalTex.Sample(smp, input.uv).xyz;
    //���[���h���W�擾
    float3 worldPos = worldTex.Sample(smp, input.uv).xyz;
    
    //�@�����𕜌�
    normal = (normal * 2.0f) - 1.0f;
    
    //���C�g�v�Z
    float3 lightDir = dirLight; //�E�����������C�g
    float diffuse = saturate(dot(normal, -lightDir)); //�f�B�t���[�Y�v�Zx
    float3 lightColor = float3(0.5, 0.5, 0.5);
    
    //���ʕۑ�
    float3 lig = 0.0f;
    float t = max(0.0f, dot(normal, lightDir) * -1.0f);
    
    //�f�B�t���[�Y�����������l
    lig = lightColor * diffuse;
    
    //�n�[�t�x�N�g�������߂�
    float3 toEye = normalize(eyePos - worldPos);
    //���ˌv�Z
    float3 r = reflect(lightDir, normal);
    t = max(0.0f, dot(toEye, r));
    t = pow(t, 4.0f);
    //�X�y�L����������
    lig += lightColor * t;

    //ADS����
    float4 resultColor = albedo;
    resultColor.xyz *= lig;
    return resultColor;
}