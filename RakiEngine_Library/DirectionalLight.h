#pragma once
#include "BaseLight.h"
#include <DirectXMath.h>

class DirectionalLight 
{
public:

	static void SetLightDir(float x, float y, float z);

	static DirectX::XMFLOAT3 GetLightDir() { return dir; }

	static DirectX::XMMATRIX GetLightCamera() { return lightCamera; }

private:
	static DirectX::XMFLOAT3 dir;

	static DirectX::XMMATRIX lightCamera;
};

