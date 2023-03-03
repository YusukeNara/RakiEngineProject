#include "DifferrdRenderingMgr.h"
#include "NY_Camera.h"
#include "RenderTargetManager.h"
#include "DirectionalLight.h"

void DiferredRenderingMgr::Init(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd)
{
	m_dev = dev;
	m_cmd = cmd;

    DirectionalLight::SetLightPos(RVector3(500.f, 1000.f, 500.f),
        RVector3(0.f, 0.f, 0.f),
        RVector3(0.f, 1.f, 0.f));

    //DirectionalLight::SetLightDir(1.0f, -1.0f, 1.0f);

	ShaderCompile();

	CreateGraphicsPipeline();
}

void DiferredRenderingMgr::Rendering(RTex* gBuffer, RTex* shadowMap)
{
    UpdateConstBuff();

    //�f�B�t�@�[�h�����_�����O���s

    //�O���t�B�b�N�X�p�C�v���C�����Z�b�g
    m_cmd->SetPipelineState(m_pipelineState.Get());
    //���[�g�V�O�l�`�����Z�b�g
    m_cmd->SetGraphicsRootSignature(m_rootSignature.Get());
    //�v���~�e�B�u�`��ݒ�
    m_cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //�f�X�N���v�^�q�[�v�ݒ�
    ID3D12DescriptorHeap* ppHeaps[] = { gBuffer->GetDescriptorHeapSRV() };
    m_cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    //���_�o�b�t�@�ݒ�
    m_cmd->IASetVertexBuffers(0, 1, &m_vbview);
    //SRV�Z�b�g�i�v�Z����p�����[�^��������ƁA������������j
    m_cmd->SetGraphicsRootDescriptorTable(2,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(), 
            0, 
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    m_cmd->SetGraphicsRootDescriptorTable(3,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
            1,
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    m_cmd->SetGraphicsRootDescriptorTable(4,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
            2,
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    m_cmd->SetGraphicsRootDescriptorTable(5,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
            3,
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //�萔�o�b�t�@�ݒ�i�p�����[�^�[�𑝂₷���тɂ������m�F����j
    m_cmd->SetGraphicsRootConstantBufferView(0, m_constBuffEyePos->GetGPUVirtualAddress());
    m_cmd->SetGraphicsRootConstantBufferView(1, m_constBuffDirLight->GetGPUVirtualAddress());

    //�f�B�t�@�[�h�����_�����O���ʏo��
    m_cmd->DrawInstanced(6, 1, 0, 0);

    shadowMap->ClearRenderTarget();

   

    //�������p�Ƀf�v�X��gBuffer�ɁA�`�����o�b�N�o�b�t�@�ɂ���
    RenderTargetManager::GetInstance()->SetDSV(gBuffer);
}

void DiferredRenderingMgr::ShaderCompile()
{
    ComPtr<ID3DBlob> errorBlob = nullptr; //�G���[�I�u�W�F�N�g
    HRESULT result;

//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/Shaders/LIT_OBJVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
        nullptr,//�V�F�[�_�[�}�N���I�u�W�F�N�g�i����͎g��Ȃ��j
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�I�u�W�F�N�g�i�C���N���[�h�\�ɂ���j
        "main", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
        0,
        &m_litVS, &errorBlob
    );
    //�V�F�[�_�[�̃G���[���e��\��
    if (FAILED(result))
    {
        //errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //�G���[���e���o�̓E�C���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
    //�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/Shaders/LIT_OBJPixelShader.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &m_litPS, &errorBlob
    );
    //�V�F�[�_�[�̃G���[���e��\��
    if (FAILED(result))
    {
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //�G���[���e���o�̓E�C���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

}

void DiferredRenderingMgr::CreateGraphicsPipeline()
{
#pragma region VERTEX_INIT

    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resDesc  = CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 6);

    HRESULT result = m_dev->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertBuff)
    );

    VertexPosUv vertex[6]{
        {{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},//����
        {{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},//����
        {{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},//�E��
        {{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},
        {{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},
        {{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},//�E��
        
    };

    VertexPosUv* vertMap = nullptr;
    result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
    if (SUCCEEDED(result)) {
        memcpy(vertMap, vertex, sizeof(vertex));
        m_vertBuff->Unmap(0, nullptr);
    }

    m_vbview.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
    m_vbview.SizeInBytes    = sizeof(VertexPosUv) * 6;
    m_vbview.StrideInBytes  = sizeof(VertexPosUv);

    //�萔�o�b�t�@����
    auto cbuffResdDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(cbuffer_b0) + 0xff) & ~0xff);
    m_dev->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &cbuffResdDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constBuffEyePos)
    );

    //�萔�o�b�t�@�f�[�^�]��
    cbuffer_b0 *ConstMapB0 = nullptr;
    result = m_constBuffEyePos->Map(0, nullptr, (void**)&ConstMapB0);
    if (SUCCEEDED(result)) {
        ConstMapB0->eyePos = camera->GetEye();
        m_constBuffEyePos->Unmap(0, nullptr);
    }

    cbuffResdDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(cbuffer_b1) + 0xff) & ~0xff);
    m_dev->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &cbuffResdDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constBuffDirLight)
    );

    //�萔�o�b�t�@�f�[�^�]��
    cbuffer_b1* ConstMapB1 = nullptr;
    result = m_constBuffDirLight->Map(0, nullptr, (void**)&ConstMapB1);
    if (SUCCEEDED(result)) {
        ConstMapB1->lightDir = DirectionalLight::GetLightDir();
        m_constBuffDirLight->Unmap(0, nullptr);
    }

#pragma endregion VERTEX_INIT

    //-----���_���C�A�E�g-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz���W
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv���W
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};

    //���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[���p�C�v���C���ɐݒ�
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(m_litVS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(m_litPS.Get());

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    gpipeline.SampleMask        = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
    gpipeline.RasterizerState   = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //�u�����h�X�e�[�g�̐ݒ�
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddesc�������������RenderTarget[0]�����������
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�W���ݒ�

    //�u�����h�X�e�[�g�̋��ʐݒ�
    blenddesc.BlendEnable       = true;//�u�����h�L��
    blenddesc.BlendOpAlpha      = D3D12_BLEND_OP_ADD;//���Z����
    blenddesc.SrcBlendAlpha     = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g�p
    blenddesc.DestBlendAlpha    = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g�p

    //�����ݒ�(�e���ڂ����������邱�ƂŐݒ�\)
    blenddesc.BlendOp           = D3D12_BLEND_OP_ADD;//���Z
    blenddesc.SrcBlend          = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̒l��100%�g�p
    blenddesc.DestBlend         = D3D12_BLEND_INV_SRC_ALPHA;//�f�X�g�̒l��100%�g�p

    //�f�v�X�X�e���V���X�e�[�g�ݒ�
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat         = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

    //���_���C�A�E�g�̐ݒ�
    gpipeline.InputLayout.pInputElementDescs    = inputLayout;
    gpipeline.InputLayout.NumElements           = _countof(inputLayout);

    //�}�`�̌`����O�p�`�ɐݒ�
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //���̑�
    gpipeline.NumRenderTargets      = 1;//�`��Ώۂ�1��
    gpipeline.RTVFormats[0]         = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255�w���RGBA
    gpipeline.SampleDesc.Count      = 1;//1px�ɂ�1��T���v�����O

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_0{};
    descRangeSRV_0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_1{};
    descRangeSRV_1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_2{};
    descRangeSRV_2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_3{};
    descRangeSRV_3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);


    //���[�g�p�����[�^�[�̐ݒ�
    CD3DX12_ROOT_PARAMETER rootparams[6] = {};
    rootparams[0].InitAsConstantBufferView(0);//b0 �X�y�L�����p���_���W
    rootparams[1].InitAsConstantBufferView(1);
    //GBuffer�e�N�X�`���p�i�萔�o�b�t�@�����C�g�z�������̂Ɏg���\�肾���A����͂Ȃ��j
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV_0, D3D12_SHADER_VISIBILITY_ALL);//�A���x�h�e�N�X�`��
    rootparams[3].InitAsDescriptorTable(1, &descRangeSRV_1, D3D12_SHADER_VISIBILITY_ALL);//�@���e�N�X�`��
    rootparams[4].InitAsDescriptorTable(1, &descRangeSRV_2, D3D12_SHADER_VISIBILITY_ALL);//���[���h���W�e�N�X�`��
    rootparams[5].InitAsDescriptorTable(1, &descRangeSRV_3, D3D12_SHADER_VISIBILITY_ALL);//�[�x���e�N�X�`��
    //�萔�o�b�t�@


    //�e�N�X�`���T���v���[�ݒ�
    D3D12_STATIC_SAMPLER_DESC samplerDesc   = {};
    samplerDesc.AddressU                    = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV                    = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW                    = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor                 = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter                      = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD                      = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD                      = 0.0f;
    samplerDesc.ComparisonFunc              = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility            = D3D12_SHADER_VISIBILITY_PIXEL;

    //���[�g�V�O�l�`���̐���
    ComPtr<ID3DBlob> errorBlob          = nullptr; //�G���[�I�u�W�F�N�g
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc = {};
    rootsignatureDesc.Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters       = rootparams;//���[�g�p�����[�^�[�̐擪�A�h���X
    rootsignatureDesc.NumParameters     = _countof(rootparams);//���[�g�p�����[�^��
    rootsignatureDesc.pStaticSamplers   = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob        = nullptr;

    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = m_dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

    //�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    gpipeline.pRootSignature = m_rootSignature.Get();

    //�p�C�v���C���X�e�[�g����
    result = m_dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_pipelineState));

}

void DiferredRenderingMgr::UpdateConstBuff()
{
    //�萔�o�b�t�@�f�[�^�]��
    cbuffer_b0* ConstMapB0 = nullptr;
    HRESULT result = m_constBuffEyePos->Map(0, nullptr, (void**)&ConstMapB0);
    if (SUCCEEDED(result)) {
        ConstMapB0->eyePos = { 500.f,500.f,500.f };
        m_constBuffEyePos->Unmap(0, nullptr);
    }

    //�萔�o�b�t�@�f�[�^�]��
    cbuffer_b1* ConstMapB1 = nullptr;
    result = m_constBuffDirLight->Map(0, nullptr, (void**)&ConstMapB1);
    if (SUCCEEDED(result)) {
        ConstMapB1->lightDir = DirectionalLight::GetLightDir();
        m_constBuffDirLight->Unmap(0, nullptr);
    }
}
