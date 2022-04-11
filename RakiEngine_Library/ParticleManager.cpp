#include <d3dcompiler.h>

#include "ParticleManager.h"
#include "Raki_DX12B.h"
#include "TexManager.h"

#include <iostream>

#pragma comment(lib,"d3dcompiler.lib")

const int ParticleManager::MAX_VERTEX;

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 &rhs) {
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

static void operator+=(DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
}

const DirectX::XMFLOAT4 operator-(DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 &rhs) {
	XMFLOAT4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4 &lhs, const float a) {
	XMFLOAT4 result;
	result.x = lhs.x / a;
	result.y = lhs.y / a;
	result.z = lhs.z / a;
	result.w = lhs.w / a;
	return result;
}

const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4 &lhs, const float a) {
	XMFLOAT4 result;
	result.x = lhs.x * a;
	result.y = lhs.y * a;
	result.z = lhs.z * a;
	result.w = lhs.w * a;
	return result;
}


ParticleManager *ParticleManager::Create() {

	//パーティクルマネージャー生成
	ParticleManager *pm = new ParticleManager(
		Raki_DX12B::Get()->GetDevice(),
		Raki_DX12B::Get()->GetGCommandList()
	);

	//生成したものを初期化
	pm->Initialize();

	return pm;
}

void ParticleManager::Initialize() {
	//nullチェック

	HRESULT result;

	//パイプライン初期化
	InitializeGraphicsPipeline();

	//パーティクル用モデル作成
	CreateModel();

	//定数バッファ生成
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	if (FAILED(result)) {
		assert(0);
	}
}

void ParticleManager::Update() {

	//寿命が切れたパーティクルを削除
	grains.remove_if([](Particle &p) {return p.nowFrame >= p.endFrame; });

	//全パーティクル更新
	for (std::forward_list<Particle>::iterator itr = grains.begin();
		itr != grains.end(); itr++) {

		//フレーム数カウント
		itr->nowFrame++;

		//進行度を0~1の割合に
		float rate = (float)itr->nowFrame / itr->endFrame;

		//速度加算
		itr->vel = itr->vel + itr->acc;

		//速度による移動
		itr->pos += itr->vel;

		//色線形補間
		itr->color = itr->s_color + (itr->e_color - itr->s_color) * rate;

		//スケーリングの線形補間
		itr->scale = itr->s_scale + (itr->e_scale - itr->s_scale) * rate;

		//回転線形補間
		itr->rot = itr->s_rotation + (itr->e_rotation - itr->s_rotation) / rate;
	}

	//頂点バッファデータ転送
	int vcount = 0;
	PVertex *vertMap = nullptr;
	result = vertbuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		// パーティクルの情報を1つずつ反映
		for (std::forward_list<Particle>::iterator it = grains.begin();
			it != grains.end();
			it++) {
			// 座標
			vertMap->pos = it->pos;
			// スケール
			vertMap->scale = it->scale;
			//色
			vertMap->color = it->color;
			// 次の頂点へ
			vertMap++;
			if (++vcount >= MAX_VERTEX) {
				break;
			}
		}
		vertbuff->Unmap(0, nullptr);
	}

	//定数バッファデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (result == S_OK) {
		//ビュープロジェクション行列
		constMap->mat = camera->GetMatrixViewProjection();
		//全方向ビルボード
		constMap->matBillBoard = camera->GetMatrixBillBoardAll();
		//色
		constBuff->Unmap(0, nullptr);
	}

}

void ParticleManager::Draw(UINT drawTexNum)
{
	UINT drawNum = (UINT)std::distance(grains.begin(), grains.end());
	if (drawNum > MAX_VERTEX) {
		drawNum = MAX_VERTEX;
	}

	// パーティクルが1つもない場合
	if (drawNum == 0) {
		return;
	}

	// パイプラインステートの設定
	cmd->SetPipelineState(pipeline.Get());
	// ルートシグネチャの設定
	cmd->SetGraphicsRootSignature(rootsig.Get());
	// プリミティブ形状を設定
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmd->IASetVertexBuffers(0, 1, &vbview);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
	cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmd->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			drawTexNum, Raki_DX12B::Get()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	// 描画コマンド
	cmd->DrawInstanced(drawNum, 1, 0, 0);
}

void ParticleManager::Add(int life, RVector3 pos, RVector3 vel, RVector3 acc, 
	float startScale, float endScale, XMFLOAT4 s_color, XMFLOAT4 e_color)
{
	//要素追加
	grains.emplace_front();
	//追加した要素の参照
	Particle &p = grains.front();
	p.pos = pos;			//初期位置
	p.vel = vel;			//速度
	p.acc = acc;			//加速度
	p.s_scale = startScale; //開始時のスケールサイズ
	p.e_scale = endScale;	//終了時のスケールサイズ
	p.endFrame = life;		//生存時間
	p.s_color = s_color;
	p.e_color = e_color;
}

void ParticleManager::Prototype_Set(ParticlePrototype *proto)
{
	prototype_ = proto;
}

void ParticleManager::Prototype_Add(int addCount, RVector3 startPos)
{
	for (int i = 0; i < addCount; i++) {
		//uniqueポインタで動的生成
		ParticlePrototype *newp = prototype_->clone(startPos);

		pplist.emplace_front(newp);
	}
}

void ParticleManager::Prototype_Update()
{
	////寿命切れパーティクルを消去
	//pplist.erase(std::find_if(pplist.begin(), pplist.end(),
	//	[](std::unique_ptr<ParticlePrototype> &p) {return p.get()->nowFrame >= p.get()->endFrame; }));
	//pplist.shrink_to_fit();
	////全パーティクル更新処理実行
	//for (std::vector<std::unique_ptr<ParticlePrototype>>::iterator itr = pplist.begin();
	//	itr != pplist.end(); itr++) {
	//	(*itr)->nowFrame++;
	//	(*itr)->Update();
	//}

   	std::erase_if(pplist, [](std::unique_ptr<ParticlePrototype> &p) {
		return p->nowFrame >= p->endFrame; });

	//pplist.remove_if([](std::unique_ptr<ParticlePrototype> &p) {
	//	return p->nowFrame >= p->endFrame;
	//	});

	//バッファデータ転送
	int vcount = 0;
	PVertex *vertMap = nullptr;
	result = vertbuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		// パーティクルの情報を1つずつ反映
		for (std::forward_list<std::unique_ptr<ParticlePrototype>>::iterator it = pplist.begin();
			it != pplist.end();
			it++) {
			(*it)->Update();
			// 座標
			vertMap->pos = (*it)->pos;
			// スケール
			vertMap->scale = (*it)->scale;
			//色
			vertMap->color = (*it)->color;
			// 次の頂点へ
			vertMap++;
			if (++vcount >= MAX_VERTEX) {
				break;
			}
		}
		vertbuff->Unmap(0, nullptr);
	}

	//定数バッファデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (result == S_OK) {
		//ビュープロジェクション行列
		constMap->mat = camera->GetMatrixViewProjection();
		//全方向ビルボード
		constMap->matBillBoard = camera->GetMatrixBillBoardAll();
		//色
		constBuff->Unmap(0, nullptr);
	}

}

void ParticleManager::Prototype_Draw(UINT drawTexNum)
{
	UINT drawNum = (UINT)std::distance(pplist.begin(), pplist.end());
	if (drawNum > MAX_VERTEX) {
		drawNum = MAX_VERTEX;
	}

	// パーティクルが1つもない場合
	if (drawNum == 0) {
		return;
	}

	// パイプラインステートの設定
	cmd->SetPipelineState(pipeline.Get());
	// ルートシグネチャの設定
	cmd->SetGraphicsRootSignature(rootsig.Get());
	// プリミティブ形状を設定
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmd->IASetVertexBuffers(0, 1, &vbview);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
	cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmd->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			drawTexNum, Raki_DX12B::Get()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	// 描画コマンド
	cmd->DrawInstanced(drawNum, 1, 0, 0);
}



void ParticleManager::InitializeGraphicsPipeline() {

	result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	// デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	// 加算ブレンディング
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用
	//// 減算ブレンディング
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（点）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsig));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = rootsig.Get();

	// グラフィックスパイプラインの生成
	result = Raki_DX12B::Get()->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline));

	if (FAILED(result)) {
		assert(0);
	}
}

void ParticleManager::CreateModel() {
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto res = CD3DX12_RESOURCE_DESC::Buffer(sizeof(PVertex) * MAX_VERTEX);
	result = dev->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&res,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertbuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	//// 頂点バッファビューの作成
	vbview.BufferLocation = vertbuff->GetGPUVirtualAddress();
	vbview.SizeInBytes = sizeof(PVertex) * MAX_VERTEX;
	vbview.StrideInBytes = sizeof(PVertex);


}
