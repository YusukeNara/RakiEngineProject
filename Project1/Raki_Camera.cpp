#include "Raki_Camera.h"

void Raki_BaseCamera::Init()
{
}

void Raki_BaseCamera::UpdateViewMatrix()
{

	/// �J����z���x�N�g�������߂�
	camAxisZ = XMVectorSubtract(targetVec, eyeVec);

	//Z���x�N�g����0���ƌv�Z�ł��Ȃ��̂ŏ��O
	assert(!XMVector3Equal(camAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(camAxisZ));
	//������x�N�g����0���ƌv�Z�ł��Ȃ��̂ŏ��O
	assert(!XMVector3Equal(upVec, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVec));

	//z���x�N�g���𐳋K��
	camAxisZ = XMVector3Normalize(camAxisZ);

	/// �J����X���x�N�g�������߂�
	camAxisX = XMVector3Cross(upVec, camAxisZ);
	//X���x�N�g�����K��
	camAxisX = XMVector3Normalize(camAxisX);

	/// �J����Y���x�N�g�������߂�
	camAxisY = XMVector3Cross(camAxisZ, camAxisX);
	//y���x�N�g�����K��
	camAxisY = XMVector3Normalize(camAxisY);

	/// ��]�s������߂�
	matViewRot.r[0] = camAxisX;
	matViewRot.r[1] = camAxisY;
	matViewRot.r[2] = camAxisZ;
	matViewRot.r[3] = XMVectorSet(0, 0, 0, 1);

	/// �]�u�ɂ���]�s��̋t�s������߁A�r���[�ɔ��f
	matView = XMMatrixTranspose(matViewRot);

	/// ���s�ړ��̋t�s������߂�

	//�J�����ʒu�̋t�x�N�g��(* -1)
	XMVECTOR revEye = XMVectorNegate(eyeVec);
	//�J�����ʒu���烏�[���h���_�̃x�N�g��
	XMVECTOR tx = XMVector3Dot(camAxisX, revEye);//x
	XMVECTOR ty = XMVector3Dot(camAxisY, revEye);//y
	XMVECTOR tz = XMVector3Dot(camAxisZ, revEye);//z
	//�܂Ƃ߂�
	XMVECTOR t  = XMVectorSet(tx.m128_f32[0], ty.m128_f32[1], tz.m128_f32[2], 1.0f);

	/// �r���[�ɔ��f
	matView.r[3] = t;

}

void Raki_BaseCamera::Update()
{

	//�r���[�s��X�V
	UpdateViewMatrix();
}

XMMATRIX Raki_BaseCamera::GetView()
{
	return matView;
}
