#pragma once

#include "RVector.h"
#include "ColliderShapeType.h"
#include "ColliderInfo.h"

class GameObject;

//�R���C�_�[���N���X
class BaseCollider
{
public:
	BaseCollider()			= default;
	virtual ~BaseCollider()	= default;

	inline void SetObject(GameObject* object) {
		this->object = object;
	}

	GameObject *GetObject3d() { 
		return object; 
	}

	virtual void Update() = 0;

	inline COLIDER_TYPE GetColliderType() { return type; }

	virtual void OnColision(const ColliderInfo& info);

protected:
	// �R���C�_�[�o�^�I�u�W�F�N�g
	GameObject* object;

	// �R���C�_�[�̎��
	COLIDER_TYPE type = COLIDER_TYPE_UNKNOWN;





};