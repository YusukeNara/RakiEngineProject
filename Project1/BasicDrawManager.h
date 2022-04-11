#pragma once
#include <DirectXMath.h>
#include "Raki_DX12B.h"

using namespace DirectX;

#define MAX_TEX (32)

//���_�f�[�^�\����
struct BasicVertex {
	XMFLOAT3 pos;
};

//�萔�o�b�t�@�\����B0
struct BasicConstBufferB0 {
	XMFLOAT4 color;
	XMMATRIX mat;
};



/// <summary>
/// �V���O���g���A�SBasicDraw�̋��ʊǗ��N���X
/// </summary>
class BasicDrawManager final
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	/// �����o�ϐ�

	//�O���t�B�b�N�X�p�C�v���C���I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�\����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff[MAX_TEX];
	//�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> BasicVS;
	ComPtr<ID3DBlob> BasicPS;
	//�e�}�`�`��p�W�I���g��
	ComPtr<ID3DBlob> BasicSquare;

};

