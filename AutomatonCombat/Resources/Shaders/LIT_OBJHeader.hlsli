
//���[���h���W�e�N�X�`��

//�J�������_���W�X�^ b0
cbuffer cbuff0 : register(b0)
{
    float3 eyePos : packoffset(c0);
}

cbuffer cbuff1 : register(b1)
{
    float3 dirLight;
}


struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
};