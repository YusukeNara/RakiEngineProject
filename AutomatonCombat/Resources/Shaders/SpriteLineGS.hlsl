#include "Sprite.hlsli"

[maxvertexcount(4)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	//�o�͗p�ϐ�
    GSOutput element;
    
    //freedata�Ń��[�J���ȃ|���S�����`
    float2 local[4] =
    {
        { 0, -input[0].size.y }, //����
        { -input[0].size.x, 0 }, //����
        { 0 + 1, -input[0].size.y + 1 }, //�E��
        { -input[0].size.x + 1, 0 + 1 }, //�E��
    };
	
	//�c���������Ƃɏo�̓I�t�Z�b�g���쐬
    float2 sizeOffset[4] =
    {
        { 0, 0 }, //����
        { input[0].size.x, input[0].size.y }, //����
        { 0 + 3, 0 + 3 }, //�E��
        { input[0].size.x + 3, input[0].size.y + 3 }, //�E��
    };
	//uv�I�t�Z�b�g���i�[
    float2 uvOffset[4] =
    {
        { input[0].uvOffset.x, input[0].uvOffset.w },
        { input[0].uvOffset.x, input[0].uvOffset.y },
        { input[0].uvOffset.z, input[0].uvOffset.w },
        { input[0].uvOffset.z, input[0].uvOffset.y },
    };
	
	//�_�̓��͂���l�p�`���o��
    for (uint i = 0; i < 4; i++)
    {
        element.pos = input[0].svpos;
		//�o�͒��_�̃��[�J�����W���A�`��T�C�Y�ɒ����i�����j
        element.pos.xy += sizeOffset[i];
		//���[�J�����W�����[���h���W�ɕϊ�
        element.pos = mul(input[0].ins_matrix, element.pos);
		//uv���W�̓I�t�Z�b�g����i���ƂŊO������擾�j
        element.uv = uvOffset[i];
		//�ق��͎g���̂ł��̂܂�
        element.ins_matrix = input[0].ins_matrix;
        element.size = input[0].size;
        element.uvOffset = input[0].uvOffset;
        element.color = input[0].color;
        element.freedata = input[0].freedata;
		//�o��
        output.Append(element);
    }
}