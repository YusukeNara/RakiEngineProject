#include "GroundObject.h"

void GroundObject::Init()
{
	Load();
	object3d->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));
}

void GroundObject::Update()
{
}

void GroundObject::Draw()
{
	object3d->DrawObject();
}

void GroundObject::Finalize()
{
}

void GroundObject::OnCollision(ColliderInfo* info)
{
}

void GroundObject::Load()
{
	object3d.reset(NY_Object3DManager::Get()->CreateModel_Tile(5000, 5000, 50, 50, TexManager::LoadTexture("Resources/blackParticleTex.png")));
}
