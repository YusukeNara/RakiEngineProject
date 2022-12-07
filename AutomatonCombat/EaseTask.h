#pragma once

#include <Sprite.h>
#include <RVector.h>

using namespace Rv3Ease;

//イージング課題用
class EaseTask
{
public:
	Sprite easeWindow;

	UINT handle;

	RVector3 ease[3];
	RVector3 target[3];
	RVector3 start[3];

	void Init();

	void Update();

	void Draw();

	int frame;
	const int EASE_FRAME = 30;

	float width = 100;
	float height = 50;

};

