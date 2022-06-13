#pragma once

#include "Raki_DX12B.h"
#include "TexManager.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>

#include <string>
#include <vector>

class Node
{
public:
	std::string name;

	DirectX::XMVECTOR scaling;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR translation;
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;

	Node* parent = nullptr;
};

//���_�f�[�^
struct fbxVertex
{
	DirectX::XMFLOAT3 pos;   //xyz
	DirectX::XMFLOAT3 normal;//�@��
	DirectX::XMFLOAT2 uv;    //uv
};

//�}�e���A���f�[�^�\����
struct fbxMaterial
{
	std::string   mtlName;      //�}�e���A����(obj�t�@�C������ł͂Ȃ��ꍇ�A�g��Ȃ��Ă����H)
	DirectX::XMFLOAT3 ambient;      //�A���r�G���g
	DirectX::XMFLOAT3 diffuse;      //�f�B�t���[�Y
	DirectX::XMFLOAT3 specurar;     //�X�y�L�����[
	float    alpha;        //�A���t�@
	std::string		texFileName;  //�e�N�X�`����
	UINT			texNumber;    //�e�N�X�`���ԍ�(�g�p����ꍇ�K���K�v)
	DirectX::TexMetadata mdata = {};
	DirectX::ScratchImage simg = {};

	//�R���X�g���N�^
	fbxMaterial() {
		ambient = { 1.0f,1.0f,1.0f };
		diffuse = { 0.8f,0.8f,0.8f };
		specurar = { 0.8f,0.8f,0.8f };
		alpha = 1.0f;
	}
};


class fbxModel
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	friend class FbxLoader;

	void CreateBuffers();

	void Draw();

	inline const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	inline const fbxMaterial& GetMaterial() { return material; }

private:
	//���f����
	std::string name;

	std::vector<Node> nodes;

	Node* meshNode = nullptr;

	std::vector<fbxVertex> vertices;
	ComPtr<ID3D12Resource> vertbuff;

	std::vector<unsigned short> indices;
	ComPtr<ID3D12Resource> indexbuff;

	D3D12_VERTEX_BUFFER_VIEW vbview = {};
	D3D12_INDEX_BUFFER_VIEW ibview = {};

	fbxMaterial material;
};

