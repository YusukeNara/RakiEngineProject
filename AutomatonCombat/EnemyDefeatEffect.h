#pragma once

#include <ParticleManager.h>

class DefeatParticle : public ParticlePrototype
{
public:
	DefeatParticle() {
		Init();
	};

	virtual void Init() override;

	virtual void Update() override;

	virtual ParticlePrototype* clone(RVector3 pos) override;


private:

	RVector3 vec;


};