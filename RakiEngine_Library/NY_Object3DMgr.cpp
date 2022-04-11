#include "NY_Object3DMgr.h"

#include "Raki_WinAPI.h"
#include "Raki_DX12B.h"
#include "TexManager.h"
#include "NY_Model.h"

ID3D12Device *NY_Object3DManager::dev = nullptr;

bool NY_Object3DManager::CreateObject3DManager()
{
    //Object3D用パイプライン生成
    object3dPipelineSet = Create3DPipelineState(RAKI_DX12B_DEV);

    //デバイスのポインタを格納
    this->dev = RAKI_DX12B_DEV;

    return true;
}


HRESULT NY_Object3DManager::QuickDrawShadersCompile()
{
    HRESULT result;//リザルト
    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト

    //標準ジオメトリシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"QuickDrawBoxGeometry.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &qd_BoxGS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);

        return result;
    }


    //ここまでシェーダーのコンパイル作業を実行
    //問題がなければ、ここでS_OKが帰る
    return result;
}


Pipeline3D NY_Object3DManager::Create3DPipelineState(ID3D12Device *dev)
{
    HRESULT result;

    //-----シェーダーの読み込みとコンパイル-----//
#pragma region LoadShader
    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト

    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJVertexShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &obj_VS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //標準ジオメトリシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJGeometryShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &obj_GS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //ピクセルシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJPixelShader.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &obj_PS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
#pragma endregion LoadShader

    //-----頂点レイアウト-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//法線
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv座標
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    //-----グラフィックスパイプライン設定-----//
#pragma region GraphicsPipeline
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    //頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダーをパイプラインに設定
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(obj_VS.Get());
    gpipeline.GS = CD3DX12_SHADER_BYTECODE(obj_GS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(obj_PS.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //ブレンドステートの設定
    D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    //ブレンドステートの共通設定
    blenddesc.BlendEnable = true;//ブレンド有効
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用

    //デプスステンシルステート設定
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //その他
    gpipeline.NumRenderTargets = 1;//描画対象は1つ
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //ルートパラメーターの設定
    //定数用
    CD3DX12_ROOT_PARAMETER rootparams[3] = {};
    rootparams[0].InitAsConstantBufferView(0);//定数バッファ用
    rootparams[1].InitAsConstantBufferView(1);
    //テクスチャ用
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);//標準

    //テクスチャサンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    Pipeline3D pipelineset;

    //ルートシグネチャの生成
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparams;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters = _countof(rootparams);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = pipelineset.rootsignature.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));
#pragma endregion GraphicsPipeline

    //デスクリプタヒープ生成
    //D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
    //descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //descHeapDesc.NumDescriptors = 256;
    //result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descheap));

    return pipelineset;
}


Object3d *NY_Object3DManager::CreateObject3d(Model3D *modelData)
{
    if (modelData == nullptr) {
        assert(modelData == nullptr);
    }

    //Object3dのデータを新たに作成
    Object3d *newobj = new Object3d;

    //生成したオブジェクトのデータを初期化
    newobj->InitObject3D(dev);

    //モデルデータをセット
    newobj->SetLoadedModelData(modelData);

    //初期化したオブジェクトをコンテナに格納
    objects.push_back(newobj);

    //生成したオブジェクトを返却
    return newobj;
}

Object3d *NY_Object3DManager::CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum)
{
    Object3d *newobj = new Object3d;

    newobj->InitObject3D(RAKI_DX12B_DEV);

    newobj->CreateModel_Tile(x_size, y_size, x_uv, y_uv, useTexNum);

    objects.push_back(newobj);

    return newobj;
}


void NY_Object3DManager::DeleteObject3d(Object3d *obj)
{
    for (int i = 0; i < objects.size(); i++) {
        //消すオブジェクトと同じオブジェクトを検出
        if (obj == objects[i]) {
            objects.erase(objects.begin() + i);
            break;
        }
    }

    objects.shrink_to_fit();

    delete obj;
    obj = nullptr;
}

void NY_Object3DManager::UpdateAllObjects()
{
    //すべてのオブジェクトを更新する
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->UpdateObject3D();
    }
}


void NY_Object3DManager::SetCommonBeginDrawObject3D()
{
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(object3dPipelineSet.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(object3dPipelineSet.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NY_Object3DManager::SetCommonBeginDrawObject3D2MultiPassRenderResource()
{
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(object3dPipelineSet.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(object3dPipelineSet.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap *ppHeaps[] = { RAKI_DX12B_GET->GetMuliPassSrvDescHeap() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

Object3d *CreateObject3d(Model3D *modelData, RVector3 pos)
{
    //返却用のポインタ変数にObject3Dを作成
    Object3d *result = NY_Object3DManager::Get()->CreateObject3d(modelData);

    //位置を設定
    result->SetAffineParamTranslate(pos);

    //生成物を返却
    return result;
}


Object3d *LoadModel_ObjFile(string modelname)
{
    //オブジェクトデータを新規作成
    Object3d *object = new Object3d;

    //オブジェクト初期化
    object->InitObject3D(RAKI_DX12B_DEV);

    //生成したオブジェクトのモデルデータをロードする
    object->LoadAndSetModelData(modelname);

    //返却
    return object;
}

void DrawObject3d(Object3d *obj)
{
    //描画準備
    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();
    //オブジェクト描画
    //obj->DrawModel3D(Raki_DX12B::Get()->GetGCommandList(), Raki_DX12B::Get()->GetDevice());
}
