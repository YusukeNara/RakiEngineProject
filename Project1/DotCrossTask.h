#pragma once
#include <Sprite.h>
#include <RVector.h>
#include <Raki_Input.h>
#include <NY_random.h>

class DotCrossTask
{
public:

	//����ab
	RVector3 a, b;
	//�~���S
	RVector3 center;
	//�~���a
	float r;

	//�~�X�v���C�g
	Sprite sphere;
	//���X�v���C�g
	Sprite line;

	void Init();

	void Update();

	void Draw();

	bool isHitLineCircle(RVector3 a, RVector3 b, RVector3 center, float r);

};

