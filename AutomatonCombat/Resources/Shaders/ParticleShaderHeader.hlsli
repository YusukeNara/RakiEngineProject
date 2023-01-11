cbuffer cbuff0 : register(b0)
{
    matrix mat;                 //�r���[�v���W�F�N�V�����s��
    matrix matBill;             //�r���{�[�h�s��
};

struct VSOutput
{
    float4 pos   : POSITION;    //���_���W
    float scale  : TEXCOORD;    //�X�P�[�����O
    float4 color : COLOR;       //�F���
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e�����_
    float2 uv    : TEXCOORD;    //uv�l
    float4 color : COLOR;       //�F���
};

struct PSOutput
{
    float4 pixel_color  : SV_TARGET0;
    float4 normal       : SV_TARGET1;
    float4 worldPos     : SV_TARGET2;
};