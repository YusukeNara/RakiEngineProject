#pragma once
#include <Sprite.h>
#include <RVector.h>
#include <Raki_Input.h>

class MouseStatus
{
	//���W�@���x
	RVector3 pos;
	RVector3 v;
	//�d��
	float m;
	//�^����
	float p;
	//���a
	float r;

};

struct Circle
{
	RVector3 center;
	float r;

	float m;
	RVector3 v;
	RVector3 v0;
};

class ConflictAndRepulsionTask
{
public:
	//�_
	MouseStatus mouse;

	Circle a;
	Circle b;

	//�~�X�e�[�^�X
	RVector3 center;
	float r;
	//���ʁA���x
	float m;
	RVector3 v;
	//�^����
	float p;

	//���˕Ԃ�W��
	float e = 1.0f;



	Sprite circle;

	void Init();

	void Update();

	void Draw();

	bool isHitPointCircle(RVector3 center, float r, RVector3 ppos, RVector3& coliPos);

	bool isHitCircle(Circle a, Circle b);

};

