cbuffer cbuff0 : register(b0)
{
    matrix mat; //3D�ϊ��s��
    float4 color; //�F
    matrix lightMat;
}

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0); //�A���r�G���g
    float3 m_diffuse : packoffset(c1); //�f�B�t���[�Y
    float3 m_specular : packoffset(c2); //�X�y�L�����[
    float m_alpha : packoffset(c2.w); //�A���t�@
}

static const int MAX_BONES = 32;

cbuffer skinnning : register(b4)
{
    matrix matSkinning[MAX_BONES];
}

struct VSInput
{
    float4 svpos        : POSITION; //�V�X�e���p���_���W
};

struct fbxVSInput
{
    float4  svpos           : POSITION; //�V�X�e���p���_���W
    uint4   boneIndices     : BONEINDICES;
    float4  boneWeights     : BONEWEIGHTS;
};

//���_�V�F�[�_�[����W�I���g���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos        : SV_POSITION; //�V�X�e���p���_���W
};

struct SkinOutput
{
    float4 pos;
};

//�s�N�Z���V�F�[�_�[�o�͍\����
struct PixelOutput
{
    float4 depthColor   : SV_TARGET;
};