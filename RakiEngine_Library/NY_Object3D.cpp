#include "NY_Object3D.h"
#include "FbxLoader.h"
#include "NY_Camera.h"
#include "TexManager.h"
#include "Raki_DX12B.h"
#include "NY_Object3DMgr.h"
#include "RenderTargetManager.h"

//----- NY_Object3D -----//

void Object3d::InitObject3D(ID3D12Device *dev)
{

	HRESULT result;
	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);
	
	//�萔�o�b�t�@b0����
	result =  dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	//�萔�o�b�t�@b1����
	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);

}

void Object3d::SetLoadedModelData(Model3D *loadedModel)
{
	//model = loadedModel;

	isDirty = true;
}

void Object3d::SetWorldMatrix(XMMATRIX matWorld)
{
	//�ϊ��s���ݒ�
	this->matWorld = matWorld;
	//�_�[�e�B�t���O�L����
	isDirty = true;
}

void Object3d::SetAffineParam(RVector3 scale, RVector3 rot, RVector3 trans)
{
	//�A�t�B���ϊ������r���A�l���قȂ�ꍇ�X�V����
	if (this->scale != scale) {
		this->scale = scale;
		isDirty = true;
	}
	if (this->rotation != rot) {
		this->rotation = rot;
		isDirty = true;
	}
	if (this->position != trans) {
		this->position = trans;
		isDirty = true;
	}

	//�_�[�e�B�t���O���L���̂Ƃ�
	if (isDirty == true) {
		//�����ɉ����ăp�����[�^�X�V
		if (isBillBoard != true) {
			UpdateObject3D();
		}
		else {
			UpdateBillBoard3D();
		}
	}
}

void Object3d::SetAffineParamScale(RVector3 scale)
{
	if (this->scale != scale) {
		this->scale = scale;
		isDirty = true;
	}

}

void Object3d::SetAffineParamRotate(RVector3 rot)
{
	if (this->rotation != rot) {
		this->rotation = rot;
		isDirty = true;
	}

}

void Object3d::SetAffineParamTranslate(RVector3 trans)
{
	if (this->position != trans) {
		this->position = trans;
		isDirty = true;
	}
}

void Object3d::UpdateObject3D()
{
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;
	matWorld *= matTrans;

	//�萔�o�b�t�@B0�f�[�^�]��
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = matWorld * camera->GetMatrixView() * camera->GetMatrixProjection();
		ConstMapB0->color = this->color;
		constBuffB0->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@B1�f�[�^�]��
	ConstBufferDataB1 *ConstMapB1 = nullptr;
	if (isThisModel == MODEL_DATA_FBX) {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = fbxmodel->GetMaterial().ambient;
			ConstMapB1->diffuse = fbxmodel->GetMaterial().diffuse;
			ConstMapB1->specular = fbxmodel->GetMaterial().specurar;
			ConstMapB1->alpha = fbxmodel->GetMaterial().alpha;
			constBuffB1->Unmap(0, nullptr);
		}
	}
	else {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = model->material.ambient;
			ConstMapB1->diffuse = model->material.diffuse;
			ConstMapB1->specular = model->material.specurar;
			ConstMapB1->alpha = model->material.alpha;
			constBuffB1->Unmap(0, nullptr);
		}
	}


	//�_�[�e�B�t���O���Z�b�g
	isDirty = false;
	if (isThisModel == MODEL_DATA_FBX) {

	}
	else {
		model->Update();
	}

}

void Object3d::UpdateBillBoard3D()
{

	//���[���h�ϊ��p
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();

	matWorld *= camera->GetMatrixBillBoardAll();

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//�萔�o�b�t�@B0�f�[�^�]��
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = matWorld * camera->GetMatrixView() * camera->GetMatrixProjection();
		ConstMapB0->color = this->color;
		constBuffB0->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@B1�f�[�^�]��
	ConstBufferDataB1* ConstMapB1 = nullptr;
	if (isThisModel == MODEL_DATA_FBX) {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = fbxmodel->GetMaterial().ambient;
			ConstMapB1->diffuse = fbxmodel->GetMaterial().diffuse;
			ConstMapB1->specular = fbxmodel->GetMaterial().specurar;
			ConstMapB1->alpha = fbxmodel->GetMaterial().alpha;
			constBuffB1->Unmap(0, nullptr);
		}
	}
	else {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = model->material.ambient;
			ConstMapB1->diffuse = model->material.diffuse;
			ConstMapB1->specular = model->material.specurar;
			ConstMapB1->alpha = model->material.alpha;
			constBuffB1->Unmap(0, nullptr);
		}
	}

	model->Update();
}

void Object3d::DrawObject()
{
	UpdateObject3D();

	//�`��X�^���o�C
	NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

	if (isThisModel == MODEL_DATA_FBX) {
		//�萔�o�b�t�@�ݒ�
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
		//�萔�o�b�t�@�ݒ�
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

		fbxmodel->Draw();
	}
	else {
		//���_�o�b�t�@�ݒ�
		RAKI_DX12B_CMD->IASetVertexBuffers(0, 1, &model->vbView);
		//�C���f�b�N�X�o�b�t�@�ݒ�
		RAKI_DX12B_CMD->IASetIndexBuffer(&model->ibview);
		//�萔�o�b�t�@�ݒ�
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
		//�萔�o�b�t�@�ݒ�
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

		//�V�F�[�_�[���\�[�X�r���[���Z�b�g
		RAKI_DX12B_CMD->SetGraphicsRootDescriptorTable(2,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
				model->material.texNumber, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

		//�`��
		RAKI_DX12B_CMD->DrawIndexedInstanced(UINT(model->indices.size()), 1, 0, 0, 0);
	}

}

void Object3d::DrawRTexObject(int rtHandle)
{
	//�͈͊O�Q�Ƃ����m
	if (RenderTargetManager::GetInstance()->isHandleOutOfRange(rtHandle)) { return; }

	if (RenderTargetManager::GetInstance()->isNullHandle(rtHandle)) { return; }

	UpdateObject3D();

	NY_Object3DManager::Get()->SetCommonBeginDrawObject3DFeatRTex(rtHandle);

	//���_�o�b�t�@�ݒ�
	RAKI_DX12B_CMD->IASetVertexBuffers(0, 1, &model->vbView);
	//�C���f�b�N�X�o�b�t�@�ݒ�
	RAKI_DX12B_CMD->IASetIndexBuffer(&model->ibview);
	//�萔�o�b�t�@�ݒ�
	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//�萔�o�b�t�@�ݒ�
	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	RAKI_DX12B_CMD->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(RenderTargetManager::GetInstance()->renderTextures[rtHandle]->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
			0, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��
	RAKI_DX12B_CMD->DrawIndexedInstanced(UINT(model->indices.size()), 1, 0, 0, 0);
}

void Object3d::DrawMultiPassResource()
{




}

void Object3d::LoadAndSetModelData(string modelname)
{
	//���f���f�[�^��ǂݍ���Őݒ肷��
	model.get()->LoadObjModel(modelname.c_str());

	isThisModel = MODEL_DATA_OBJ;
}

void Object3d::LoadAndSetModelData_Fbx(string filename)
{
	//���f���f�[�^�����[�h���ď��L�����ڍs
	fbxModel *fmodel = FbxLoader::GetInstance()->LoadFBXFile(filename);
	fbxmodel.reset(fmodel);

	isThisModel = MODEL_DATA_FBX;
}

void Object3d::SetAnotherObjectModelData(Object3d *anotherObj)
{
	//�ʃI�u�W�F�N�g�̃��f���f�[�^���擾����
	this->model = anotherObj->model;
}

void Object3d::CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum)
{
	model.get()->CreatePlaneModelXY(x_size, y_size, x_uv, y_uv, useTexNum, nullptr);
}


