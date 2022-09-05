#pragma once

#include <NY_Object3DMgr.h>
#include <RVector.h>
#include <memory>

class Bullet
{
public:

	RVector3			pos;
	RVector3			startPos;
	RVector3			vec;
	float				speedScale;
	RV3Colider::Sphere	coliSphere;
	Object3d			*object;
	bool				isAlive = false;

	Bullet(RVector3 pos, RVector3 start, RVector3 vec, float ss, float size, Model3D *modelData);
	Bullet() {};
	~Bullet();

	void Init(RVector3 pos, RVector3 start, RVector3 vec, float ss, float size);

	void Update();

	void Draw();

	void Finalize();

	bool CheckDistance();

	void Fire(RVector3 pos, RVector3 vec, float speedScale, float bulletSize);
};