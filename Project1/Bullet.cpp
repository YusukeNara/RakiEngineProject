#include "Bullet.h"

Bullet::Bullet(RVector3 pos, RVector3 start, RVector3 vec, float ss, float size, Model3D *modelData)
{
	this->pos = pos;
	startPos = start;
	this->vec = vec;
	speedScale = ss;
	coliSphere.center = pos;
	coliSphere.rad = size;
	isAlive = false;
	object = LoadModel_ObjFile("Sphere");
}

Bullet::~Bullet()
{
	if (object) { 
		delete object; 
	}
}

void Bullet::Init(RVector3 pos, RVector3 start, RVector3 vec, float ss, float size)
{
	this->pos = pos;
	startPos = start;
	this->vec = vec;
	speedScale = ss;
	coliSphere.center = pos;
	coliSphere.rad = size;
	isAlive = false;
	object = LoadModel_ObjFile("Sphere");
}

void Bullet::Update()
{
	if (!isAlive) { return; }

	pos += (vec * speedScale);
	coliSphere.center = pos;

	object->SetAffineParam(
		RVector3(1.5, 1.5, 1.5),
		RVector3(0, 0, 0),
		pos);

}

void Bullet::Draw()
{
	//if (!isAlive) { return; }

	object->DrawObject();
}

void Bullet::Finalize()
{

}

bool Bullet::CheckDistance()
{
	return distance(pos, startPos) > 1000.0f;
}

void Bullet::Fire(RVector3 pos, RVector3 vec, float speedScale, float bulletSize)
{
	if (isAlive) { return; }
	this->pos = pos;
	this->startPos = pos;
	this->vec = vec;
	this->speedScale = speedScale;
	this->coliSphere.rad = bulletSize;
	isAlive = true;
}
