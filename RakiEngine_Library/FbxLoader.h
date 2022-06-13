#pragma once

#include "fbxsdk.h"

#include "Raki_DX12B.h"
#include "fbxModel.h"

#include <string>

extern class fbxModel;
extern class Node;

class FbxLoader
{
private:
	using string = std::string;

	string folderPass;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	static const string baseDir;

	static const string defaultTexName;

	void Initialize();

	void Finalize();

	fbxModel* LoadFBXFile(string filename);

	void ParseNodeRecursive(fbxModel* model, FbxNode* fbxnode,Node *parent = nullptr);

	void ParseMesh(fbxModel* model, FbxNode* fbxnode);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	std::string ExtractFileName(const std::string& path);

	void ParseMeshVertices(fbxModel* model, FbxMesh* mesh);

	void ParseMeshFaces(fbxModel* model, FbxMesh* mesh);

	void ParseMaterial(fbxModel* model, FbxNode* node);

	
	//テクスチャはテクスチャマネージャーのしごと


	FbxManager* fbxmgr = nullptr;

	FbxImporter* fbxImporter = nullptr;
};