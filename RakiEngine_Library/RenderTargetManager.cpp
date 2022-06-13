#include "RenderTargetManager.h"
#include "Raki_WinAPI.h"
#include "Raki_DX12B.h"

RenderTargetManager::RenderTargetManager()
{

}

RenderTargetManager::~RenderTargetManager()
{

}

void RenderTargetManager::InitRenderTargetManager(ID3D12Device* device, ID3D12GraphicsCommandList* cmd)
{
	//�f�o�C�X�ƃR�}���h��ݒ�
	dev		= device;
	cmdlist = cmd;

	CreateSwapChain();

	CreateDepthBuffer();

	CreateBackBuffers();

	//�r���[�|�[�g�ƃV�U�[��`�ݒ�
	auto temp = CD3DX12_VIEWPORT(0.0f, 0.0f, Raki_WinAPI::window_width, Raki_WinAPI::window_height);
	default_viewport = temp;
	default_rect = CD3DX12_RECT(0, 0, Raki_WinAPI::window_width, Raki_WinAPI::window_height);
}

void RenderTargetManager::FinalizeRenderTargetManager()
{
	//�R���e�i����
	renderTextures.clear();
	renderTextures.shrink_to_fit();
}

void RenderTargetManager::CrearAndStartDraw()
{
	//���ׂẴ����_�[�e�N�X�`�����N���A���邽�߂ɁA��x�����_�[�e�N�X�`����ݒ肷��
	//for (int i = 0; i < renderTextures.size(); i++) {
	//	SetRenderTarget(i);
	//	ClearRenderTarget(renderTextures[i]->GetDescriptorHeapRTV());
	//	ClearDepthBuffer(renderTextures[i]->GetDescriptorHeapDSV());
	//	CloseDrawRenderTexture();
	//}

	//�����_�[�e�N�X�`���̓��[�U�[���C�ӂ̃^�C�~���O�ŃN���A����B

	//�o�b�N�o�b�t�@�̔ԍ��擾
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//�����_�[�^�[�Q�b�g�ɕύX
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdlist->ResourceBarrier(1, &barrier);

	//�f�X�N���v�^�q�[�v�ݒ�
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//�����_�[�^�[�Q�b�g�ɐݒ�
	cmdlist->OMSetRenderTargets(1, &rtvh, false, &dsvh);

	//��ʃN���A
	ClearBackBuffer();

	//�r���[�|�[�g�A�V�U�[��`�ݒ�
	cmdlist->RSSetViewports(1, &default_viewport);
	cmdlist->RSSetScissorRects(1, &default_rect);

	isDrawing = USING_BACKBUFFER;

	//ImGui�̕`�揀��
	ImguiMgr::Get()->NewFrame();
}

int RenderTargetManager::CreateRenderTexture(int width, int height)
{
	//�V������郌���_�[�e�N�X�`��
	RTex* returnData = new RTex;

	//�e�N�X�`���f�[�^����
	returnData->CreateRTex(width, height, clearcolor);

	//�����_�[�e�N�X�`���R���e�i�Ƀf�[�^�i�[
	renderTextures.emplace_back().reset(returnData);
	//�����̃C�e���[�^�[�擾
	std::vector<std::shared_ptr<RTex>>::iterator itr = renderTextures.end();
	itr--;
	//distance���g�p���A�C���f�b�N�X�ԍ����擾
	size_t indexnum = std::distance(renderTextures.begin(), itr);

	//�L���X�g���āA�������������_�[�e�N�X�`���̃n���h����ԋp
	return static_cast<int>(indexnum);
}

void RenderTargetManager::SetRenderTarget(int handle)
{
	//�n���h���̃G���[�ɑΏ�

	//���łɃ����_�[�^�[�Q�b�g�̃n���h��
	if (handle == nowRenderTargetHandle && isDrawing != USING_BACKBUFFER) {
		return;
	}

	//���̒l�A�܂��͔͈͊O�Q�Ƃ͑������^�[��
	if (handle < 0 || handle > renderTextures.size()) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Render texture handle is out of range" << std::endl;
		return;
	}

	//��̃n���h���͑������^�[��
	if (renderTextures[handle] == nullptr) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Pointing to an empty handle." << std::endl;
		SetDrawBackBuffer();
		return;
	}

	//�Y���e�N�X�`���������_�[�^�[�Q�b�g�ɐݒ肷�鏈��

	//�O�̃����_�[�^�[�Q�b�g�̏I������
	if(isDrawing != USING_BACKBUFFER){ CloseDrawRenderTexture(); }

	//�e�N�X�`���̃��\�[�X�X�e�[�g�������_�[�^�[�Q�b�g�ɕύX
	auto barrierState = CD3DX12_RESOURCE_BARRIER::Transition(
		renderTextures[handle]->rtdata->rtexBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	cmdlist->ResourceBarrier(1, &barrierState);
	
	//�f�X�N���v�^�q�[�v�ݒ�
	D3D12_CPU_DESCRIPTOR_HANDLE rtvh = renderTextures[handle]->GetDescriptorHeapRTV()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvh = renderTextures[handle]->GetDescriptorHeapDSV()->GetCPUDescriptorHandleForHeapStart();

	//�����_�[�^�[�Q�b�g�ݒ�
	cmdlist->OMSetRenderTargets(1, &rtvh, false, &dsvh);

	//�r���[�|�[�g�A�V�U�[��`�ݒ�
	cmdlist->RSSetViewports(1, &renderTextures[handle]->viewport);
	cmdlist->RSSetScissorRects(1, &renderTextures[handle]->rect);

	isDrawing = USING_RENDERTEXTURE;
	nowRenderTargetHandle = handle;
}

void RenderTargetManager::SetRenderTargetDrawArea(int handle, int x1, int y1, int x2, int y2)
{
	//�`�F�b�N
	if (isNullHandle(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Pointing to an empty handle." << std::endl;
		return;
	}
	if (isHandleOutOfRange(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Render texture handle is out of range" << std::endl;
		return;
	}

	//�摜�T�C�Y���傫���ꍇ�̓T�C�Y�ɗ}����
	int temp_sizeX = renderTextures[handle]->graph_size.first;
	int temp_sizeY = renderTextures[handle]->graph_size.second;

	if (temp_sizeX < x2 - x1) { x2 = x1 + temp_sizeX; }
	if (temp_sizeY < y2 - y1) { y2 = y1 + temp_sizeY; }

	renderTextures[handle]->rect = CD3DX12_RECT(x1, y1, x2, y2);
}

void RenderTargetManager::SetRenderTargetClipingArea(int handle, int x1, int y1, int x2, int y2)
{
	//�`�F�b�N
	if (isNullHandle(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Pointing to an empty handle." << std::endl;
		return;
	}
	if (isHandleOutOfRange(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Render texture handle is out of range" << std::endl;
		return;
	}



	renderTextures[handle]->viewport = CD3DX12_VIEWPORT(x1, y1, x2, y2);
}

void RenderTargetManager::SetClearColor(float red, float green, float blue)
{
	clearcolor[0] = red;
	clearcolor[1] = green;
	clearcolor[2] = blue;
}

void RenderTargetManager::ClearRenderTexture(int handle)
{
	int handletmp = 0;
	if (handle != nowRenderTargetHandle && isDrawing != USING_BACKBUFFER) {
		handletmp = nowRenderTargetHandle;
		SetRenderTarget(handle);
		ClearRenderTarget(renderTextures[handle]->GetDescriptorHeapRTV());
		ClearDepthBuffer(renderTextures[handle]->GetDescriptorHeapDSV());
		CloseDrawRenderTexture();
		
		SetRenderTarget(handletmp);
	}
	else {
		ClearRenderTarget(renderTextures[nowRenderTargetHandle]->GetDescriptorHeapRTV());
		ClearDepthBuffer(renderTextures[nowRenderTargetHandle]->GetDescriptorHeapDSV());
	}

}

void RenderTargetManager::SetDrawBackBuffer()
{
	//�����_�[�e�N�X�`���̏�Ԃ�\����Ԃ�
	if (isDrawing == USING_BACKBUFFER) { return; }
	
	CloseDrawRenderTexture();

	//�o�b�N�o�b�t�@�̔ԍ��擾
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//�f�X�N���v�^�q�[�v�ݒ�
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//�����_�[�^�[�Q�b�g�ɐݒ�
	cmdlist->OMSetRenderTargets(1, &rtvh, false, &dsvh);

	//�r���[�|�[�g�A�V�U�[��`�ݒ�
	cmdlist->RSSetViewports(1, &default_viewport);
	cmdlist->RSSetScissorRects(1, &default_rect);

	isDrawing = USING_BACKBUFFER;
}

void RenderTargetManager::SwapChainBufferFlip()
{
	//Imgui�`����s
	ImguiMgr::Get()->SendImguiDrawCommand();

	//�o�b�N�o�b�t�@���N���[�Y
	CloseDrawBackBuffer();

	//�`��I���R�}���h
	Raki_DX12B::Get()->CloseDraw(swapchain.Get());
}

void RenderTargetManager::CreateSwapChain()
{
	HRESULT result = S_FALSE;

	// �e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = Raki_WinAPI::window_width;
	swapchainDesc.Height = Raki_WinAPI::window_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �F���̏�������ʓI�Ȃ��̂�
	swapchainDesc.SampleDesc.Count = 1;                 // �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	// �o�b�N�o�b�t�@�Ƃ��Ďg����悤��
	swapchainDesc.BufferCount = 2;	                    // �o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // �t���b�v��͑��₩�ɔj��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // �t���X�N���[���؂�ւ�������
	ComPtr<IDXGISwapChain1> swapchain1;
	result = Raki_DX12B::Get()->GetDXGIFactory()->CreateSwapChainForHwnd(
		Raki_DX12B::Get()->GetCmdQueue(),
		Raki_WinAPI::GetHWND(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);
	if (FAILED(result)) {
		assert(0);
	}
	swapchain1.Get()->Present(0, 0);
	swapchain1.As(&swapchain);

}

void RenderTargetManager::CreateBackBuffers()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result)) {
		assert(0);
	}

	// �e��ݒ�����ăf�B�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// �����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));
	if (FAILED(result)) {
		assert(0);
	}

	// ���\�̂Q���ɂ���
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		if (FAILED(result)) {
			assert(0);
		}

		// �f�B�X�N���v�^�q�[�v�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			i, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// �����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}

}

void RenderTargetManager::CreateDepthBuffer()
{
	//�E�B���h�E�T�C�Y�ɍ��킹�ăo�b�t�@�𐶐�����
	CD3DX12_RESOURCE_DESC depthresdesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			Raki_WinAPI::window_width,
			Raki_WinAPI::window_height,
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

	//�[�x�o�b�t�@����f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC depthheapdesc{};
	depthheapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	depthheapdesc.NumDescriptors = 1;

	auto rtex_dsv_descheap_create_result = RAKI_DX12B_DEV->CreateDescriptorHeap(
		&depthheapdesc,
		IID_PPV_ARGS(&dsvHeap)
	);
	assert(SUCCEEDED(rtex_dsv_descheap_create_result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	RAKI_DX12B_DEV->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void RenderTargetManager::ClearDepthBuffer(ID3D12DescriptorHeap* dsv)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsv->GetCPUDescriptorHandleForHeapStart());
	cmdlist->ClearDepthStencilView(dsvh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void RenderTargetManager::ClearRenderTarget(ID3D12DescriptorHeap* rtv)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtv->GetCPUDescriptorHandleForHeapStart());
	cmdlist->ClearRenderTargetView(rtvh, clearcolor, 0, nullptr);
}

void RenderTargetManager::ClearBackBuffer()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	cmdlist->ClearDepthStencilView(dsvh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);
	cmdlist->ClearRenderTargetView(rtvh, clearcolor, 0, nullptr);
}

void RenderTargetManager::CloseDrawBackBuffer()
{
	// �o�b�N�o�b�t�@�̃��\�[�X�o���A��ύX�i�`��Ώہ��\����ԁj
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	auto barrier_temp = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
	
	//���\�[�X�o���A
	cmdlist->ResourceBarrier(1, &barrier_temp);
}

void RenderTargetManager::CloseDrawRenderTexture()
{

	//���݂̃����_�[�e�N�X�`����\����Ԃ�
	auto resourceBattier = CD3DX12_RESOURCE_BARRIER::Transition(
		renderTextures[nowRenderTargetHandle]->GetTextureBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	RAKI_DX12B_CMD->ResourceBarrier(1, &resourceBattier);
}

void RenderTargetManager::BufferFlip()
{
	//�X���b�v�`�F�[�����t���b�v
	swapchain->Present(1, 0);

}
