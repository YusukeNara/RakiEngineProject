#pragma once
#include <GameObject.h>

#include "Player.h"
#include "HandgunObject.h"

//�n�`
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

    void OnCollision(const ColliderInfo* info) override;

    void Load() override;

    void SetPlayer(Player* player) {
        this->player = player;
    }

private:

    Player* player;

    HandgunObject hg;

};

