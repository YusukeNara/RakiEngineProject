#include "Bullet.h"

Bullet::Bullet(RVector3 pos, RVector3 start, RVector3 vec, float ss, float size, Model3D *modelData)
{
	this->pos = pos;
	startPos = start;
	this->vec = vec;
	speedScale = ss;
	isAlive = false;
	object3d->SetLoadedModelData(modelData);
	this->object3d = std::make_shared<Object3d>();
}

Bullet::~Bullet()
{

}

void Bullet::Init()
{
	this->object3d = std::make_shared<Object3d>();
}

void Bullet::Update()
{
	if (!isAlive) { return; }

	pos += (vec * speedScale);

	object3d->SetAffineParam(
		RVector3(1.5, 1.5, 1.5),
		RVector3(0, 0, 0),
		pos);

	dynamic_cast<SphereCollider*>(collider.get())->sphere.center = pos;

	sphere.center = pos;

}

void Bullet::Draw()
{
	if (!isAlive) { return; }

	object3d->DrawObject();
}

void Bullet::Finalize()
{
	this->collider.reset();
}

bool Bullet::CheckDistance()
{
	return distance(pos, startPos) > 1000.0f;
}

void Bullet::Fire(RVector3 pos, RVector3 vec, float speedScale, float bulletSize, std::shared_ptr<Model3D> modelData)
{
	this->pos = pos;
	this->startPos = pos;
	this->vec = vec;
	this->speedScale = speedScale;
	this->collider.reset(new SphereCollider(pos, bulletSize));
	this->object3d->SetLoadedModelData(modelData);
	isAlive = true;
	sphere.center = pos;
	sphere.rad = bulletSize;
}
