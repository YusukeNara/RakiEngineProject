#include "ShadowMapShaderHeader.hlsli"


VSOutput main(float4 pos : POSITION)
{
    VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
	//�r���[�v���W�F�N�V�����ϊ�
    output.svpos = mul(lightMat, pos);
    
    return output;
}