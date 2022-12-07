#include "testParticle.h"
#include <NY_random.h>

BomParticle::~BomParticle()
{
	int a = 0;
}

void BomParticle::Init()
{
	//�J�n���W
	pos = spos;

	//�I���t���[���ݒ�
	endFrame = 60;

	//���x�ݒ�
	float xvel = NY_random::floatrand_sl(1.0f, -1.0f);
	float yvel = NY_random::floatrand_sl(1.0f, -1.0f);
	float zvel = NY_random::floatrand_sl(1.0f, -1.0f);

	vel = RVector3(xvel, yvel, zvel);

	color = DirectX::XMFLOAT4(1, 1, 1, 1);
	scale = 1.0f;
}

void BomParticle::Update()
{
	nowFrame++;
	//���x�𖈃t���[�����Z
	pos += vel;
}

ParticlePrototype *BomParticle::clone(RVector3 startPos)
{
	return new BomParticle(startPos);
}
