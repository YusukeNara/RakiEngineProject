#pragma once

#include <ParticleManager.h>

class BomParticle : public ParticlePrototype
{
public:
	//�ϐ��Ƃ�

	//�J�n�ʒu���ق��񂷂��[��
	RVector3 spos;

public:
	BomParticle(RVector3 startpos) : spos(startpos) {
		Init();
	};
	~BomParticle();

	//������
	void Init() override;
	//�X�V
	void Update() override;
	//�N���[���쐬
	ParticlePrototype *clone(RVector3 startPos) override;
};