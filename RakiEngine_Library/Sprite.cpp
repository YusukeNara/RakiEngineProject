#include "Sprite.h"
#include "NY_Camera.h"

#include "TexManager.h"

#include "Raki_DX12B.h"

void Sprite::CreateSprite(XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag, uvAnimData *animData)
{
	HRESULT result;

    //頂点情報を適当に作成
    if (animData != nullptr) {
        //this->animData = animData;
        ////頂点データ
        //SpriteVertex vertices[] = {
        //    {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetLT},
        //    {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetRT},
        //    {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetLB},
        //    {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetRB},
        //};

        //spdata.vertices[0] = vertices[0];
        //spdata.vertices[1] = vertices[1];
        //spdata.vertices[2] = vertices[2];
        //spdata.vertices[3] = vertices[3];
    }
    else {
        //引数がヌルならヌルを直接入れる
        this->animData = nullptr;
        //頂点データ
        SpriteVertex vertices = {
            {0.0f,0.0f,0.0f},{0.0f,0.0f},
        };

        spdata.vertice = vertices;
    }

    //リソースID設定
    spdata.texNumber = resourceID;

    //アンカーポイントのコピー
    spdata.anchorPoint = anchor;

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(SpriteVertex) * 1);

	//頂点バッファ生成
    D3D12_HEAP_PROPERTIES heapprop{}; //ヒープ設定
    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用
    D3D12_RESOURCE_DESC resdesc{}; //リソース設定
    resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resdesc.Width = sizeVB; //頂点データ全体のサイズ
    resdesc.Height = 1;
    resdesc.DepthOrArraySize = 1;
    resdesc.MipLevels = 1;
    resdesc.SampleDesc.Count = 1;
    resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    //頂点バッファの生成
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &heapprop, //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resdesc, //リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertBuff));

    //-----頂点バッファへのデータ転送-----//
    SpriteVertex *vertMap = nullptr;
    result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //全頂点に対して
    vertMap = &spdata.vertice;//座標をコピー
    //マップを解除
    spdata.vertBuff->Unmap(0, nullptr);

    //頂点バッファビュー生成
    spdata.vbView.BufferLocation = spdata.vertBuff->GetGPUVirtualAddress();
    spdata.vbView.SizeInBytes = sizeof(spdata.vertice);
    spdata.vbView.StrideInBytes = sizeof(SpriteVertex);

    //インスタンシング用頂点バッファ生成

    //初期化用
    SpriteInstance spins[] = {
        {XMMatrixIdentity()},
    };

    sizeInsVB = static_cast<UINT>(sizeof(SpriteInstance) * 32);

    auto INS_HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC INS_RESDESC{};
    INS_RESDESC.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    INS_RESDESC.Width = sizeInsVB; //頂点データ全体のサイズ
    INS_RESDESC.Height = 1;
    INS_RESDESC.DepthOrArraySize = 1;
    INS_RESDESC.MipLevels = 1;
    INS_RESDESC.SampleDesc.Count = 1;
    INS_RESDESC.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //頂点バッファの生成
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &INS_HEAP_PROP, //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &INS_RESDESC, //リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertInsBuff));

    //データ転送
    SpriteInstance *insmap = nullptr;
    result = spdata.vertInsBuff->Map(0, nullptr, (void **)&insmap);
    for (int i = 0; i < _countof(spins); i++) {
        insmap[i].worldmat = spins[i].worldmat * camera->GetMatrixViewProjection();
    }
    spdata.vertInsBuff->Unmap(0, nullptr);

    //ビュー作成
    spdata.vibView.BufferLocation = spdata.vertInsBuff->GetGPUVirtualAddress();
    spdata.vibView.SizeInBytes = sizeof(spins);
    spdata.vibView.StrideInBytes = sizeof(SpriteInstance);


    auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpConstBufferData) + 0xff) & ~0xff);
    //定数バッファ生成
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &HEAP_PROP,
        D3D12_HEAP_FLAG_NONE,
        &RESDESC,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&spdata.constBuff)
    );

    //定数バッファデータ転送
    SpConstBufferData *constMap = nullptr;
    result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);//色指定
    //平行投影行列
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height, 0.0f, 0.0f, 1.0f);
    spdata.constBuff->Unmap(0, nullptr);

    //リソースに合わせて調整する場合
    if (adjustResourceFlag == true)
    {
        //テクスチャ情報取得
        D3D12_RESOURCE_DESC resDesc = TexManager::textureData[resourceID].texBuff->GetDesc();
        //リソースに合わせてサイズ調整
        spdata.size = { (float)resDesc.Width,(float)resDesc.Height };
    }
    else//しない場合
    {
        spdata.size = size;//引数のサイズに設定
    }

    ResizeSprite(spdata.size);

}

void Sprite::Create(UINT resourceID)
{
    HRESULT result;

    //引数がヌルならヌルを直接入れる
    this->animData = nullptr;
    //頂点データ
    SpriteVertex vertices = {
        {0.0f,0.0f,0.0f},{0.0f,0.0f},
    };

    spdata.vertice = vertices;

    //テクスチャ設定
    spdata.texNumber = resourceID;

    //頂点データとインデックスデータを生成して更新

    //頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    UINT sizeVB = static_cast<UINT>(sizeof(SpriteVertex) * 2);
    //頂点バッファ生成
    D3D12_HEAP_PROPERTIES heapprop{}; //ヒープ設定
    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用
    D3D12_RESOURCE_DESC resdesc{}; //リソース設定
    resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resdesc.Width = sizeVB; //頂点データ全体のサイズ
    resdesc.Height = 1;
    resdesc.DepthOrArraySize = 1;
    resdesc.MipLevels = 1;
    resdesc.SampleDesc.Count = 1;
    resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //頂点バッファの生成
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &heapprop, //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resdesc, //リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertBuff));
    //-----頂点バッファへのデータ転送-----//
    SpriteVertex *vertMap = nullptr;
    result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //全頂点に対して
    vertMap = &spdata.vertice;//座標をコピー
    //マップを解除
    spdata.vertBuff->Unmap(0, nullptr);
    //頂点バッファビュー生成
    spdata.vbView.BufferLocation = spdata.vertBuff->GetGPUVirtualAddress();
    spdata.vbView.SizeInBytes = sizeof(spdata.vertice);
    spdata.vbView.StrideInBytes = sizeof(SpriteVertex);

    ////大きさ変更
    //ResizeSprite({ sizeX,sizeY });

    //インスタンシング用頂点バッファ生成
    //初期化用
    SpriteInstance spins[] = {
        {XMMatrixIdentity()},
    };
    sizeInsVB = static_cast<UINT>(sizeof(SpriteInstance) * 8);
    auto INS_HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC INS_RESDESC{};
    INS_RESDESC.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    INS_RESDESC.Width = sizeInsVB; //頂点データ全体のサイズ
    INS_RESDESC.Height = 1;
    INS_RESDESC.DepthOrArraySize = 1;
    INS_RESDESC.MipLevels = 1;
    INS_RESDESC.SampleDesc.Count = 1;
    INS_RESDESC.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //頂点バッファの生成
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &INS_HEAP_PROP, //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &INS_RESDESC, //リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertInsBuff));
    //データ転送
    SpriteInstance *insmap = nullptr;
    result = spdata.vertInsBuff->Map(0, nullptr, (void **)&insmap);
    for (int i = 0; i < _countof(spins); i++) {
        insmap[i].worldmat = spins[i].worldmat * camera->GetMatrixProjection();
    }
    spdata.vertInsBuff->Unmap(0, nullptr);
    //ビュー作成
    spdata.vibView.BufferLocation = spdata.vertInsBuff->GetGPUVirtualAddress();
    spdata.vibView.SizeInBytes = sizeof(spins);
    spdata.vibView.StrideInBytes = sizeof(SpriteInstance);
    auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpConstBufferData) + 0xff) & ~0xff);

    //定数バッファ生成
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &HEAP_PROP,
        D3D12_HEAP_FLAG_NONE,
        &RESDESC,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&spdata.constBuff)
    );
    //定数バッファデータ転送
    SpConstBufferData *constMap = nullptr;
    result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);//色指定
    //平行投影行列
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height, 0.0f, 0.0f, 1.0f);
    spdata.constBuff->Unmap(0, nullptr);

    //テクスチャのデフォルトサイズを取得
    TEXTURE_DEFAULT_SIZE.x = TexManager::textureData[resourceID].metaData.width;
    TEXTURE_DEFAULT_SIZE.y = TexManager::textureData[resourceID].metaData.height;

}

void Sprite::ResizeSprite(XMFLOAT2 newsize)
{
    //HRESULT result;

    //spdata.size = newsize;

    ////アンカーポイントに合わせた設定
    //float left   = (0.0f - spdata.anchorPoint.x) * spdata.size.x;
    //float right  = (1.0f - spdata.anchorPoint.x) * spdata.size.x;
    //float top    = (0.0f - spdata.anchorPoint.y) * spdata.size.y;
    //float bottom = (1.0f - spdata.anchorPoint.y) * spdata.size.y;

    //spdata.vertices[0].pos = { left  ,   top,0.0f };
    //spdata.vertices[1].pos = { right ,   top,0.0f };
    //spdata.vertices[2].pos = { left  ,bottom,0.0f };
    //spdata.vertices[3].pos = { right ,bottom,0.0f };

    ////頂点バッファ転送
    //SpriteVertex *vertMap = nullptr;
    //result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    ////全頂点に対して
    //memcpy(vertMap, spdata.vertices, sizeof(spdata.vertices));
    ////マップを解除
    //spdata.vertBuff->Unmap(0, nullptr);

}

void Sprite::UpdateSprite()
{
    //アニメーション更新
    //if (animData != nullptr) {
    //    spdata.vertices[0].uv = animData->GetOffset().offsetLB; //左上
    //    spdata.vertices[1].uv = animData->GetOffset().offsetRB; //左上
    //    spdata.vertices[2].uv = animData->GetOffset().offsetLT; //左上
    //    spdata.vertices[3].uv = animData->GetOffset().offsetRT; //左上
    //    //頂点バッファデータ転送
    //    SpriteVertex *vertMap = nullptr;
    //    auto result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //    //全頂点に対して
    //    memcpy(vertMap, spdata.vertices, sizeof(spdata.vertices));
    //    //マップを解除
    //    spdata.vertBuff->Unmap(0, nullptr);
    //}

    spdata.matWorld = XMMatrixIdentity();

    spdata.matWorld *= XMMatrixRotationZ(XMConvertToRadians(spdata.rotation));

    spdata.matWorld *= XMMatrixTranslation(spdata.position.x, spdata.position.y, spdata.position.z);

    //定数バッファ転送
    SpConstBufferData *constMap = nullptr;
    HRESULT result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->mat = spdata.matWorld * camera->GetMatrixProjection();
    constMap->color = spdata.color;
    spdata.constBuff->Unmap(0, nullptr);

}

void Sprite::InstanceUpdate()
{
    //描画数に合わせて可変させる
    spdata.vibView.SizeInBytes = spdata.insWorldMatrixes.size() * sizeof(SpriteInstance);
    
    //インスタンシング頂点バッファのサイズを変更する必要がある場合
    if (isVertexBufferNeedResize()) {

        ResizeVertexInstanceBuffer(spdata.insWorldMatrixes.size() * sizeof(SpriteInstance));
    }

    //バッファデータ転送
    SpriteInstance *insmap = nullptr;
    auto result = spdata.vertInsBuff->Map(0, nullptr, (void **)&insmap);
    for (int i = 0; i < spdata.insWorldMatrixes.size(); i++) {
        insmap[i].worldmat = spdata.insWorldMatrixes[i].worldmat * XMMatrixOrthographicOffCenterLH(0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height, 0.0f, 0.0f, 1.0f);
        insmap[i].drawsize = spdata.insWorldMatrixes[i].drawsize;
    }
    spdata.vertInsBuff->Unmap(0, nullptr);
}

void Sprite::Draw()
{
    //インスタンシングデータ更新
    InstanceUpdate();

    //頂点バッファセット
    D3D12_VERTEX_BUFFER_VIEW vbviews[] = {
        spdata.vbView,spdata.vibView
    };
    SpriteManager::Get()->cmd->IASetVertexBuffers(0, _countof(vbviews), vbviews);
    //定数バッファセット
    SpriteManager::Get()->cmd->SetGraphicsRootConstantBufferView(0, spdata.constBuff->GetGPUVirtualAddress());
    //シェーダーリソースビューをセット
    SpriteManager::Get()->cmd->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap->GetGPUDescriptorHandleForHeapStart(),
        spdata.texNumber, SpriteManager::Get()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //描画
    SpriteManager::Get()->cmd->DrawInstanced(1, (UINT)spdata.insWorldMatrixes.size(), 0, 0);

    //インスタンスデータをクリアし、コンテナリセット
    spdata.insWorldMatrixes.clear();
    spdata.insWorldMatrixes.shrink_to_fit();
}

void Sprite::DrawSprite(float posX, float posY)
{
    //座標をもとに平行移動行列作成
    XMMATRIX trans = XMMatrixTranslation(posX, posY, 0);
    //回転、スケーリングはなし
    XMMATRIX norot = XMMatrixRotationZ(XMConvertToRadians(0.0f));

    //

    //行列コンテナに格納
    SpriteInstance ins = {};
    ins.worldmat = XMMatrixIdentity();
    ins.worldmat *= norot;
    ins.worldmat *= trans;
    //デフォルトサイズを格納
    ins.drawsize = TEXTURE_DEFAULT_SIZE;
    spdata.insWorldMatrixes.push_back(ins);
}

void Sprite::DrawExtendSprite(float x1, float y1, float x2, float y2)
{
    //座標をもとに平行移動行列を作成
    XMMATRIX trans = XMMatrixTranslation(x1, y1, 0);
    //回転、スケーリングはなし
    XMMATRIX norot = XMMatrixRotationZ(XMConvertToRadians(0.0f));
    XMMATRIX noScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

    //サイズを変更
    //spdata.size.x = x2 - x1;
    //spdata.size.y = y2 - y1;
    //ResizeSprite(spdata.size);

    //行列コンテナに格納
    SpriteInstance ins = {};

    ins.worldmat = XMMatrixIdentity();
    ins.worldmat *= norot;
    ins.worldmat *= trans;
    ins.drawsize = { x2 - x1, y2 - y1 };
    //行列コンテナに格納
    spdata.insWorldMatrixes.push_back(ins);
}

void Sprite::DrawMPRender()
{
    SpriteManager::Get()->SetCommonBeginDrawmpResource();
    //頂点バッファセット
    SpriteManager::Get()->cmd->IASetVertexBuffers(0, 1, &spdata.vbView);
    //定数バッファセット
    SpriteManager::Get()->cmd->SetGraphicsRootConstantBufferView(0, spdata.constBuff->GetGPUVirtualAddress());
    //シェーダーリソースビューをセット
    SpriteManager::Get()->cmd->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(RAKI_DX12B_GET->GetMuliPassSrvDescHeap()->GetGPUDescriptorHandleForHeapStart(),
            0, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //描画
    SpriteManager::Get()->cmd->DrawInstanced(4, 1, 0, 0);
}

bool Sprite::isVertexBufferNeedResize()
{
    return spdata.vertInsBuff.Get()->GetDesc().Width < spdata.insWorldMatrixes.size() * sizeof(SpriteInstance);
}

void Sprite::ResizeVertexInstanceBuffer(UINT newWidthSize)
{
    //頂点バッファの設定
    auto INS_HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC INS_RESDESC{};
    INS_RESDESC.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    INS_RESDESC.Width = newWidthSize; //頂点データ全体のサイズ
    INS_RESDESC.Height = 1;
    INS_RESDESC.DepthOrArraySize = 1;
    INS_RESDESC.MipLevels = 1;
    INS_RESDESC.SampleDesc.Count = 1;
    INS_RESDESC.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    //頂点バッファの再生成
    auto result = SpriteManager::Get()->dev->CreateCommittedResource(
        &INS_HEAP_PROP, //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &INS_RESDESC, //リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertInsBuff));

    spdata.vibView.BufferLocation = spdata.vertInsBuff.Get()->GetGPUVirtualAddress();
    spdata.vibView.SizeInBytes = newWidthSize;

}
