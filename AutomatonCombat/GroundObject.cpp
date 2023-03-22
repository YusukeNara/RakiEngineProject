#include "GroundObject.h"
#include <NY_random.h>

void GroundObject::Init()
{
	Load();
	object3d->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));
	hg.Init();

	reloadP = new ReloadedEffect();

	reloadParticle = std::make_unique<ParticleManager>();
	reloadParticle.reset(ParticleManager::Create());
	reloadParticle->Prototype_Set(reloadP);

	reloadedTex = TexManager::LoadTexture("Resources/effect1.png");
}

void GroundObject::Update()
{
	hg.Update();

	if (RV3Colider::Colision2Sphere(player->bodyColider, hg.sphere)) {
		player->Reload();
		hg.Repositioning(RVector3(NY_random::floatrand_sl(200.f, -200.f), 10, NY_random::floatrand_sl(200.f, -200.f)));
		reloadParticle->Prototype_Add(1, player->pos);
	}

	reloadParticle->Prototype_Update();
}

void GroundObject::Draw()
{
	object3d->DrawObject();
	hg.Draw();
}

void GroundObject::EffectDraw()
{
	reloadParticle->Prototype_Draw(reloadedTex);
}

void GroundObject::Finalize()
{
}

void GroundObject::OnCollision(const ColliderInfo* info)
{
}

void GroundObject::Load()
{
	std::shared_ptr<Model3D> plane = std::make_shared<Model3D>();

	plane->CreatePlaneModelXZ(1000.0f, 1000.0f, 2, 2, TexManager::LoadTexture("Resources/asp3.dds"), nullptr);

	object3d = std::make_shared<Object3d>();

	object3d->SetLoadedModelData(plane);
}

void ReloadedEffect::Init()
{
}

void ReloadedEffect::Update()
{
	pos += vel;

	nowFrame++;
}

ParticlePrototype* ReloadedEffect::clone(RVector3 pos)
{
	ReloadedEffect* p = new ReloadedEffect;

	p->pos = pos;
	p->vel.y = 1.0f;

	p->scale = 5.0f;

	p->nowFrame = 0;

	p->endFrame = 60;

	return p;
}
