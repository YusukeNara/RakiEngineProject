#include "RTex.h"
#include "Raki_DX12B.h"

void RenderTextureData::Init(int texwidth, int texheight,float *clearColor)
{
	//�e�탊�\�[�X����
	CreateTextureBuffer(texwidth, texheight,clearColor);
	CreateSRVDescriptorHeap();
	CreateRTVDescriptorHeap();
	CreateDepthBuffer(texwidth, texheight);
	CreateDSVDescriptorHeap();
}

void RenderTextureData::CreateTextureBuffer(int texture_width, int texture_height,float *clearColor)
{
	auto hp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	auto resdesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		static_cast<UINT>(texture_width),
		static_cast<UINT>(texture_height),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);
	auto clearvalue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);

	HRESULT RenderTexture_Create_Result = 
		RAKI_DX12B_DEV->CreateCommittedResource(
		&hp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearvalue,
		IID_PPV_ARGS(&rtexBuff)
	);

	//�������s���͏I��
	assert(SUCCEEDED(RenderTexture_Create_Result));
}

void RenderTextureData::CreateSRVDescriptorHeap()
{
	HRESULT result;

	//�e�N�X�`���p�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC texdhd = {};
	texdhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	texdhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	texdhd.NumDescriptors = 1;

	//�e�N�X�`���p�f�X�N���v�^�q�[�v
	result = RAKI_DX12B_DEV->CreateDescriptorHeap(&texdhd, IID_PPV_ARGS(&srvHeap));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	RAKI_DX12B_DEV->CreateShaderResourceView(rtexBuff.Get(),
		&srvDesc,
		srvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void RenderTextureData::CreateRTVDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvdhd = {};
	rtvdhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvdhd.NumDescriptors = 1;

	HRESULT result = RAKI_DX12B_DEV->CreateDescriptorHeap(
		&rtvdhd, IID_PPV_ARGS(&rtvHeap)
	);

	RAKI_DX12B_DEV->CreateRenderTargetView(rtexBuff.Get(),
		nullptr,
		rtvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void RenderTextureData::CreateDepthBuffer(int texture_width, int texture_height)
{
	//�摜�T�C�Y�ɍ��킹�ăo�b�t�@�𐶐�����
	CD3DX12_RESOURCE_DESC depthresdesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			texture_width,
			texture_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto clearvalue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	//�[�x�o�b�t�@����
	HRESULT rtex_depthbuff_create_result = RAKI_DX12B_DEV->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearvalue,
		IID_PPV_ARGS(&depthBuff)
	);

	//�������s�͋����I��
	assert(SUCCEEDED(rtex_depthbuff_create_result));
}

void RenderTextureData::CreateDSVDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC depthheapdesc{};
	depthheapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	depthheapdesc.NumDescriptors = 1;

	auto rtex_dsv_descheap_create_result = RAKI_DX12B_DEV->CreateDescriptorHeap(
		&depthheapdesc,
		IID_PPV_ARGS(&dsvHeap)
	);
	assert(SUCCEEDED(rtex_dsv_descheap_create_result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {  };
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	RAKI_DX12B_DEV->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

RTex::RTex()
{
	RenderTextureData* tmp = new RenderTextureData;
	rtdata.reset(tmp);
}

RTex::~RTex()
{

}

void RTex::CreateRTex(int texture_width, int texture_height, float* clearColor)
{
	//�����_�[�e�N�X�`���f�[�^������
	rtdata->Init(texture_width, texture_height, clearColor);

	//�r���[�|�[�g�A�V�U�[��`������
	InitViewAndRect(texture_width, texture_height);

	graph_size.first = texture_width;
	graph_size.second = texture_height;
}

void RTex::InitViewAndRect(int width, int height)
{
	//�r���[�|�[�g�̓E�B���h�E�T�C�Y�ɍ��킹��
	auto temp = CD3DX12_VIEWPORT(0.0f, 0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height);
	viewport = temp;

	//�V�U�[��`�͉�ʍ���
	rect = CD3DX12_RECT(0, 0, width, height);

}
