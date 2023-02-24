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

	void SetPointer(std::shared_ptr<SwordEnemy> enemy, std::shared_ptr<Player> player,std::shared_ptr<NavMeshAstar> astar) {
		this->enemy = enemy;
		this->player = player;
		this->astar = astar;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//参照するオブジェクト
	std::weak_ptr<SwordEnemy> enemy;
	std::weak_ptr<Player> player;
	std::weak_ptr<NavMeshAstar> astar;
};

class SwordAttackNode : public bTreesys::BehaviorActionNode
{
public:

	SwordAttackNode() {};
	~SwordAttackNode() {};

	void SetPointer(std::shared_ptr<SwordEnemy> enemy, std::shared_ptr<Player> player) {
		this->enemy = enemy;
		this->player = player;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//参照するオブジェクト
	std::weak_ptr<SwordEnemy> enemy;
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

	void SetPointer(std::shared_ptr<SwordEnemy> enemy, std::shared_ptr<Player> player) {
		this->enemy = enemy;
		this->player = player;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//参照するオブジェクト
	std::weak_ptr<SwordEnemy> enemy;
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

	void SetPointer(std::shared_ptr<SwordEnemy> enemy, std::shared_ptr<Player> player) {
		this->enemy = enemy;
		this->player = player;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//参照するオブジェクト
	std::weak_ptr<SwordEnemy> enemy;
	std::weak_ptr<Player> player;
};
