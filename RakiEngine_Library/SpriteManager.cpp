#include "SpriteManager.h"
#include "TexManager.h"

#include "Raki_DX12B.h"

void SpriteManager::CreateSpriteManager(ID3D12Device *dev, ID3D12GraphicsCommandList *cmd, int window_w, int window_h)
{
    //デバイスとコマンドリストのポインタを格納
    this->dev = dev;
    this->cmd = cmd;
    //パイプライン生成
    CreateSpritePipeline();
}

void SpriteManager::CreateSpritePipeline()
{
    HRESULT result;

    //-----シェーダーの読み込みとコンパイル-----//
#pragma region LoadShader
    ComPtr<ID3DBlob> vsBlob = nullptr; //頂点シェーダーオブジェクト
    ComPtr<ID3DBlob> psBlob = nullptr; //ピクセルシェーダーオブジェクト
    ComPtr<ID3DBlob> gsBlob = nullptr;
    ID3DBlob *errorBlob = nullptr; //エラーオブジェクト

    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/SpriteVS.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &vsBlob, &errorBlob
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
        L"Resources/Shaders/SpritePS.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &psBlob, &errorBlob
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

    //ジオメトリシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/SpriteGS.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "gs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &gsBlob, &errorBlob
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
        //頂点データ
        {//頂点座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv座標
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },

        //インスタンスデータ
        {//行列
            "INSTANCE_WORLD_MAT",0 ,DXGI_FORMAT_R32G32B32A32_FLOAT,1,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1
        },
        {//行列
            "INSTANCE_WORLD_MAT",1U,DXGI_FORMAT_R32G32B32A32_FLOAT,1,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1
        },
        {//行列
            "INSTANCE_WORLD_MAT",2U,DXGI_FORMAT_R32G32B32A32_FLOAT,1,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1
        },
        {//行列
            "INSTANCE_WORLD_MAT",3U,DXGI_FORMAT_R32G32B32A32_FLOAT,1,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1
        },
        {//縦横幅
            "INSTANCE_DRAWSIZE" ,0,DXGI_FORMAT_R32G32_FLOAT,       1,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1
        },
    };

    

    //-----グラフィックスパイプラインのセット-----//

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    //頂点シェーダー、ピクセルシェーダーをパイプラインに設定
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
    gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;


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
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//一度表示設定
    gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書き
    gpipeline.DepthStencilState.DepthEnable = false;//深度テスト無効
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

    //その他
    gpipeline.NumRenderTargets = 1;//描画対象は1つ
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER rootparam[2];
    rootparam[0].InitAsConstantBufferView(0);
    rootparam[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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

    //ルートシグネチャの生成
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparam;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters = _countof(rootparam);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    if (result != S_OK) { cout << "ERROR : ENGINE : SPRITE : ROOTSIGNATURE" << endl; }
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
    if (result != S_OK) { cout << "ERROR : ENGINE : SPRITE : ROOTSIGNATURE" << endl; }
    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = rootsignature.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

    //マルチパス用グラフィックスパイプライン

#pragma region mpPipeline

    //シェーダーコンパイル
    ComPtr<ID3DBlob> mpPsBlob;
    //ピクセルシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/mpEffectPixelShader.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &mpPsBlob, &errorBlob
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
    
    ////-----グラフィックスパイプラインのセット-----//

    //D3D12_GRAPHICS_PIPELINE_STATE_DESC mpps{};
    ////頂点シェーダー、ピクセルシェーダーをパイプラインに設定
    //mpps.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
    //mpps.PS = CD3DX12_SHADER_BYTECODE(mpPsBlob.Get());

    ////サンプルマスクとラスタライザステートの設定
    //mpps.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    //mpps.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    //mpps.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

    ////ブレンドステートの設定
    //D3D12_RENDER_TARGET_BLEND_DESC &bdesc = mpps.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    //blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    ////ブレンドステートの共通設定
    //bdesc.BlendEnable = true;//ブレンド有効
    //bdesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    //bdesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    //bdesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使用

    ////合成設定(各項目を書き換えることで設定可能)
    //bdesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    //bdesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    //bdesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用

    ////デプスステンシルステート設定
    //mpps.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//一度表示設定
    //mpps.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書き
    //mpps.DepthStencilState.DepthEnable = false;//深度テスト無効
    //mpps.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    ////頂点レイアウトの設定
    //mpps.InputLayout.pInputElementDescs = inputLayout;
    //mpps.InputLayout.NumElements = _countof(inputLayout);

    ////図形の形状を三角形に設定
    //mpps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    ////その他
    //mpps.NumRenderTargets = 1;//描画対象は1つ
    //mpps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    //mpps.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    //CD3DX12_DESCRIPTOR_RANGE dRSRV{};
    //dRSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //CD3DX12_ROOT_PARAMETER rparam[2];
    //rparam[0].InitAsConstantBufferView(0);
    //rparam[1].InitAsDescriptorTable(1, &dRSRV, D3D12_SHADER_VISIBILITY_ALL);

    ////テクスチャサンプラー設定
    //D3D12_STATIC_SAMPLER_DESC sDesc{};

    //sDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    //sDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    //sDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    //sDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    //sDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    //sDesc.MaxLOD = D3D12_FLOAT32_MAX;
    //sDesc.MinLOD = 0.0f;
    //sDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    //sDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    ////ルートシグネチャの生成
    //D3D12_ROOT_SIGNATURE_DESC rootsigDesc{};
    //rootsigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    //rootsigDesc.pParameters = rootparam;//ルートパラメーターの先頭アドレス
    //rootsigDesc.NumParameters = _countof(rootparam);//ルートパラメータ数
    //rootsigDesc.pStaticSamplers = &sDesc;
    //rootsigDesc.NumStaticSamplers = 1;

    //ComPtr<ID3DBlob> rsBlob = nullptr;
    //result = D3D12SerializeRootSignature(&rootsigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob);
    //result = dev->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(&mpRootsig));

    ////パイプラインにルートシグネチャをセット
    //mpps.pRootSignature = mpRootsig.Get();

    ////パイプラインステート生成
    //result = dev->CreateGraphicsPipelineState(&mpps, IID_PPV_ARGS(&mpPipeline));


#pragma endregion mpPipeline

    gpipeline.PS = CD3DX12_SHADER_BYTECODE(mpPsBlob.Get());

    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&mpRootsig));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = mpRootsig.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&mpPipeline));
}

void SpriteManager::SetCommonBeginDraw()
{
    //パイプラインステートをセット
    cmd->SetPipelineState(pipelinestate.Get());
    //ルートシグネチャをセット
    cmd->SetGraphicsRootSignature(rootsignature.Get());
    //プリミティブ形状設定
    cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
    cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void SpriteManager::SetCommonBeginDrawmpResource()
{
    //パイプラインステートセット
    cmd->SetPipelineState(mpPipeline.Get());
    //ルートシグネチャをセット
    cmd->SetGraphicsRootSignature(mpRootsig.Get());
    //プリミティブ形状設定
    cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap *ppHeaps[] = { RAKI_DX12B_GET->GetMuliPassSrvDescHeap() };
    cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

}
