#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "RTex.h"

//���C�u�����R���p�C��
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct cbuffer_b0
{
	DirectX::XMFLOAT3 eyePos;	//�J�������W
	float pad;
};

struct cbuffer_b1
{
	DirectX::XMFLOAT3 lightDir;
	float pad;
};

class DiferredRenderingMgr
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�R���X�g���N�^�A�f�X�g���N�^
	DiferredRenderingMgr(){};
	~DiferredRenderingMgr(){};

	//������
	void Init(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd);

	//�`��
	void Rendering(RTex* gBuffer);


private:
	struct VertexPosUv {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	void ShaderCompile();

	void CreateGraphicsPipeline();

	void UpdateConstBuff();

	//�f�o�C�X�A�R�}���h���X�g�̃|�C���^
	ID3D12Device				*m_dev;
	ID3D12GraphicsCommandList	*m_cmd;
	//�O���t�B�b�N�X�p�C�v���C���A���[�g�V�O�l�`��
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	//�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob>			m_litVS;
	ComPtr<ID3D10Blob>			m_litPS;
	//�y���|���S�����\�[�X
	ComPtr<ID3D12Resource>		m_vertBuff;		//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW	m_vbview;		//VBV
	//�萔�o�b�t�@�i�J�������_���W�j
	ComPtr<ID3D12Resource>		m_constBuffEyePos;
	ComPtr<ID3D12Resource>		m_constBuffDirLight;

};

