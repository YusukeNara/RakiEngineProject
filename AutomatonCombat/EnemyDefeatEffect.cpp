#include "EnemyDefeatEffect.h"
#include <NY_random.h>

void DefeatParticle::Init()
{
}

void DefeatParticle::Update()
{

	if (pos.y >= 0.0f) {
		pos -= vel;
		vel -= acc;
	}

	nowFrame++;
}

ParticlePrototype* DefeatParticle::clone(RVector3 pos)
{
	DefeatParticle* p = new DefeatParticle;

	p->pos = pos;
	p->vel.x = NY_random::floatrand_sl(5.f, -5.f);
	p->vel.y = NY_random::floatrand_sl(5.f, 0.f);
	p->vel.z = NY_random::floatrand_sl(5.f, -5.f);

	p->acc.y = -0.2f;

	p->nowFrame = 0;

	p->endFrame = 60;

	return p;
}
