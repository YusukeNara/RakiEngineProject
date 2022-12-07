#pragma once
#include <Sprite.h>
#include <RVector.h>
#include <Raki_Input.h>
#include <NY_random.h>

class DotCrossTask
{
public:

	//線分ab
	RVector3 a, b;
	//円中心
	RVector3 center;
	//円半径
	float r;

	//円スプライト
	Sprite sphere;
	//線スプライト
	Sprite line;

	void Init();

	void Update();

	void Draw();

	bool isHitLineCircle(RVector3 a, RVector3 b, RVector3 center, float r);

};

