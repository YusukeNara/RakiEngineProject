#include "SkyBoxObject.h"
#include <NY_Object3DMgr.h>

void SkyBoxObject::Init()
{
	Load();
	object3d->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));
}

void SkyBoxObject::Update()
{
}

void SkyBoxObject::Draw()
{
	object3d->DrawObject();
}

void SkyBoxObject::Finalize()
{

}

void SkyBoxObject::Load()
{
	object3d.reset(NY_Object3DManager::Get()->CreateModel_Tile(500, 500, 1, 1, TexManager::LoadTexture("Resources/Sky2.png")));
}

void SkyBoxObject::OnCollision(const ColliderInfo* info)
{
}

void SkyBoxObject::SetAffinParam(RVector3 rot, RVector3 pos)
{
	object3d->SetAffineParamRotate(rot);
	object3d->SetAffineParamTranslate(pos);
}
