#pragma once

#include <BehaviorTree.h>
#include <Behavior_JudgeBase.h>

#include <RVector.h>
#include <NY_Object3DMgr.h>

#include "Player.h"

#include <string>
using namespace std;

//ノードと行動定義
//座標、描画オブジェクト、パラメーターを持つ
class SwordEnemyObject
{
public:

	//座標
	RVector3 pos;
	//方向ベクトル
	RVector3 lookVec;
	//描画オブジェクト
	Object3d* swordObject;
	//プレイヤー座標
	Player* player;
};

//待機ノード
class Sword_WaitJudge : public BehaviorJudgeBase
{
public:
	Sword_WaitJudge(SwordEnemyObject *enemy) {
		this->enemy = enemy;
	};
	~Sword_WaitJudge() = default;

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_WaitAct : public BehaviorActionBase
{
public:
	Sword_WaitAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;
	
	SwordEnemyObject* enemy;

	int frame = 0;

	RVector3 moveVec;
};


//接近
class Sword_ApproachJudge : public BehaviorJudgeBase
{
public:
	Sword_ApproachJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_ApproachingAct : public BehaviorActionBase
{
public:
	Sword_ApproachingAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;
};



//攻撃
class Sword_AttackJudge : public BehaviorJudgeBase
{
public:
	Sword_AttackJudge(SwordEnemyObject* enemy,BehaviorActionBase *waitAct) {
		this->enemy = enemy;
		waitAction = waitAct;
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;

	BehaviorActionBase* waitAction;
};


//突進
class Sword_AttackAct : public BehaviorActionBase
{
public:
	Sword_AttackAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	//接近用イージング
	RVector3 startPos;
	RVector3 endPos;

	//攻撃範囲（コリジョンマネージャーが完成してから本実装）
	
	//AABB


	const int easeFrame			= 60;
	const int atkMotionFrame	= 20;

	int frame = 0;

};

class Sword_ChargeAct : public BehaviorActionBase
{
public:
	Sword_ChargeAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	int frame = 0;

	//接近用イージング
	RVector3 startPos;
	RVector3 endPos;

	const int chargeFrame = 100;

};

class Sword_ChargeJudge : public BehaviorJudgeBase
{
public:
	Sword_ChargeJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

