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
	//当たり判定
	RV3Colider::Sphere bodyColision;

	~SwordEnemyObject();
};

//不本意だが、現時点では敵の攻撃の衝突判定もここでやるしかない？
//待機ノード
class Sword_WaitJudge : public BehaviorJudgeBase
{
public:
	Sword_WaitJudge(SwordEnemyObject *enemy) {
		this->enemy = enemy;
		judgeScriptName = "waitjudge";
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
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;
	
	SwordEnemyObject* enemy;

	int frame = 0;

	RVector3 moveVec;
	RVector3 lookVec;
};


//接近
class Sword_ApproachJudge : public BehaviorJudgeBase
{
public:
	Sword_ApproachJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		judgeScriptName = "approachJudge";
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_ApproachingAct : public BehaviorActionBase
{
public:
	Sword_ApproachingAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		Init();
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
		judgeScriptName = "atkJudge";
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
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	//接近用イージング
	RVector3 startPos;
	RVector3 endPos;

	//攻撃範囲（コリジョンマネージャーが完成してから本実装）
	RV3Colider::Sphere atkSphere;

	const int easeFrame			= 60;
	const int atkMotionFrame	= 20;

	int damage = 1;
	bool isAtkEnable = false;

	int frame = 0;

};

class Sword_ChargeAct : public BehaviorActionBase
{
public:
	Sword_ChargeAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	int frame = 0;

	//接近用イージング
	RVector3 startPos;
	RVector3 endPos;

	//ベクトル
	RVector3 chargeVec;



	//判定有効
	bool isAtkEnable = false;

	int damage = 2;

	const int chargeFrame = 100;

};

class Sword_ChargeJudge : public BehaviorJudgeBase
{
public:
	Sword_ChargeJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		judgeScriptName = "chargeJudge";
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_TolnadeJudge : public BehaviorJudgeBase
{
public:
	Sword_TolnadeJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		judgeScriptName = "tolJudge";
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_TolnadeAct : public BehaviorActionBase {
public:
	Sword_TolnadeAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		actScriptName = "tolJudge";

	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;
};

