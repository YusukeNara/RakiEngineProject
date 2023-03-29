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

	RVector3 rand(NY_random::floatrand_sl(100.0f, -100.f),
		NY_random::floatrand_sl(100.0f, -100.f),
		NY_random::floatrand_sl(100.0f, -100.f));

	p->pos += rand;

	p->vel.x = 0;
	p->vel.y = NY_random::floatrand_sl(1.0f, 0.f);
	p->vel.z = 0;

	p->acc.y = 0.2f;

	p->nowFrame = 0;

	p->endFrame = 120;

	return p;
}
