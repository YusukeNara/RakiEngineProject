#pragma once
#include <GameObject.h>
#include <memory>

class HandgunObject : public GameObject
{
public:

    HandgunObject();
    ~HandgunObject() override {};

    void Init() override;

    void Update() override;

    void Draw() override;

    void Finalize() override;

    void OnCollision(const ColliderInfo* info) override;

    void Load() override;

    void Repositioning(RVector3 newpos);

    std::shared_ptr<Model3D> model;

    RV3Colider::Sphere sphere;
};

