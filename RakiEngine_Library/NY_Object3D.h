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

//定数バッファデータ構造体B0（座標系なので、同じオブジェクトを複数出すなら増やさないとだめ）
struct ConstBufferDataB0
{
	XMMATRIX mat;
	XMFLOAT4 color;
};

//定数バッファデータ構造体B1（マテリアルを使用->モデル依存なので複数いらない）
struct ConstBufferDataB1
{
	XMFLOAT3 amdient;  //アンビエント
	float pad1;//パディング
	XMFLOAT3 diffuse;  //ディフューズ
	float pad2;//パディング
	XMFLOAT3 specular; //スペキュラー
	float alpha;       //アルファ
};

//ビルボードパーティクル頂点データ
struct ParticleVertex
{
	XMFLOAT3 pos;
};

// 2022/03/16 クラスの依存関係を変更
//mgr -> object3d -> model3d
//このクラスは中間に位置させ、modelを所有する
//ただし作成時の処理を変更、mgrにモデルのロード関数を置く
//内部の処理は、model3dを作成 -> モデルを読み込み -> object3dに所有させる


class Object3d
{
public:
	//オブジェクトの名前
	string tagName;

	//色情報
	XMFLOAT4 color = { 1,1,1,1 };

	//ビルボードフラグ
	bool isBillBoard;

public:
	//コンストラクタ
	Object3d() {
		isBillBoard = false;
		isDirty = true;
		//モデルデータ（空）を作成
		model = make_shared<Model3D>();
	};

	//オブジェクトの初期化
	//static void InitObject3D(Object3d *obj, ID3D12Device *dev);
	void InitObject3D(ID3D12Device *dev);

	//ロード済モデルデータの設定
	void SetLoadedModelData(Model3D *loadedModel);

	//変換行列の設定
	void SetWorldMatrix(XMMATRIX matWorld);
	//アフィン変換情報設定
	void SetAffineParam(RVector3 scale, RVector3 rot, RVector3 trans);
	void SetAffineParamScale(RVector3 scale);
	void SetAffineParamRotate(RVector3 rot);
	void SetAffineParamTranslate(RVector3 trans);

	//オブジェクト更新
	void UpdateObject3D();

	//ビルボード更新（カメラオブジェクトをそのまま取り込んで、ビルボード用の更新処理を行う）
	void UpdateBillBoard3D();

	//モデルデータを使用したオブジェクト描画
	//static void DrawModel3D(Object3d *obj, ID3D12GraphicsCommandList *cmd,ID3D12Device *dev);
	//void DrawModel3D(ID3D12GraphicsCommandList *cmd, ID3D12Device *dev);
	//void DrawModel3DSelectTexture(UINT useTexNum);

	void DrawObject();

	//マルチパスレンダリングを使用した描画（マルチパスで作ったリソースを使って描画）
	void DrawMultiPassResource();

	//モデルデータをロード
	void LoadAndSetModelData(string modelname);
	//モデルデータを別オブジェクトから設定する
	void SetAnotherObjectModelData(Object3d *anotherObj);

	void CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum);

private:
	// モデルデータ（ほかのオブジェクトでモデルデータは同一のものを使う場合に備えて、weak_ptrを使用）
	shared_ptr<Model3D> model;

	//アフィン変換情報
	RVector3 scale = { 1,1,1 };
	RVector3 rotation = { 0,0,0 };
	RVector3 position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matWorld;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	//定数バッファビューハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;
	//定数バッファビューハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	//ダーティフラグ
	bool isDirty;

	//適用するリソースの番号
	UINT resourceNumber;
};





