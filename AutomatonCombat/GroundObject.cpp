#include "GroundObject.h"
#include <NY_random.h>

void GroundObject::Init()
{
	Load();
	object3d->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));
	hg.Init();
}

void GroundObject::Update()
{
	hg.Update();

	if (RV3Colider::Colision2Sphere(player->bodyColider, hg.sphere)) {
		player->Reload();
		hg.Repositioning(RVector3(NY_random::floatrand_sl(200.f, -200.f), 5, NY_random::floatrand_sl(200.f, -200.f)));
	}


}

void GroundObject::Draw()
{
	object3d->DrawObject();
	hg.Draw();
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

	plane->CreatePlaneModelXZ(1000.0f, 1000.0f, 10, 10, TexManager::LoadTexture("Resources/asp3.png"), nullptr);

	object3d = std::make_shared<Object3d>();

	object3d->SetLoadedModelData(plane);
}
