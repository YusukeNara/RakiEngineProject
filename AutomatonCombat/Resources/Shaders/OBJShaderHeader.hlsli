cbuffer cbuff0 : register(b0)
{
	matrix mat;  //3D�ϊ��s��
    float4 color;//�F
}

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0);   //�A���r�G���g
	float3 m_diffuse  : packoffset(c1);   //�f�B�t���[�Y
	float3 m_specular : packoffset(c2);   //�X�y�L�����[
	float m_alpha     : packoffset(c2.w); //�A���t�@
}

//���_�V�F�[�_�[����W�I���g���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos  : SV_POSITION; //�V�X�e���p���_���W
	float3 normal : NORMAL;      //�@���x�N�g��
	float2 uv     : TEXCOORD;    //uv�l
    float4 worldPos : TEXCOORD1;
};

//�W�I���g���V�F�[�_�[����s�N�Z���V�F�[�_�[��
struct GSOutput
{
    float4 svpos  : SV_POSITION; //�V�X�e���p���_���W�i�����ADS�悤�Ƀs�N�Z���V�F�[�_�[�ł��v�Z����j
    float3 normal : NORMAL;      //�@���x�N�g��
    float2 uv     : TEXCOORD;    //uv�l
    float4 worldPos : TEXCOORD1;
};

//�s�N�Z���V�F�[�_�[�o�͍\����
struct PixelOutput
{
    float4 pixel_color	: SV_TARGET0;
    float4 normal		: SV_TARGET1;
    float4 worldPos		: SV_TARGET2;
};