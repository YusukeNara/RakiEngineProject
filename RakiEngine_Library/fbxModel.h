#pragma once

#pragma warning (push)
#pragma warning (disable:6001)
#pragma warning (disable:26812)
#pragma warning (disable:26451)
#pragma warning (disable:26495)

#include "Raki_DX12B.h"
#include "TexManager.h"

#include <Windows.h>
#include <wrl.h>
#include <fbxsdk.h>

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
	UINT boneIndex[4];
	float boneWeight[4];
};

struct Bone
{
	std::string name;
	DirectX::XMMATRIX invInitialBone;
	FbxCluster* fbxCluster;
	Bone(std::string name) {
		this->name = name;
	}

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
	fbxModel(){}
	~fbxModel();

	void CreateBuffers();

	void Draw();

	inline const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	inline const fbxMaterial& GetMaterial() { return material; }

	inline std::vector<Bone>& GetBones() { return bones; }

	FbxScene* GetFbxScene() { return fbxScene; }

	static const int BONE_INDICES_MAX = 4;

private:
	//���f����
	std::string name;

	std::vector<Node> nodes;
	std::vector<Bone> bones;

	Node* meshNode = nullptr;

	std::vector<fbxVertex> vertices;
	ComPtr<ID3D12Resource> vertbuff;

	std::vector<unsigned short> indices;
	ComPtr<ID3D12Resource>		indexbuff;

	D3D12_VERTEX_BUFFER_VIEW vbview = {};
	D3D12_INDEX_BUFFER_VIEW ibview = {};

	fbxMaterial material;

	FbxScene* fbxScene = nullptr;
};

#pragma warning (pop)