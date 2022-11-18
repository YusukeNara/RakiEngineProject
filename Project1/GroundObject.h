#pragma once
#include <GameObject.h>

//ínå`
class GroundObject :
    public GameObject
{
public:

    GroundObject(){}
    ~GroundObject() override {};

    void Init() override;

    void Update() override;

    void Draw() override;

    void Finalize() override;

    void OnCollision(ColliderInfo* info) override;

    void Load() override;



private:





};

