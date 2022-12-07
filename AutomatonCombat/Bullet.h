#pragma once

#include <NY_Object3DMgr.h>
#include <RVector.h>
#include <memory>
#include <GameObject.h>

class Bullet : public GameObject
{
public:

	RVector3			pos;
	RVector3			startPos;
	RVector3			vec;
	float				speedScale;
	bool				isAlive = false;
	RV3Colider::Sphere	sphere;

	Bullet(RVector3 pos, RVector3 start, RVector3 vec, float ss, float size, Model3D *modelData);
	Bullet() {};
	~Bullet();

	void Init()		override;

	void Update()	override;

	void Draw()		override;

	void Finalize()	override;

	bool CheckDistance();

	void Fire(RVector3 pos, RVector3 vec, float speedScale, float bulletSize, std::shared_ptr<Model3D> modelData);
};