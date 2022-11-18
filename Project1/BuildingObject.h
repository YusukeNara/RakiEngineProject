#pragma once
#include <GameObject.h>

class BuildingObject :
    public GameObject
{
public:

    BuildingObject() {}
    ~BuildingObject() override {};

    void Init() override;

    void Update() override;

    void Draw() override;

    void Finalize() override;

    void OnCollision(ColliderInfo* info) override;

    void Load() override;

    void SetBuildingPos(RVector3 pos) {
        this->object3d->SetAffineParamTranslate(pos);
    }

    static shared_ptr<Model3D> BuildModel;
};

