#pragma once
#include <ParticleManager.h>

class EnemySpawnEffect : public ParticlePrototype
{
public:

	virtual void Init() override;

	virtual void Update() override;

	virtual ParticlePrototype* clone(RVector3 pos) override;
};