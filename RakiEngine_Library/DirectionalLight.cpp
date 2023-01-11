#include "DirectionalLight.h"

DirectX::XMFLOAT3 DirectionalLight::dir = { 1.0f,-1.0f,1.0f };

void DirectionalLight::SetLightDir(float x, float y, float z)
{
	dir.x = x;
	dir.y = y;
	dir.z = z;
}
