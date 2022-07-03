#include "Enemy.h"

MoveJudgeNode::MoveJudgeNode(Enemy* enemy, RVector3* playerPos)
{
	//操作する敵
	this->enemy = enemy;
	//プレイヤーの座標
	this->playerPos = playerPos;
}

bool MoveJudgeNode::Judge()
{
	return true;
}

void Enemy::Init(Object3d* enemyObject)
{
	pos = RVector3(0, 0, 100);
}

void Enemy::Draw()
{
	//座標設定
	object->SetAffineParamTranslate(pos);
	//描画
	object->DrawObject();
}










ApproachingMoveAct::ApproachingMoveAct(Enemy* enemy, RVector3* playerPos)
{
	this->enemy = enemy;
	this->playerPos = playerPos;
}

BehaviorActionBase::ACTION_STATE ApproachingMoveAct::Run()
{
	if (actFrame < 0) {
		return ACTION_STATE::SUCCESS;
	}

	actFrame--;

	return ACTION_STATE::RUNNING;
}

void ApproachingMoveAct::Init()
{
	actFrame = 120;
	actionState = ACTION_STATE::STANDBY;
}

RetreatMoveAct::RetreatMoveAct(Enemy* enemy, RVector3* playerPos)
{
	this->enemy = enemy;
	this->playerPos = playerPos;
}

BehaviorActionBase::ACTION_STATE RetreatMoveAct::Run()
{
	if (actFrame < 0) {
		return ACTION_STATE::SUCCESS;
	}

	actFrame--;

	return ACTION_STATE::RUNNING;
}

void RetreatMoveAct::Init()
{
	actionState = ACTION_STATE::STANDBY;
	actFrame = 180;
}

WaitAct::WaitAct(Enemy* enemy, RVector3* playerPos)
{
	this->enemy = enemy;
	this->playerPos = playerPos;


}

BehaviorActionBase::ACTION_STATE WaitAct::Run()
{
	if (actFrame < 0) {
		return ACTION_STATE::SUCCESS;
	}

	actFrame--;

	return ACTION_STATE::RUNNING;
}

void WaitAct::Init()
{
	actionState = ACTION_STATE::STANDBY;
	actFrame = 180;
}






AttackJudge::AttackJudge(Enemy* enemy, RVector3* playerPos)
{
	this->enemy = enemy;
	this->playerPos = playerPos;
}

bool AttackJudge::Judge()
{
	return false;
}

ShotAttackAct::ShotAttackAct(Enemy* enemy, RVector3* playerPos)
{
	this->enemy = enemy;
	this->playerPos = playerPos;
}

BehaviorActionBase::ACTION_STATE ShotAttackAct::Run()
{
	return ACTION_STATE();
}

BehaviorActionBase::ACTION_STATE ChargeAttackAct::Run()
{
	return ACTION_STATE();
}
