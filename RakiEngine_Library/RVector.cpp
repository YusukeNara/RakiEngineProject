#include "RVector.h"

#include "Raki_WinAPI.h"
#include "NY_Camera.h"
#include "FPS.h"

bool RV3Colider::ColisionSphereToPlane(const Sphere &sphere, const Plane &plane, RVector3 *coliPos)
{
	//���ʂƋ��̒��S�Ƃ̋��������߂�
	float dist = dot(sphere.center, plane.normal) - plane.distance;
	//�����̐�Βl�����a���傫����΂������ĂȂ�
	if (fabsf(dist) > sphere.rad) { return false; }
	//�������Ă���ꍇ�͋^����_���v�Z
	*coliPos = -dist * plane.normal + sphere.center;
	//�Փ�
	return true;
}

RVector3 RV3Colider::CalcScreen2World(const XMFLOAT2& scrPos, float fz)
{
	XMVECTOR pos;
	//�ˉe�ϊ��s��ƃr���[�|�[�g�s��̋t�s����i�[����ϐ�
	XMMATRIX InvPrj, InvVP, InvV;
	//�e�s��̋t�s����o��
	InvPrj = XMMatrixInverse(nullptr, camera->GetMatrixProjection());
	//�r���[�|�[�g�s��͂��Ƃ͂Ȃ��H�̂ł����Œ�`���ċt�s����o��
	InvVP = XMMatrixIdentity();
	InvVP.r[0].m128_f32[0] = Raki_WinAPI::window_width / 2.0f;
	InvVP.r[1].m128_f32[1] = -Raki_WinAPI::window_height / 2.0f;
	InvVP.r[3].m128_f32[0] = Raki_WinAPI::window_width / 2.0f;
	InvVP.r[3].m128_f32[1] = Raki_WinAPI::window_height / 2.0f;
	InvVP = XMMatrixInverse(nullptr, InvVP);

	InvV = XMMatrixInverse(nullptr, camera->GetMatrixView());

	XMMATRIX inverce = InvVP * InvPrj * InvV;
	XMVECTOR scr = { scrPos.x,scrPos.y,fz };

	pos = XMVector3TransformCoord(scr, inverce);

	RVector3 returnpos = { pos.m128_f32[0],pos.m128_f32[1],pos.m128_f32[2] };
	return returnpos;
}

const RVector3 Rv3Ease::lerp(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t) + end * t;
}

const RVector3 Rv3Ease::InQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t * t) + end * (float)pow(t, 2);
}

const RVector3 Rv3Ease::OutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - (1.0f - (1.0f - t) * (1.0f - t))) + end * (1.0f - (1.0f - t) * (1.0f - t));
}

const RVector3 Rv3Ease::InOutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return t < 0.5f ? (start * (1.0f - t)) + end * (t * t) : start * (1.0f - t) + end * (1 - (float)pow(-2 * t + 2, 2) / 2);
}

float CalcVelocityToForceAndMass(const float force, const float mass, float nowVel, float &acc)
{
	float accel = acc + CalcAccelToForceAndMass(force, mass);

	float vel = nowVel + accel;

	acc = accel;

	return vel;
}

float CalcAccelToForceAndMass(const float force, const float mass)
{
	return force / mass;
}

float CalcAccelToFramePerVel(float onesecmove)
{
	return onesecmove / 60.0f;
}

float CalcGravity(float start, float end, float v0, int time,float &v1)
{


	//���݂̑��x^2 = �����x^2 + -2*�d�͉����x
	float v = (v0 * v0) + (-2 * CalcAccelToFramePerVel(9.8f));

	float t1 = time / 60.0f;
	float t2 = (time - 1) / 60.0f;

	//���ݍ��W = �J�n�_ + ���ݑ��x
	float now = start + (sqrtf(v) * -t2);

	float next = now + (sqrtf(v) * -t1);

	if (next < end) {//���~���A�I���_��聫
		now = end;//�l���ۂ߂�
	}

	//����
	return next;
}
