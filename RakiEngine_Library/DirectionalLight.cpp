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

	//ƒrƒ…[s—ñŠi”[
	lightCamera = XMMatrixLookAtLH(XMLoadFloat3(&dir), XMLoadFloat3(&target), XMLoadFloat3(&up));

	lightCamera *= NY_Camera::Get()->GetMatrixProjection();

	
}
