#include "DirectionalLight.h"

#include "NY_Camera.h"

DirectX::XMFLOAT3 DirectionalLight::dir = { 1.0f,-1.0f,1.0f };
DirectX::XMMATRIX DirectionalLight::lightCamera;

void DirectionalLight::SetLightDir(float x, float y, float z)
{
	dir.x = x;
	dir.y = y;
	dir.z = z;

	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 up = { 0,1,0 };

	//�r���[�s��i�[
	lightCamera = XMMatrixLookAtLH(XMLoadFloat3(&dir), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void DirectionalLight::SetLightPos(RVector3 lightPos, RVector3 lightVec, RVector3 lightUp)
{
	lightCamera = XMMatrixLookAtLH(XMLoadFloat3(&lightPos), XMLoadFloat3(&lightVec), XMLoadFloat3(&lightUp));

	//���W�ƃx�N�g�����烉�C�g�������v�Z
	dir = { RVector3(lightPos - lightVec).norm() * 2 };
}
