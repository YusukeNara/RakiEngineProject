#pragma once
#include <Sprite.h>
#include <RVector.h>

class CircleMoveTask
{
public:
	RVector3 circles[3];
	int frame = 0;
	float t = 0.0f;
	//”¼Œa
	float r[3];
	//Šp‘¬“x
	float radSpeed[3];
	float initr = 0.0f;
	
	Sprite circle;

	const float SIZE = 10.0f;
	const float OFFSET_X = 300.0f;
	const float OFFSET_Y = 300.0f;

	void Init();

	void Update();

	void Draw();

	RVector3 CircleMove(float r, float rs, float t, float initRad, RVector3& v);


};

