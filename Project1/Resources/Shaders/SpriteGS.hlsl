#include "Sprite.hlsli"

[maxvertexcount(4)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	//�o�͗p�ϐ�
    GSOutput output;
	
	//�c���������Ƃɏo�̓I�t�Z�b�g���쐬
    float2 sizeOffset[4] =
    {
        { 0              , input[0].size.y },	//����
        { 0              , 0 },					//����
        { input[0].size.x, 0 },					//�E��
        { input[0].size.x, input[0].size.y },	//�E��
    };
	
	//�_�̓��͂���l�p�`���o��
	for (uint i = 0; i < 4; i++)
	{
		//�o�͒��_�̃��[�J�����W���A�`��T�C�Y�ɒ���
        output.pos = input[0].svpos.xy + sizeOffset[i];
		//���[�J�����W�����[���h���W�ɕϊ�
        output.pos = mul(input[0].ins_matrix, output.pos);
		
	}
}