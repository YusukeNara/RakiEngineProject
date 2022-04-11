#pragma once
//DirectX
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
//c++
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//myEngine
#include "NY_Model.h"
#include "RVector.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

//�萔�o�b�t�@�f�[�^�\����B0�i���W�n�Ȃ̂ŁA�����I�u�W�F�N�g�𕡐��o���Ȃ瑝�₳�Ȃ��Ƃ��߁j
struct ConstBufferDataB0
{
	XMMATRIX mat;
	XMFLOAT4 color;
};

//�萔�o�b�t�@�f�[�^�\����B1�i�}�e���A�����g�p->���f���ˑ��Ȃ̂ŕ�������Ȃ��j
struct ConstBufferDataB1
{
	XMFLOAT3 amdient;  //�A���r�G���g
	float pad1;//�p�f�B���O
	XMFLOAT3 diffuse;  //�f�B�t���[�Y
	float pad2;//�p�f�B���O
	XMFLOAT3 specular; //�X�y�L�����[
	float alpha;       //�A���t�@
};

//�r���{�[�h�p�[�e�B�N�����_�f�[�^
struct ParticleVertex
{
	XMFLOAT3 pos;
};

// 2022/03/16 �N���X�̈ˑ��֌W��ύX
//mgr -> object3d -> model3d
//���̃N���X�͒��ԂɈʒu�����Amodel�����L����
//�������쐬���̏�����ύX�Amgr�Ƀ��f���̃��[�h�֐���u��
//�����̏����́Amodel3d���쐬 -> ���f����ǂݍ��� -> object3d�ɏ��L������


class Object3d
{
public:
	//�I�u�W�F�N�g�̖��O
	string tagName;

	//�F���
	XMFLOAT4 color = { 1,1,1,1 };

	//�r���{�[�h�t���O
	bool isBillBoard;

public:
	//�R���X�g���N�^
	Object3d() {
		isBillBoard = false;
		isDirty = true;
		//���f���f�[�^�i��j���쐬
		model = make_shared<Model3D>();
	};

	//�I�u�W�F�N�g�̏�����
	//static void InitObject3D(Object3d *obj, ID3D12Device *dev);
	void InitObject3D(ID3D12Device *dev);

	//���[�h�σ��f���f�[�^�̐ݒ�
	void SetLoadedModelData(Model3D *loadedModel);

	//�ϊ��s��̐ݒ�
	void SetWorldMatrix(XMMATRIX matWorld);
	//�A�t�B���ϊ����ݒ�
	void SetAffineParam(RVector3 scale, RVector3 rot, RVector3 trans);
	void SetAffineParamScale(RVector3 scale);
	void SetAffineParamRotate(RVector3 rot);
	void SetAffineParamTranslate(RVector3 trans);

	//�I�u�W�F�N�g�X�V
	void UpdateObject3D();

	//�r���{�[�h�X�V�i�J�����I�u�W�F�N�g�����̂܂܎�荞��ŁA�r���{�[�h�p�̍X�V�������s���j
	void UpdateBillBoard3D();

	//���f���f�[�^���g�p�����I�u�W�F�N�g�`��
	//static void DrawModel3D(Object3d *obj, ID3D12GraphicsCommandList *cmd,ID3D12Device *dev);
	//void DrawModel3D(ID3D12GraphicsCommandList *cmd, ID3D12Device *dev);
	//void DrawModel3DSelectTexture(UINT useTexNum);

	void DrawObject();

	//�}���`�p�X�����_�����O���g�p�����`��i�}���`�p�X�ō�������\�[�X���g���ĕ`��j
	void DrawMultiPassResource();

	//���f���f�[�^�����[�h
	void LoadAndSetModelData(string modelname);
	//���f���f�[�^��ʃI�u�W�F�N�g����ݒ肷��
	void SetAnotherObjectModelData(Object3d *anotherObj);

	void CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum);

private:
	// ���f���f�[�^�i�ق��̃I�u�W�F�N�g�Ń��f���f�[�^�͓���̂��̂��g���ꍇ�ɔ����āAweak_ptr���g�p�j
	shared_ptr<Model3D> model;

	//�A�t�B���ϊ����
	RVector3 scale = { 1,1,1 };
	RVector3 rotation = { 0,0,0 };
	RVector3 position = { 0,0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	//�萔�o�b�t�@�r���[�n���h��(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;
	//�萔�o�b�t�@�r���[�n���h��(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	//�_�[�e�B�t���O
	bool isDirty;

	//�K�p���郊�\�[�X�̔ԍ�
	UINT resourceNumber;
};





