#pragma once
#include <Sprite.h>
#include <RVector.h>
#include <Raki_Input.h>

class MouseStatus
{
	//座標　速度
	RVector3 pos;
	RVector3 v;
	//重量
	float m;
	//運動量
	float p;
	//半径
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
	//点
	MouseStatus mouse;

	Circle a;
	Circle b;

	//円ステータス
	RVector3 center;
	float r;
	//質量、速度
	float m;
	RVector3 v;
	//運動量
	float p;

	//跳ね返り係数
	float e = 1.0f;



	Sprite circle;

	void Init();

	void Update();

	void Draw();

	bool isHitPointCircle(RVector3 center, float r, RVector3 ppos, RVector3& coliPos);

	bool isHitCircle(Circle a, Circle b);

};

