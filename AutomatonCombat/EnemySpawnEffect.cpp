#include "EnemySpawnEffect.h"

#include <NY_random.h>

void EnemySpawnEffect::Init()
{

}

void EnemySpawnEffect::Update()
{
	if (pos.y >= 0.0f) {
		pos += vel;
		vel += acc;
	}

	nowFrame++;

	color = { 0.8f,0.3f,0.3f,1.0f };

	scale = 5.0f;
}

ParticlePrototype* EnemySpawnEffect::clone(RVector3 pos)
{
	EnemySpawnEffect* p = new EnemySpawnEffect;

	p->pos = pos;
	p->vel.x = NY_random::floatrand_sl(3.f, -1.f);
	p->vel.y = NY_random::floatrand_sl(10.0f, 0.f);
	p->vel.z = NY_random::floatrand_sl(3.f, -1.f);

	p->acc.y = -0.2f;

	p->nowFrame = 0;

	p->endFrame = 120;

	return p;
}
