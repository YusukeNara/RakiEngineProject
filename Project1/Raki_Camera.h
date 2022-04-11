#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// <para> �����C�N�ŃJ�����A�r���[�s�񂩂����Ă���</para>
/// <para> �J�����ɒǏ]�Ȃǂ̋@�\����������ꍇ�́A�p����p���ċ@�\���������邱��</para>
/// </summary>
class Raki_BaseCamera
{
private:
	// ---------- �v���C�x�[�g�����o�ϐ� ----------//

	//�r���[�s��
	XMMATRIX matView;
	//��]�s��
	XMMATRIX matViewRot;
	//��]�����x�N�g��
	XMVECTOR camAxisX, camAxisY, camAxisZ;

public:
	// ---------- �R���X�g���N�^�A�f�X�g���N�^ ---------- //
	Raki_BaseCamera() {
		//�r���[�\���v�f������
		XMFLOAT3 ie{ 0,0,0 }, it{ 0,0,camRange }, iu{ 0,1,0 };
		eyeVec    = XMLoadFloat3(&ie);
		targetVec = XMLoadFloat3(&it);
		upVec     = XMLoadFloat3(&iu);
		//��]�p������
		XMFLOAT3 rad{ 0,0,0 };
		camRad = XMLoadFloat3(&rad);
		//�r���[�s�񏉊���
		matView    = XMMatrixIdentity();
		matViewRot = XMMatrixIdentity();
		//��]�����x�N�g��������
		camAxisX = XMLoadFloat3(&ie);
		camAxisY = XMLoadFloat3(&ie);
		camAxisZ = XMLoadFloat3(&ie);

		UpdateViewMatrix();
	};
	~Raki_BaseCamera() {};

	// ---------- �����o�ϐ� ---------- //

	///�r���[�\���v�f
	XMVECTOR eyeVec;    //�J�����ʒu
	XMVECTOR targetVec; //���������x�N�g��
	XMVECTOR upVec;     //������x�N�g��

	//�J������]�p�v�f
	XMVECTOR    camRad;           //�J������]�p
	const float camRange = 5.0f;  //�J�����ʒu�Ǝ��������x�N�g���Ƃ̋���

	// ---------- �����o�֐� ---------- //

	/// <summary>
	/// �J����������
	/// </summary>
	void Init();

	/// <summary>
	/// �r���[�s��X�V
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// ���z�֐�
	/// <para> ���N���X�ł�UpdateViewMatrix�̌Ăяo���̂ݎ��s</para>
	/// <para> �h���N���X�ł͑Ή�����@�\�ɉ������X�V�������L�q���Ď��s���邱��</para>
	/// <para> �������h����ł͕K���A�֐�����UpdateViewMatrix�����s���邱��</para>
	/// <para>�i�h����ł̌ŗL�̕ϐ�������ꍇ�͂܂��ʂ����A����ȂƂ��͍X�V�����͕ʂŋL�q����ׂ������j</para>
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	XMMATRIX GetView();

};

