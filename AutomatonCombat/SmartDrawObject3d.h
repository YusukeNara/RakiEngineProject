#pragma once
#include <iostream>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>


using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

//頂点データ構造体
struct SmartVertexPosNormalUv
{
	XMFLOAT3 pos;    //座標
	XMFLOAT3 normal; //法線
	XMFLOAT3 uv;     //uv
};

//定数バッファ構造体
struct SmartConstBufferb1 {
	XMMATRIX mat;   //座標変換行列
	XMMATRIX color; //色行列
};

/// <summary>
/// <para>ジオメトリシェーダーを利用して、テクスチャなしで即座に指定ワールド座標に立体などを描画するクラス</para>
/// <para>現状は実験段階</para>
/// </summary>
class SmartDrawObject3d
{
public:
	//シェーダーオブジェクト（もしかしたら図形1つにつき作る必要があるかもだけど知らん）
	static ComPtr<ID3DBlob> SmartVS;
	static ComPtr<ID3DBlob> SmartPS;

	//ジオメトリシェーダ（これは描画する立体に応じて変える）
	static ComPtr<ID3DBlob> SmartBoardGS;//板ポリ

	//グラフィックスパイプラインオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	//グラフィックスパイプラインステート構造体（設定を適宜入れ替える必要があるためメンバ変数）
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;

public:



};


