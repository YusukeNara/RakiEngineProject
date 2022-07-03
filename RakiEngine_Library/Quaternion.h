#pragma once
#include "RVector.h"
#include <DirectXMath.h>

struct RQuaternion {
	float x;
	float y;
	float z;
	float w;
};

//�N�I�[�^�j�I�����쐬
RQuaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
//�C�ӎ��̉�]����
RQuaternion quaternion(RVector3 &v, float angle);
//�x�N�g��v���p����\���N�I�[�^�j�I��q�ŉ�]
RQuaternion quaternion(const RVector3 &v, const RQuaternion &q);

//���ς����߂�
float dot(const RQuaternion &q1, const RQuaternion &q2);

//�m���������߂�
float lenght(const RQuaternion &q);

//���K������
RQuaternion normalize(const RQuaternion &q);

//�����l���������߂�
RQuaternion conjugate(const RQuaternion &q);

//�P�����Z�q�I�[�o�[���[�h
RQuaternion operator+(const RQuaternion &q);
RQuaternion operator-(const RQuaternion &q);

//������Z�q�I�[�o���[�h
RQuaternion &operator+=(RQuaternion &q1, const RQuaternion &q2);
RQuaternion &operator-=(RQuaternion &q1, const RQuaternion &q2);
RQuaternion &operator*=(RQuaternion &q, float s);
RQuaternion &operator/=(RQuaternion &q, float s);
RQuaternion &operator*=(RQuaternion &q1, const RQuaternion &q2);
RQuaternion &operator/=(RQuaternion &q1, const RQuaternion &q2);

//�񍀉��Z�q�I�[�o�[���[�h
RQuaternion operator+(const RQuaternion &q1, const RQuaternion &q2);
RQuaternion operator-(const RQuaternion &q1, const RQuaternion &q2);
RQuaternion operator*(const RQuaternion &q1, const RQuaternion &q2);
RQuaternion operator*(const RQuaternion &q1, float s);
RQuaternion operator*(float s, const RQuaternion &q1);
RQuaternion operator/(const RQuaternion &q1, float s);

RQuaternion slarp(const RQuaternion &q1, const RQuaternion &q2, float t);
RQuaternion larp(const RQuaternion &q1, const RQuaternion &q2, float t);
DirectX::XMMATRIX rotate(const RQuaternion &q);
RQuaternion quaternion(const DirectX::XMMATRIX &m);
RVector3 getAxis(const RQuaternion &q);