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
	delete bullet_proto;
}

void Bullet::Init()
{
	this->object3d = std::make_shared<Object3d>();
	bullet_pm = ParticleManager::Create();
	bullet_tex = TexManager::LoadTexture("Resources/effect1.png");
	bullet_proto = new BulletParticle;
	bullet_pm->Prototype_Set(bullet_proto);
}

void Bullet::Update()
{
	bullet_pm->Prototype_Update();

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
	bullet_pm->Prototype_Draw(bullet_tex);

	if (!isAlive) { return; }

	//object3d->DrawObject();


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

	bullet_proto->vel = vec * speedScale;
	bullet_pm->Prototype_Add(1, pos);

}

void BulletParticle::Init()
{
	nowFrame = 0;

	endFrame = 180;

	scale = 5.0f;

	color = { 1.0f,1.0f,1.0f,1.0f };
}

void BulletParticle::Update()
{
	pos += vel;

	nowFrame++;
}

ParticlePrototype* BulletParticle::clone(RVector3 pos)
{
	BulletParticle* p = new BulletParticle;

	p->pos = pos;
	p->vel = vel;

	return p;
}

void BulletParticle::SetVec(RVector3 vec)
{
	this->vel = vec;
}
