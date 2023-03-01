#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <memory>
#include <DirectXTex.h>
#include <array>

struct RenderTextureOption
{
	DXGI_FORMAT format;
	float clearColor[4];
};

//�����_�[�e�N�X�`���ꖇ�̃f�[�^
class RenderTextureData
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�f�t�H���g
	RenderTextureData() = default;
	~RenderTextureData() = default;
/// <summary>
	/// �����_�[�e�N�X�`��������
/// </summary>
	/// <param name="texwidth">�����_�[�e�N�X�`������</param>
	/// <param name="texheight">�����_�[�e�N�X�`���c��</param>
	/// <param name="clearColor">�N���A�J���[</param>
	/// <param name="addBufferNums">Rtex�ꖇ�ɐ�������o�b�t�@�̐�</param>
	void Init(int texwidth, int texheight, float* clearColor, int addBufferNums, RenderTextureOption option[]);

private:
	//�e�N�X�`���o�b�t�@�ivector�j
	std::vector<ComPtr<ID3D12Resource>>		rtexBuff;
	//�e�N�X�`���p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>			srvHeap	  = nullptr;
	//�����_�[�^�[�Q�b�g�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>			rtvHeap	  = nullptr;
	//�f�v�X�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>			dsvHeap   = nullptr;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource>		depthBuff;

	//�e�N�X�`���o�b�t�@����
	void CreateTextureBuffer(int texture_width, int texture_height, float* clearColor, int addBufferNums, RenderTextureOption option[]);

	//SRV�f�X�N���v�^�q�[�v����
	void CreateSRVDescriptorHeap(int bufferCount,RenderTextureOption option[]);

	//RTV�f�X�N���v�^�q�[�v�쐬
	void CreateRTVDescriptorHeap(int bufferCount);

	//�[�x�o�b�t�@�����i�摜�T�C�Y�j
	void CreateDepthBuffer(int texture_width, int texture_height);

	//DSV�f�X�N���v�^�q�[�v�쐬
	void CreateDSVDescriptorHeap(int bufferCount);

	friend class RTex;
	friend class RenderTargetManager;
};

class RTex
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�r���[�|�[�g
	CD3DX12_VIEWPORT	viewport;
	//�V�U�[��`�i�����_�����O���ʂ��擾����͈́j
	CD3DX12_RECT		rect;
	//�N���A�J���[
	std::array<float, 4> clearColors;

	//�R���X�g���N�^
	RTex();
	//�f�X�g���N�^
	~RTex();

	/// <summary>
	/// �����_�[�e�N�X�`������
	/// </summary>
	/// <param name="texture_width">�e�N�X�`����</param>
	/// <param name="texture_height">�e�N�X�`������</param>
	/// <param name="clearColor">�N���A�J���[</param>
	/// <param name="bufferCount">�e�N�X�`��������</param>
	/// <param name="depthCount">�[�x�e�N�X�`��������</param>
	/// <param name="option">�����_�[�e�N�X�`��1��������̐ݒ�</param>
	void CreateRTex(int texture_width, int texture_height, float* clearColor, int bufferCount, RenderTextureOption* option = nullptr);

	//�����_�[�e�N�X�`���̃f�[�^�擾
	const RenderTextureData* GetRTData() { return rtdata.get(); }

	ID3D12Resource *GetTextureBuffer(int arrayNum = 0) { return rtdata->rtexBuff[arrayNum].Get(); }

	ID3D12Resource* GetDepthBuffer() { return rtdata->depthBuff.Get(); }

	ID3D12DescriptorHeap* GetDescriptorHeapRTV() { return rtdata->rtvHeap.Get(); }

	ID3D12DescriptorHeap* GetDescriptorHeapDSV() { return rtdata->dsvHeap.Get(); }

	ID3D12DescriptorHeap* GetDescriptorHeapSRV() { return rtdata->srvHeap.Get(); }

	void ClearRenderTarget();

	friend class RenderTargetManager;

private:
	//�����_�[�e�N�X�`���f�[�^�|�C���^
	std::shared_ptr<RenderTextureData> rtdata;

	//�摜�T�C�Y�i�擾����ɂ�metadeta���Q�Ƃ��Ȃ��Ƃ����Ȃ��̂ŕۑ����Ă����j
	std::pair<int, int> graph_size;

	//�r���[�|�[�g�ƃV�U�[��`������
	void InitViewAndRect(int width, int height);

};

