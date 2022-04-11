#pragma once

#include <ParticleManager.h>

class BomParticle : public ParticlePrototype
{
public:
	//変数とか

	//開始位置をほぞんするやーつ
	RVector3 spos;

public:
	BomParticle(RVector3 startpos) : spos(startpos) {
		Init();
	};
	~BomParticle();

	//初期化
	void Init() override;
	//更新
	void Update() override;
	//クローン作成
	ParticlePrototype *clone(RVector3 startPos) override;
};