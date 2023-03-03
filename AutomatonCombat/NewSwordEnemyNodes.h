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

	/// ポインタメモ
	/// このファイルのクラスはswordEnemyが持っている
	/// playerとastarはswordEnemyの外部のクラス
	/// つまりenemyは内側のスコープとなるので生ポインタで参照でも良い？
	/// playerとastarは外側スコープなのでスマートポインタにしなければならない

	void SetPointer(SwordEnemy *enemy, std::shared_ptr<Player> player,std::shared_ptr<NavMeshAstar> astar) {
		this->enemy = enemy;
		this->player = player;
		this->astar = astar;
	}

	void Init() override;

	bTreesys::NODE_STATUS Run() override;

private:
	//参照するオブジェクト
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
	//参照するオブジェクト
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
	//参照するオブジェクト
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
	//参照するオブジェクト
	SwordEnemy *enemy;
	std::weak_ptr<Player> player;
};
