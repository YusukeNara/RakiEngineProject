#include "Sprite.hlsli"

//�X�v���C�g�̓��͂�1���_�̂�
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD,matrix instanceMatrix : INSTANCE_WORLD_MAT,float2 size : INSTANCE_DRAWSIZE)
{
	VSOutput output;
    output.svpos = pos;
	output.uv = uv;
    output.ins_matrix = instanceMatrix;
    output.size = size;
	return output;
}