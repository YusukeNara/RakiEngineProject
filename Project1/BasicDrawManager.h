#pragma once
#include <DirectXMath.h>
#include "Raki_DX12B.h"

using namespace DirectX;

#define MAX_TEX (32)

//頂点データ構造体
struct BasicVertex {
	XMFLOAT3 pos;
};

//定数バッファ構造体B0
struct BasicConstBufferB0 {
	XMFLOAT4 color;
	XMMATRIX mat;
};



/// <summary>
/// シングルトン、全BasicDrawの共通管理クラス
/// </summary>
class BasicDrawManager final
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	/// メンバ変数

	//グラフィックスパイプラインオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	//グラフィックスパイプラインステート構造体
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff[MAX_TEX];
	//シェーダーオブジェクト
	ComPtr<ID3DBlob> BasicVS;
	ComPtr<ID3DBlob> BasicPS;
	//各図形描画用ジオメトリ
	ComPtr<ID3DBlob> BasicSquare;

};

