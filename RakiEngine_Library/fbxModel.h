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

//頂点データ
struct fbxVertex
{
	DirectX::XMFLOAT3 pos;   //xyz
	DirectX::XMFLOAT3 normal;//法線
	DirectX::XMFLOAT2 uv;    //uv
};

//マテリアルデータ構造体
struct fbxMaterial
{
	std::string   mtlName;      //マテリアル名(objファイルからではない場合、使わなくていい？)
	DirectX::XMFLOAT3 ambient;      //アンビエント
	DirectX::XMFLOAT3 diffuse;      //ディフューズ
	DirectX::XMFLOAT3 specurar;     //スペキュラー
	float    alpha;        //アルファ
	std::string		texFileName;  //テクスチャ名
	UINT			texNumber;    //テクスチャ番号(使用する場合必ず必要)
	DirectX::TexMetadata mdata = {};
	DirectX::ScratchImage simg = {};

	//コンストラクタ
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
	//モデル名
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

