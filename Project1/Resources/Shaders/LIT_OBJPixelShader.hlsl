#include "OBJShaderHeader.hlsli"

//	2�p�X�ڏo�͗p�V�F�[�_�[�i�\��j
//	�����_�ł́AGBuffer�摜���o�͂��邾��

//�f�B�t�@�[�h�ł́A���̃��W�X�^��GBuffer������
Texture2D<float4> tex : register(t0);
//�T���v���[�͕ύX�Ȃ�
SamplerState smp : register(s0);
//�������̔z�񂪓��郌�W�X�^��ǉ��\��

float4 main(GSOutput input) : SV_TARGET
{
    //�����_�ł́A���������_�����O���ʂ���������
    //���̃V�F�[�_�[��Lit�����Ȃ̂ŁA��Ɍ����v�Z������
    
    
    float4 texColor = float4(tex.Sample(smp, input.uv));

    float3 light = normalize(float3(1, -1, 1)); //�E�����������C�g
    float diffuse = saturate(dot(-light, input.normal));
	
    float3 shade_color;
    shade_color = m_ambient;
    shade_color += m_diffuse * diffuse;
	
    float4 texcolor = tex.Sample(smp, input.uv);
	
    //�ʏ�̌��ʂ��o��
    return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha) * color;
}