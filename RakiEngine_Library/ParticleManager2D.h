#pragma once
#include "Sprite.h"
#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>
#include <memory>
#include "RVector.h"

//�p�[�e�B�N���p�����[�^�[
struct ParticleGrainState2D {
private:
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:

	//�N�_
	RVector3 position;
	//���x
	RVector3 vel;
	//���x���
	RVector3 velMax;
	//���x����
	RVector3 velMin;
	//�����x
	RVector3 acc;

	//�F�֘A
	//�F�⊮�J�n�_
	XMFLOAT4 color_start;
	//�F�⊮�I���_
	XMFLOAT4 color_end;

	//�A�t�B���ϊ�
	//�X�P�[�����O
	float scale_start;
	float scale_end;
	//��]
	float rot_start;
	float rot_end;
	//����
	int aliveTime;


	//�����_���X�|�[���t���O
	bool isRandomSpawn;
	//�X�|�[���͈�
	RVector3 position_spawnRange1;
	RVector3 position_spawnRange2;
};

//�p�[�e�B�N���ꗱ�̃N���X
struct Grain2d
{
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	//���W
	RVector3	pos		= { 0,0,0 };
	//���x
	RVector3	vel		= { 0,0,0 };
	//�����x
	RVector3	acc		= { 0,0,0 };
	//�F
	XMFLOAT4	color	= { 1,1,1,1 };
	//�X�P�[�����O
	float		scale	= 1.0f;
	//�T�C�Y
	XMFLOAT2	drawsize = { 16.0f,16.0f };
	//��](2d�Ȃ̂�z��)
	float		rot		= 0.0f;
	//�����l
	XMFLOAT4	s_color		= { 1,1,1,1 };
	float		s_scale		= 1.0f;
	float		s_rotation	= 0.0f;
	// �ŏI�l
	XMFLOAT4	e_color		= { 1,1,1,1 };
	float		e_scale		= 0.0f;
	float		e_rotation	= 0.0f;
	// ���݃t���[��
	int nowFrame = 0;
	// �I���t���[��
	int endFrame = 0;
};

//
class ParticlePrototype2D : public Grain2d
{
public:
	ParticlePrototype2D() {};
	~ParticlePrototype2D() {};
	//������
	virtual void Init() = 0;
	//�X�V
	virtual void Update() = 0;
	//�N���[�������i�N�_���W�����擾����? ���[�U��`�ł����������j
	virtual ParticlePrototype2D* clone() = 0;

	//�`��A�I���͕K�v�Ȃ��i���ۂ̕`��̓G�~�b�^�[�ł��̂Łj
};

class ParticleManager2D
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	ParticleManager2D();
	~ParticleManager2D();

	void Initialize(UINT graphHandle);

	void Add(ParticleGrainState2D &pgState2d);

	void Update();

	void Draw();

	void Prototype_Set(ParticlePrototype2D* ins);

	void Prototype_Add();

	void Prototype_Update();

	void Prototype_Draw();

	ParticlePrototype2D* GetPrototypeInstance() { return _prototype.get(); }

private:
	//3D�ƈقȂ�A�O���t�B�b�N�p�C�v���C���Ȃǂ̓X�v���C�g�̂��̂��g���̂Ńi�V

	//�X�v���C�g�f�[�^
	Sprite _psprite;
	//�p�[�e�B�N���v���g�^�C�v�|�C���^
	std::unique_ptr<ParticlePrototype2D> _prototype;
	//�p�[�e�B�N�����R���e�i
	std::vector<std::unique_ptr<ParticlePrototype2D>> _pplist;

	//�ʏ�p�[�e�B�N���|�C���^
	std::vector<std::unique_ptr<Grain2d>> _grains;

};

