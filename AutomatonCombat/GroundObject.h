#pragma once
#include <GameObject.h>
#include <ParticleManager.h>

#include "Player.h"
#include "HandgunObject.h"

class ReloadedEffect : public ParticlePrototype
{
public:
    ReloadedEffect() {
        Init();
    };

    virtual void Init() override;

    virtual void Update() override;

    virtual ParticlePrototype* clone(RVector3 pos) override;


private:

    RVector3 vec;


};

//’nŒ`
class GroundObject :
    public GameObject
{
public:

    GroundObject(){}
    ~GroundObject() override {};

    void Init() override;

    void Update() override;

    void Draw() override;

    void lastDraw();

    void EffectDraw();

    void Finalize() override;

    void OnCollision(const ColliderInfo* info) override;

    void Load() override;

    void SetPlayer(Player* player) {
        this->player = player;
    }

private:

    Player* player;

    HandgunObject hg;

    ReloadedEffect* reloadP;
    std::unique_ptr<ParticleManager> reloadParticle;
    int reloadedTex;

};



