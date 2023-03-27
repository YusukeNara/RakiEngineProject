#include "BuildingObject.h"
#include <Raki_imguiMgr.h>

shared_ptr<Model3D> BuildingObject::BuildModel;

void BuildingObject::Init()
{
	this->object3d = std::make_shared<Object3d>();

	object3d->InitObject3D(RAKI_DX12B_DEV);
	object3d->SetLoadedModelData(BuildModel);

	object3d->SetAffineParamScale(RVector3(5, 5, 5));
	object3d->SetAffineParamTranslate(RVector3(100, 0, 100));

	object3d->isDrawShadow = true;
}

void BuildingObject::Update()
{

}

void BuildingObject::Draw()
{
	object3d->DrawObject();
}

void BuildingObject::LastDraw()
{
	object3d->DrawInstanceModelObject(true);
}

void BuildingObject::DebugDraw()
{

}

void BuildingObject::Finalize()
{
}

void BuildingObject::OnCollision(const ColliderInfo* info)
{
}

void BuildingObject::Load()
{
	BuildModel = std::make_shared<Model3D>();
	BuildModel->LoadObjModel("Residential_Buildings_001");
}
