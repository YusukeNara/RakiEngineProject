//�A���x�h�e�N�X�`��
Texture2D<float4> albedoTex : register(t0);
//�@���e�N�X�`��
Texture2D<float4> normalTex : register(t1);


//�������̔z�񂪓��郌�W�X�^��ǉ��\��

//�T���v���[�͕ύX�Ȃ�
SamplerState smp : register(s0);

struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
};