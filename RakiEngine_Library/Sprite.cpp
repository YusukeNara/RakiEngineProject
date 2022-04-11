#include "Sprite.h"
#include "NY_Camera.h"

#include "TexManager.h"

#include "Raki_DX12B.h"

void Sprite::CreateSprite(XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag, uvAnimData *animData)
{
	HRESULT result;

    //���_����K���ɍ쐬
    if (animData != nullptr) {
        //this->animData = animData;
        ////���_�f�[�^
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
        //�������k���Ȃ�k���𒼐ړ����
        this->animData = nullptr;
        //���_�f�[�^
        SpriteVertex vertices = {
            {0.0f,0.0f,0.0f},{0.0f,0.0f},
        };

        spdata.vertice = vertices;
    }

    //���\�[�XID�ݒ�
    spdata.texNumber = resourceID;

    //�A���J�[�|�C���g�̃R�s�[
    spdata.anchorPoint = anchor;

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(SpriteVertex) * 1);

	//���_�o�b�t�@����
    D3D12_HEAP_PROPERTIES heapprop{}; //�q�[�v�ݒ�
    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
    D3D12_RESOURCE_DESC resdesc{}; //���\�[�X�ݒ�
    resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resdesc.Width = sizeVB; //���_�f�[�^�S�̂̃T�C�Y
    resdesc.Height = 1;
    resdesc.DepthOrArraySize = 1;
    resdesc.MipLevels = 1;
    resdesc.SampleDesc.Count = 1;
    resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    //���_�o�b�t�@�̐���
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &heapprop, //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resdesc, //���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertBuff));

    //-----���_�o�b�t�@�ւ̃f�[�^�]��-----//
    SpriteVertex *vertMap = nullptr;
    result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //�S���_�ɑ΂���
    vertMap = &spdata.vertice;//���W���R�s�[
    //�}�b�v������
    spdata.vertBuff->Unmap(0, nullptr);

    //���_�o�b�t�@�r���[����
    spdata.vbView.BufferLocation = spdata.vertBuff->GetGPUVirtualAddress();
    spdata.vbView.SizeInBytes = sizeof(spdata.vertice);
    spdata.vbView.StrideInBytes = sizeof(SpriteVertex);

    //�C���X�^���V���O�p���_�o�b�t�@����

    //�������p
    SpriteInstance spins[] = {
        {XMMatrixIdentity()},
    };

    sizeInsVB = static_cast<UINT>(sizeof(SpriteInstance) * 32);

    auto INS_HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC INS_RESDESC{};
    INS_RESDESC.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    INS_RESDESC.Width = sizeInsVB; //���_�f�[�^�S�̂̃T�C�Y
    INS_RESDESC.Height = 1;
    INS_RESDESC.DepthOrArraySize = 1;
    INS_RESDESC.MipLevels = 1;
    INS_RESDESC.SampleDesc.Count = 1;
    INS_RESDESC.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //���_�o�b�t�@�̐���
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &INS_HEAP_PROP, //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &INS_RESDESC, //���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertInsBuff));

    //�f�[�^�]��
    SpriteInstance *insmap = nullptr;
    result = spdata.vertInsBuff->Map(0, nullptr, (void **)&insmap);
    for (int i = 0; i < _countof(spins); i++) {
        insmap[i].worldmat = spins[i].worldmat * camera->GetMatrixViewProjection();
    }
    spdata.vertInsBuff->Unmap(0, nullptr);

    //�r���[�쐬
    spdata.vibView.BufferLocation = spdata.vertInsBuff->GetGPUVirtualAddress();
    spdata.vibView.SizeInBytes = sizeof(spins);
    spdata.vibView.StrideInBytes = sizeof(SpriteInstance);


    auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpConstBufferData) + 0xff) & ~0xff);
    //�萔�o�b�t�@����
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &HEAP_PROP,
        D3D12_HEAP_FLAG_NONE,
        &RESDESC,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&spdata.constBuff)
    );

    //�萔�o�b�t�@�f�[�^�]��
    SpConstBufferData *constMap = nullptr;
    result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);//�F�w��
    //���s���e�s��
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height, 0.0f, 0.0f, 1.0f);
    spdata.constBuff->Unmap(0, nullptr);

    //���\�[�X�ɍ��킹�Ē�������ꍇ
    if (adjustResourceFlag == true)
    {
        //�e�N�X�`�����擾
        D3D12_RESOURCE_DESC resDesc = TexManager::textureData[resourceID].texBuff->GetDesc();
        //���\�[�X�ɍ��킹�ăT�C�Y����
        spdata.size = { (float)resDesc.Width,(float)resDesc.Height };
    }
    else//���Ȃ��ꍇ
    {
        spdata.size = size;//�����̃T�C�Y�ɐݒ�
    }

    ResizeSprite(spdata.size);

}

void Sprite::Create(UINT resourceID, float sizeX, float sizeY)
{
    HRESULT result;

    if (animData != nullptr) {
        //this->animData = animData;
        ////���_�f�[�^
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
        //�������k���Ȃ�k���𒼐ړ����
        this->animData = nullptr;
        //���_�f�[�^
        SpriteVertex vertices = {
            {0.0f,0.0f,0.0f},{0.0f,0.0f},
        };

        spdata.vertice = vertices;
    }

    //�e�N�X�`���ݒ�
    spdata.texNumber = resourceID;

    //���_�f�[�^�ƃC���f�b�N�X�f�[�^�𐶐����čX�V

    //���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    UINT sizeVB = static_cast<UINT>(sizeof(SpriteVertex) * 1);
    //���_�o�b�t�@����
    D3D12_HEAP_PROPERTIES heapprop{}; //�q�[�v�ݒ�
    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
    D3D12_RESOURCE_DESC resdesc{}; //���\�[�X�ݒ�
    resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resdesc.Width = sizeVB; //���_�f�[�^�S�̂̃T�C�Y
    resdesc.Height = 1;
    resdesc.DepthOrArraySize = 1;
    resdesc.MipLevels = 1;
    resdesc.SampleDesc.Count = 1;
    resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //���_�o�b�t�@�̐���
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &heapprop, //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resdesc, //���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertBuff));
    //-----���_�o�b�t�@�ւ̃f�[�^�]��-----//
    SpriteVertex *vertMap = nullptr;
    result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //�S���_�ɑ΂���
    vertMap = &spdata.vertice;//���W���R�s�[
    //�}�b�v������
    spdata.vertBuff->Unmap(0, nullptr);
    //���_�o�b�t�@�r���[����
    spdata.vbView.BufferLocation = spdata.vertBuff->GetGPUVirtualAddress();
    spdata.vbView.SizeInBytes = sizeof(spdata.vertice);
    spdata.vbView.StrideInBytes = sizeof(SpriteVertex);

    //�傫���ύX
    ResizeSprite({ sizeX,sizeY });

    //�C���X�^���V���O�p���_�o�b�t�@����
    //�������p
    SpriteInstance spins[] = {
        {XMMatrixIdentity()},
    };
    sizeInsVB = static_cast<UINT>(sizeof(SpriteInstance) * 8);
    auto INS_HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC INS_RESDESC{};
    INS_RESDESC.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    INS_RESDESC.Width = sizeInsVB; //���_�f�[�^�S�̂̃T�C�Y
    INS_RESDESC.Height = 1;
    INS_RESDESC.DepthOrArraySize = 1;
    INS_RESDESC.MipLevels = 1;
    INS_RESDESC.SampleDesc.Count = 1;
    INS_RESDESC.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //���_�o�b�t�@�̐���
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &INS_HEAP_PROP, //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &INS_RESDESC, //���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertInsBuff));
    //�f�[�^�]��
    SpriteInstance *insmap = nullptr;
    result = spdata.vertInsBuff->Map(0, nullptr, (void **)&insmap);
    for (int i = 0; i < _countof(spins); i++) {
        insmap[i].worldmat = spins[i].worldmat * camera->GetMatrixProjection();
    }
    spdata.vertInsBuff->Unmap(0, nullptr);
    //�r���[�쐬
    spdata.vibView.BufferLocation = spdata.vertInsBuff->GetGPUVirtualAddress();
    spdata.vibView.SizeInBytes = sizeof(spins);
    spdata.vibView.StrideInBytes = sizeof(SpriteInstance);
    auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpConstBufferData) + 0xff) & ~0xff);

    //�萔�o�b�t�@����
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &HEAP_PROP,
        D3D12_HEAP_FLAG_NONE,
        &RESDESC,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&spdata.constBuff)
    );
    //�萔�o�b�t�@�f�[�^�]��
    SpConstBufferData *constMap = nullptr;
    result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);//�F�w��
    //���s���e�s��
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height, 0.0f, 0.0f, 1.0f);
    spdata.constBuff->Unmap(0, nullptr);

}

void Sprite::ResizeSprite(XMFLOAT2 newsize)
{
    //HRESULT result;

    //spdata.size = newsize;

    ////�A���J�[�|�C���g�ɍ��킹���ݒ�
    //float left   = (0.0f - spdata.anchorPoint.x) * spdata.size.x;
    //float right  = (1.0f - spdata.anchorPoint.x) * spdata.size.x;
    //float top    = (0.0f - spdata.anchorPoint.y) * spdata.size.y;
    //float bottom = (1.0f - spdata.anchorPoint.y) * spdata.size.y;

    //spdata.vertices[0].pos = { left  ,   top,0.0f };
    //spdata.vertices[1].pos = { right ,   top,0.0f };
    //spdata.vertices[2].pos = { left  ,bottom,0.0f };
    //spdata.vertices[3].pos = { right ,bottom,0.0f };

    ////���_�o�b�t�@�]��
    //SpriteVertex *vertMap = nullptr;
    //result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    ////�S���_�ɑ΂���
    //memcpy(vertMap, spdata.vertices, sizeof(spdata.vertices));
    ////�}�b�v������
    //spdata.vertBuff->Unmap(0, nullptr);

}

void Sprite::UpdateSprite()
{
    //�A�j���[�V�����X�V
    //if (animData != nullptr) {
    //    spdata.vertices[0].uv = animData->GetOffset().offsetLB; //����
    //    spdata.vertices[1].uv = animData->GetOffset().offsetRB; //����
    //    spdata.vertices[2].uv = animData->GetOffset().offsetLT; //����
    //    spdata.vertices[3].uv = animData->GetOffset().offsetRT; //����
    //    //���_�o�b�t�@�f�[�^�]��
    //    SpriteVertex *vertMap = nullptr;
    //    auto result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //    //�S���_�ɑ΂���
    //    memcpy(vertMap, spdata.vertices, sizeof(spdata.vertices));
    //    //�}�b�v������
    //    spdata.vertBuff->Unmap(0, nullptr);
    //}

    spdata.matWorld = XMMatrixIdentity();

    spdata.matWorld *= XMMatrixRotationZ(XMConvertToRadians(spdata.rotation));

    spdata.matWorld *= XMMatrixTranslation(spdata.position.x, spdata.position.y, spdata.position.z);

    //�萔�o�b�t�@�]��
    SpConstBufferData *constMap = nullptr;
    HRESULT result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->mat = spdata.matWorld * camera->GetMatrixProjection();
    constMap->color = spdata.color;
    spdata.constBuff->Unmap(0, nullptr);

}

void Sprite::InstanceUpdate()
{
    //�`�搔�ɍ��킹�ĉς�����
    spdata.vibView.SizeInBytes = spdata.insWorldMatrixes.size() * sizeof(SpriteInstance);
    
    //�C���X�^���V���O���_�o�b�t�@�̃T�C�Y��ύX����K�v������ꍇ
    if (isVertexBufferNeedResize()) {

        ResizeVertexInstanceBuffer(spdata.insWorldMatrixes.size() * sizeof(SpriteInstance));
    }

    //�o�b�t�@�f�[�^�]��
    SpriteInstance *insmap = nullptr;
    auto result = spdata.vertInsBuff->Map(0, nullptr, (void **)&insmap);
    for (int i = 0; i < spdata.insWorldMatrixes.size(); i++) {
        insmap[i].worldmat = spdata.insWorldMatrixes[i] * XMMatrixOrthographicOffCenterLH(0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height, 0.0f, 0.0f, 1.0f);
    }
    spdata.vertInsBuff->Unmap(0, nullptr);
}

void Sprite::Draw()
{
    //�C���X�^���V���O�f�[�^�X�V
    InstanceUpdate();

    //���_�o�b�t�@�Z�b�g
    D3D12_VERTEX_BUFFER_VIEW vbviews[] = {
        spdata.vbView,spdata.vibView
    };
    SpriteManager::Get()->cmd->IASetVertexBuffers(0, _countof(vbviews), vbviews);
    //�萔�o�b�t�@�Z�b�g
    SpriteManager::Get()->cmd->SetGraphicsRootConstantBufferView(0, spdata.constBuff->GetGPUVirtualAddress());
    //�V�F�[�_�[���\�[�X�r���[���Z�b�g
    SpriteManager::Get()->cmd->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap->GetGPUDescriptorHandleForHeapStart(),
        spdata.texNumber, SpriteManager::Get()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //�`��
    SpriteManager::Get()->cmd->DrawInstanced(4, (UINT)spdata.insWorldMatrixes.size(), 0, 0);

    //�C���X�^���X�f�[�^���N���A���A�R���e�i���Z�b�g
    spdata.insWorldMatrixes.clear();
    spdata.insWorldMatrixes.shrink_to_fit();
}

void Sprite::DrawSprite(float posX, float posY)
{
    //���W�����Ƃɕ��s�ړ��s��쐬
    XMMATRIX trans = XMMatrixTranslation(posX, posY, 0);
    //��]�A�X�P�[�����O�͂Ȃ�
    XMMATRIX norot = XMMatrixRotationZ(XMConvertToRadians(0.0f));

    //�s��R���e�i�Ɋi�[
    XMMATRIX world = XMMatrixIdentity();
    world *= norot;
    world *= trans;
    spdata.insWorldMatrixes.push_back(world);
}

void Sprite::DrawExtendSprite(float x1, float y1, float x2, float y2)
{
    //���W�����Ƃɕ��s�ړ��s����쐬
    XMMATRIX trans = XMMatrixTranslation(x1, y1, 0);
    //��]�A�X�P�[�����O�͂Ȃ�
    XMMATRIX norot = XMMatrixRotationZ(XMConvertToRadians(0.0f));
    XMMATRIX noScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

    //�T�C�Y��ύX
    spdata.size.x = x2 - x1;
    spdata.size.y = y2 - y1;
    ResizeSprite(spdata.size);

    //�s��R���e�i�Ɋi�[
    spdata.insWorldMatrixes.push_back(noScale * norot * trans);
}

void Sprite::DrawMPRender()
{
    SpriteManager::Get()->SetCommonBeginDrawmpResource();
    //���_�o�b�t�@�Z�b�g
    SpriteManager::Get()->cmd->IASetVertexBuffers(0, 1, &spdata.vbView);
    //�萔�o�b�t�@�Z�b�g
    SpriteManager::Get()->cmd->SetGraphicsRootConstantBufferView(0, spdata.constBuff->GetGPUVirtualAddress());
    //�V�F�[�_�[���\�[�X�r���[���Z�b�g
    SpriteManager::Get()->cmd->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(RAKI_DX12B_GET->GetMuliPassSrvDescHeap()->GetGPUDescriptorHandleForHeapStart(),
            0, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //�`��
    SpriteManager::Get()->cmd->DrawInstanced(4, 1, 0, 0);
}

bool Sprite::isVertexBufferNeedResize()
{
    return spdata.vertInsBuff.Get()->GetDesc().Width < spdata.insWorldMatrixes.size() * sizeof(SpriteInstance);
}

void Sprite::ResizeVertexInstanceBuffer(UINT newWidthSize)
{
    //���_�o�b�t�@�̐ݒ�
    auto INS_HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC INS_RESDESC{};
    INS_RESDESC.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    INS_RESDESC.Width = newWidthSize; //���_�f�[�^�S�̂̃T�C�Y
    INS_RESDESC.Height = 1;
    INS_RESDESC.DepthOrArraySize = 1;
    INS_RESDESC.MipLevels = 1;
    INS_RESDESC.SampleDesc.Count = 1;
    INS_RESDESC.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    //���_�o�b�t�@�̍Đ���
    auto result = SpriteManager::Get()->dev->CreateCommittedResource(
        &INS_HEAP_PROP, //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &INS_RESDESC, //���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertInsBuff));

    spdata.vibView.BufferLocation = spdata.vertInsBuff.Get()->GetGPUVirtualAddress();
    spdata.vibView.SizeInBytes = newWidthSize;

}
