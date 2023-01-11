#pragma once
#include <GameObject.h>

class SkyBoxObject : public GameObject
{
public:
	void Init() override;
	void Update()	override;
	void Draw()		override;
	void Finalize() override;
	void Load() override;
	void OnCollision(const ColliderInfo* info) override;

	void SetAffinParam(RVector3 rot, RVector3 pos);


};