#pragma once

#include "Player.h"
#include "SwordEnemy.h"

#include <GameObject.h>
#include <BehaviorNodeBase.h>
#include <NavMeshAstar.h>

class SwordApproachNode : public bTreesys::BehaviorActionNode
{
public:

	SwordApproachNode() {};
	~SwordApproachNode() {};

	/// �|�C���^����
	/// ���̃t�@�C���̃N���X��swordEnemy�������Ă���
	/// player��astar��swordEnemy�̊O���̃N���X
	/// �܂�enemy�͓����̃X�R�[�v�ƂȂ�̂Ő��|�C���^�ŎQ�Ƃł��ǂ��H
	/// player��astar�͊O���X�R�[�v�Ȃ̂ŃX�}�[�g�|�C���^�ɂ��Ȃ���΂Ȃ�Ȃ�

	void SetPointer(SwordEnemy *enemy, std::shared_ptr<Player> player,std::shared_ptr<NavMeshAstar> astar) {
		this->enemy = enemy;
		this->player = player;
		this->astar = astar;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//�Q�Ƃ���I�u�W�F�N�g
	SwordEnemy *enemy;
	std::weak_ptr<Player> player;
	std::weak_ptr<NavMeshAstar> astar;
	std::vector<RVector3> result;
};

class SwordAttackNode : public bTreesys::BehaviorActionNode
{
public:

	SwordAttackNode() {};
	~SwordAttackNode() {};

	void SetPointer(SwordEnemy *enemy, std::shared_ptr<Player> player) {
		this->enemy = enemy;
		this->player = player;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//�Q�Ƃ���I�u�W�F�N�g
	SwordEnemy *enemy;
	std::weak_ptr<Player> player;

	RVector3 startPos, endPos;
	RV3Colider::Sphere atkSphere;
	int frame = 0;
	int easeFrame = 0;
	bool isAtkEnable = false;
	const int damage = 10;
};

class SwordChargeNode : public bTreesys::BehaviorActionNode
{
public:

	SwordChargeNode() {};
	~SwordChargeNode() {};

	void SetPointer(SwordEnemy *enemy, std::shared_ptr<Player> player) {
		this->enemy = enemy;
		this->player = player;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//�Q�Ƃ���I�u�W�F�N�g
	SwordEnemy *enemy;
	std::weak_ptr<Player> player;

	RVector3 startPos, endPos, chargeVec;
	int frame = 0;
	bool isAtkEnable = false;
	const int damage = 10;

};

class SwordAttackJudgeNode : public bTreesys::BehaviorJudgeNode
{
public:

	SwordAttackJudgeNode() {};
	~SwordAttackJudgeNode() {};

	void SetPointer(SwordEnemy *enemy, std::shared_ptr<Player> player) {
		this->enemy = enemy;
		this->player = player;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//�Q�Ƃ���I�u�W�F�N�g
	SwordEnemy *enemy;
	std::weak_ptr<Player> player;
};
