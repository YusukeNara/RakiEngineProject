#pragma once

#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:

	CollisionManager() = default;
	~CollisionManager() = default;

	//�����P�̂ŌĂяo���΁A�R���C�_�[������ǉ����邱�Ƃ��\
	inline void AddCollider(BaseCollider* col) {
		colliders.push_front(col);
	}

	//�Ǘ�����O���̂�
	inline void RemoveColider(BaseCollider* col) {
		colliders.remove(col);
	}

	//���ׂĂ̏Փ˔�����ꊇ�ōs��
	void CheckAllCollisions();


private:

	std::forward_list<BaseCollider*> colliders;

	//���蕪��֐�
	void CheckCollision(BaseCollider* colA, BaseCollider* colB);

	//�e�픻��p�֐�
	//dynamic_cast���Ĕ�������Ature�̏ꍇOnCollision�����s����

	void CheckCollisionSphereTo(BaseCollider* sphere, BaseCollider* colB);

	void CheckCollisionPolygonTo(BaseCollider* polygon, BaseCollider* colB);

	void CheckCollisionAABBTo(BaseCollider* aabb, BaseCollider* colB);

	void CheckCollisionPlaneTo(BaseCollider* plane, BaseCollider* colB);

	void CheckCollisionRayTo(BaseCollider* ray, BaseCollider* colB);

};

