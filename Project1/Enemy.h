#pragma once

#include <NY_Object3D.h>
#include <RVector.h>
#include <BehaviorTree.h>

#include <Behavior_JudgeBase.h>


class Enemy
{
public:
	//座標
	RVector3 pos;
	//移動ベクトル
	RVector3 vec;
	//描画オブジェクト
	Object3d* object;

	void Init(Object3d* enemyObject);

	//描画
	void Draw();

};

//以下、ビヘイビアツリー派生クラス

//移動するかどうか判定するオブジェクト
class MoveJudgeNode : public BehaviorJudgeBase
{
public:
	//コンストラクタ
	MoveJudgeNode(Enemy* enemy,RVector3 *playerPos);

	//判定クラス
	virtual bool Judge() override;

	//敵のポインタ
	Enemy* enemy;

	//プレイヤーの座標
	RVector3 *playerPos;

};
//移動で行う行動を定義した派生クラス

//接近行動
class ApproachingMoveAct : public BehaviorActionBase
{
public:
	ApproachingMoveAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	//敵のポインタ
	Enemy* enemy;

	//プレイヤーの座標
	RVector3* playerPos;

	//行動時間
	int actFrame = 180;
};

//退避行動
class RetreatMoveAct : public BehaviorActionBase
{
public:

	RetreatMoveAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	//敵のポインタ
	Enemy* enemy;

	//プレイヤーの座標
	RVector3* playerPos;

	//行動時間
	int actFrame = 180;
};

//様子見
class WaitAct : public BehaviorActionBase
{
public:

	WaitAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	//敵のポインタ
	Enemy* enemy;

	//プレイヤーの座標
	RVector3* playerPos;

	//行動時間
	int actFrame = 120;
};

//攻撃するかどうか判定するオブジェクト
class AttackJudge : public BehaviorJudgeBase
{
public:
	//コンストラクタ
	AttackJudge(Enemy* enemy, RVector3* playerPos);

	//判定クラス
	virtual bool Judge() override;

	//敵のポインタ
	Enemy* enemy;

	//プレイヤーの座標
	RVector3* playerPos;
};

//射撃攻撃するオブジェクト
class ShotAttackAct : public BehaviorActionBase
{
public:

	ShotAttackAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	//敵のポインタ
	Enemy* enemy;

	//プレイヤーの座標
	RVector3* playerPos;

};

//突進攻撃するオブジェクト
class ChargeAttackAct : public BehaviorActionBase
{
	ChargeAttackAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	//敵のポインタ
	Enemy* enemy;

	//プレイヤーの座標
	RVector3* playerPos;
};
