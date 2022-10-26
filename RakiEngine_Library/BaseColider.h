#pragma once

#include "NY_Object3DMgr.h"
#include "RVector.h"
#include "ColliderShapeType.h"
#include "ColliderInfo.h"

//�R���C�_�[���N���X
class BaseCollider
{
public:
	BaseCollider()			= default;
	virtual ~BaseCollider()	= default;

	inline void SetObject(Object3d* object) {
		this->object = object;
	}

	inline Object3d *GetObject3d() { return object; }

	virtual void Update() = 0;

	inline COLIDER_TYPE GetColliderType() { return type; }

	virtual void OnColision(const ColliderInfo& info){}

protected:
	// �R���C�_�[�o�^�I�u�W�F�N�g
	Object3d* object;

	// �R���C�_�[�̎��
	COLIDER_TYPE type = COLIDER_TYPE_UNKNOWN;





};