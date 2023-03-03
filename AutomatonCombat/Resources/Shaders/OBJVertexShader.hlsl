#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//�s�N�Z���V�F�[�_�[�ɓn���l
	//���_���W���̂܂�
    output.worldPos = pos;
	//�r���[�v���W�F�N�V�����ϊ�
	output.svpos = mul(mat, pos);
    output.worldPos = mul(wMat, pos);
	
	output.normal = normal;
	output.uv = uv;
	return output;
}