#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Sprite.h"

#pragma comment(lib, "d3d12.lib")

#define IKD_EPSIRON 0.00001f//�덷

using namespace DirectX;

//�X�N���[�����W�����[���h���W�ɕϊ��i�Ώۂ̃X�N���[�����WX,Y,�ˉe���Z�ʒu,�r���[�|�[�g�͈́H,�ˉe�ϊ��s��j
XMFLOAT3 Proto_CalcScreenToWorldPos(float scrX, float scrY, float fz, int window_w, int window_h, XMMATRIX prj)
{
	XMVECTOR pos;
	//�ˉe�ϊ��s��ƃr���[�|�[�g�s��̋t�s����i�[����ϐ�
	XMMATRIX InvPrj, InvVP;
	//�e�s��̋t�s����o��
	InvPrj = XMMatrixInverse(nullptr, prj);
	//�r���[�|�[�g�s��͂��Ƃ͂Ȃ��H�̂ł����Œ�`���ċt�s����o��
	InvVP = XMMatrixIdentity();
	InvVP.r[0].m128_f32[0] = window_w / 2.0f;
	InvVP.r[1].m128_f32[1] = -window_h / 2.0f;
	InvVP.r[3].m128_f32[0] = window_w / 2.0f;
	InvVP.r[3].m128_f32[1] = window_h / 2.0f;
	InvVP = XMMatrixInverse(nullptr, InvVP);

	XMMATRIX inverce = InvVP * InvPrj;
	XMVECTOR scr = { scrX,scrY,fz};

	pos = XMVector3TransformCoord(scr, inverce);

	XMFLOAT3 returnpos = { pos.m128_f32[0],pos.m128_f32[1],pos.m128_f32[2] };

	return returnpos;
}


XMVECTOR Proto_CalcScreenToWorldPosVector(float scrX, float scrY, float fz, int window_w, int window_h, XMMATRIX prj,XMMATRIX view)
{
	XMVECTOR pos;
	//�ˉe�ϊ��s��ƃr���[�|�[�g�s��̋t�s����i�[����ϐ�
	XMMATRIX InvPrj, InvVP, InvCam;
	//�e�s��̋t�s����o��

	//�ˉe�ϊ��̋t�s��
	InvPrj = XMMatrixInverse(nullptr, prj);
	//�r���[�|�[�g�s��͂��Ƃ͂Ȃ��H�̂ł����Œ�`���ċt�s����o��
	InvVP = XMMatrixIdentity();
	InvVP.r[0].m128_f32[0] = (float) window_w / 2.0f;
	InvVP.r[1].m128_f32[1] = (float)-window_h / 2.0f;
	InvVP.r[2].m128_f32[2] = (float)1.0f - 0.0f;
	InvVP.r[3].m128_f32[0] = (float) window_w / 2.0f;
	InvVP.r[3].m128_f32[1] = (float) window_h / 2.0f;
	InvVP.r[3].m128_f32[2] = (float)0.0f;
	InvVP = XMMatrixInverse(nullptr, InvVP);

	InvCam = XMMatrixInverse(nullptr, InvCam);
	//�e�t�s�����Z
	XMMATRIX inverce = InvVP * InvPrj * InvCam;
	//�X�N���[�����W�Ɏˉez�����������̂�XMVECTOR�Œ�`
	XMVECTOR scr = { scrX,scrY,fz };
	//�X�N���[���Ɏˉez����������ƍ����t�s�����Z
	pos = XMVector3TransformCoord(scr, inverce);
	//���C�̕����x�N�g����ԋp
	return pos;
}

/*
	��q�֐����g�p����near��far�����߂��Ƃ��Az���ˉe��Ԃ�z�Ƃ��Čv�Z����Ȃ��i����1�ɂȂ�j

	�����F�����Ɏ�����ˉe�s�񂪁A�X�v���C�g�̎ˉe�s�񂾂����B
	�ˉe��3D��ԏ�ɕ`�悳��镨�̂𒼕��̋�ԓ��Ɏ��߂邽�߂̍s��B
	������sprite��2D��ԂȂ̂ŁA����̋t�s����g�p���ĕϊ����Ă�2D�̂܂܁B

	3D�I�u�W�F�N�g�̕ϊ��Ɏg�p���Ă���ˉe�s����g�p���Ȃ���΂Ȃ�Ȃ��I�I�I
*/

//���C�Ƌ��̏Փ˓_���v�Z���ԋp(�Ώۂ̔��a�A�Ώۂ̒��S�_�A���C�̊J�n�_�A���C�̕����x�N�g���A�i�o�́j�Փ˓_�A�i�o�́j����)
bool CalcSphereRayColision(float rad, XMVECTOR center, XMVECTOR rayStart, XMVECTOR rayVec, XMVECTOR *out_ColiP, float *pOut_t)
{
	XMVECTOR u = rayStart - center;

	float a = XMVectorGetX(XMVector3Dot(rayVec, rayVec));
	float b = XMVector3Dot(rayVec, u).m128_f32[0];
	float c = XMVector3Dot(u, u).m128_f32[0] - rad * rad;

	if (a - IKD_EPSIRON <= 0.0f) {
		//�덷
		return false;
	}

	float isColli = b * b - a * c;
	if (isColli < 0.0f) {
		//�Փ˂��Ȃ�
		return false;
	}

	float t = (-b - sqrt(b * b - a * c)) / a;

	if (pOut_t) {
		*pOut_t = t;
	}

	if (out_ColiP) {
		*out_ColiP = rayStart + rayVec * t;
	}

	return true;
}


XMVECTOR GetFollowingBulletPos(XMVECTOR targetPos, XMVECTOR bulletPos, XMVECTOR velocity, float period)
{
	XMVECTOR accel = { 0,0,0 };
	XMVECTOR diff = targetPos - bulletPos;

	accel += (diff - velocity * period) * 2.0f / (period * period);

	XMVECTOR vel = velocity;
	vel += accel;

	XMVECTOR returnPos = bulletPos;
	returnPos += vel;
	
	return returnPos;
}