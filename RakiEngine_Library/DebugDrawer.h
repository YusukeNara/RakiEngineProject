#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

//���C�u�����R���p�C��
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

//�g�p���鎩��G���W���̋@�\�̃C���N���[�h
#include "NY_Object3D.h"
#include "RVector.h"
#include "RenderTargetManager.h"


class DebugDrawer
{
public:

	static DebugDrawer *GetInstance();

	void Init();

	void Rendering();

	void DrawLine(RVector3 pos1, RVector3 pos2);

private:

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;

	//�V�F�[�_�[�I�u�W�F�N�g�i�W���j
	ComPtr<ID3DBlob> line_VS;
	ComPtr<ID3DBlob> line_PS;

	//�����_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;



};

