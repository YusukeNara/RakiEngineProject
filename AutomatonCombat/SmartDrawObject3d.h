#pragma once
#include <iostream>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>


using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

//���_�f�[�^�\����
struct SmartVertexPosNormalUv
{
	XMFLOAT3 pos;    //���W
	XMFLOAT3 normal; //�@��
	XMFLOAT3 uv;     //uv
};

//�萔�o�b�t�@�\����
struct SmartConstBufferb1 {
	XMMATRIX mat;   //���W�ϊ��s��
	XMMATRIX color; //�F�s��
};

/// <summary>
/// <para>�W�I���g���V�F�[�_�[�𗘗p���āA�e�N�X�`���Ȃ��ő����Ɏw�胏�[���h���W�ɗ��̂Ȃǂ�`�悷��N���X</para>
/// <para>����͎����i�K</para>
/// </summary>
class SmartDrawObject3d
{
public:
	//�V�F�[�_�[�I�u�W�F�N�g�i������������}�`1�ɂ����K�v�����邩�������ǒm���j
	static ComPtr<ID3DBlob> SmartVS;
	static ComPtr<ID3DBlob> SmartPS;

	//�W�I���g���V�F�[�_�i����͕`�悷�闧�̂ɉ����ĕς���j
	static ComPtr<ID3DBlob> SmartBoardGS;//�|��

	//�O���t�B�b�N�X�p�C�v���C���I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�\���́i�ݒ��K�X����ւ���K�v�����邽�߃����o�ϐ��j
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;

public:



};


