#pragma once
#include <BehaviorTree.h>
#include <Behavior_JudgeBase.h>
#include <RVector.h>
#include <NY_Object3DMgr.h>
#include "Player.h"
#include <string>
#include <GameObject.h>
#include <NavMeshAstar.h>
#include <BehaviorNodeBase.h>


class Gun_WaitJudge : public BehaviorJudgeBase
{
public:
	Gun_WaitJudge(GameObject *object,Player *player,RVector3 *pos) {
		this->object = object;
		this->player = player;
		judgeScriptName = "waitjudge";
	};
	~Gun_WaitJudge() = default;

	virtual bool Judge() override;

	GameObject *object;
	Player* player;

	RVector3 *pos;
};

class Gun_WaitAct : public BehaviorActionBase
{
public:
	Gun_WaitAct(GameObject* object, Player* player, RVector3 *pos,NavMeshAstar *na) {
		this->object = object;
		this->player = player;
		this->pos = pos;
		this->navAstar = na;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	GameObject* object;
	Player* player;

	int frame = 0;

	RVector3 moveVec;

	RVector3 *pos;

	NavMeshAstar* navAstar;
};



class Gun_ShotJudge : public BehaviorJudgeBase
{
public:
	Gun_ShotJudge(GameObject* object, Player* player, RVector3* pos) {
		this->object = object;
		this->player = player;
		this->pos = pos;
		judgeScriptName = "waitjudge";
	};
	~Gun_ShotJudge() = default;

	virtual bool Judge() override;

	GameObject* object;
	Player* player;

	RVector3* pos;
};

class Gun_ShotAct : public BehaviorActionBase
{
public:
	Gun_ShotAct(GameObject* object, Player* player, RVector3* pos) {
		this->object = object;
		this->player = player;
		this->pos = pos;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	GameObject* object;
	Player* player;

	int frame = 0;

	RVector3 moveVec;

	RVector3 *pos;
};

////射撃ノード
//class GunEnemyShotNode : public bTreesys::BehaviorActionNode
//{
//public:
//	GunEnemyShotNode() {};
//	~GunEnemyShotNode() {};
//
//	void SetPointer(GameObject* enemy, const std::shared_ptr<Player>& player,ParticleManager *bp) {
//		this->enemy = enemy;
//		this->player = player;
//		this->bulletParticle = bp;
//	}
//
//	void Init() override;
//
//	bTreesys::NODE_STATUS Run() override;
//
//private:
//	//参照するオブジェクト
//	GameObject* enemy;
//	std::weak_ptr<Player> player;
//	ParticleManager* bulletParticle;
//	ParticleManager* blastParticle;
//
//	RVector3 startPos, endPos;
//	RV3Colider::Sphere atkSphere;
//	int frame = 0;
//	int easeFrame = 0;
//	bool isAtkEnable = false;
//	const int damage = 10;
//};
//
////移動ノード
//class GunEnemyMoveNode : public bTreesys::BehaviorActionNode
//{
//public:
//	GunEnemyMoveNode() {};
//	~GunEnemyMoveNode() {};
//
//	void SetPointer(GameObject* enemy, const std::shared_ptr<Player>& player, ParticleManager* bp) {
//		this->enemy = enemy;
//		this->player = player;
//	}
//
//	void Init() override;
//
//	bTreesys::NODE_STATUS Run() override;
//
//private:
//
//	//参照するオブジェクト
//	GameObject* enemy;
//	std::weak_ptr<Player> player;
//};

