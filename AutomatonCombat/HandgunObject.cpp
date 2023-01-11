#include "HandgunObject.h"

HandgunObject::HandgunObject()
{
}

void HandgunObject::Init()
{
	model = std::make_shared<Model3D>();
	model->LoadObjModel("saru");

	this->object3d = std::make_shared<Object3d>();
	object3d->InitObject3D(RAKI_DX12B_DEV);
	object3d->SetLoadedModelData(model);
	object3d->SetAffineParamScale(RVector3(10, 10, 10));
	object3d->SetAffineParamTranslate(RVector3(100, 0, 100));

	//collider = std::make_shared<SphereCollider>();

	sphere.center.zero();
	sphere.rad = 16.0f;
}

void HandgunObject::Update()
{
}

void HandgunObject::Draw()
{
	object3d->DrawObject();
}

void HandgunObject::Finalize()
{
}

void HandgunObject::OnCollision(const ColliderInfo* info)
{
}

void HandgunObject::Load()
{
}

void HandgunObject::Repositioning(RVector3 newpos)
{
	sphere.center = newpos;
	object3d->SetAffineParamTranslate(newpos);
}
