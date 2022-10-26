#pragma once
#include "RVector.h"
#include "BaseColider.h"
#include "ColliderInfo.h"
#include <memory>

//�e��R���C�_�[�̃N���X

class SphereCollider : public BaseCollider
{
public:
	RV3Colider::Sphere sphere;


	SphereCollider(RVector3 centerPos, float rad) {
		sphere.center = centerPos;
		sphere.rad = rad;
		type = COLIDER_TYPE_SPHERE;
	}

	void Update() override;

	void SetRadius();

protected:
	//���L����Ă���I�u�W�F�N�g�̃|�C���^
	std::shared_ptr<Object3d> objptr;

};

class AABBCollider : public BaseCollider
{
	RV3Colider::Rv3AABB abBox;

	AABBCollider(const RVector3 &max, const RVector3 &min,const RVector3 &pos) {
		abBox.max = max;
		abBox.min = min;
		abBox.oldPos = pos;
	}

	void Update() override;

protected:
	//���L����Ă���I�u�W�F�N�g�̃|�C���^
	std::shared_ptr<Object3d> objptr;
};

class PolygonCollider : public BaseCollider
{



};

class PlaneCollider : public BaseCollider
{



};

class RayCollider : public BaseCollider
{



};

