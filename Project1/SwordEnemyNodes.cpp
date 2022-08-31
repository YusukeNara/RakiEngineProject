#include "SwordEnemyNodes.h"
#include <Behavior_ActionBase.h>


bool Sword_WaitJudge::Judge()
{
    //その他前提条件があればここに
    judgeScriptName = "WaitMoveJudge";

    //距離が近いとき候補に
    return distance(enemy->pos, enemy->player->pos) < 50.0f;
}

BehaviorActionBase::ACTION_STATE Sword_WaitAct::Run()
{
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    if (frame > 90) {
        //行動成功を返す
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //様子を見るようにじわじわ移動する
    enemy->pos += moveVec;



    return actionState;
}

void Sword_WaitAct::Init()
{
    frame = 0;

    moveVec.zero();

    moveVec = enemy->player->pos - enemy->pos;
    moveVec = moveVec.norm();

    //xzについて、90度曲げたベクトルを作る
    RVector3 rotVec = moveVec;
    rotVec.x = moveVec.z;
    rotVec.z = -moveVec.x;

    moveVec = rotVec;
    moveVec.y = 0.0f;
    moveVec *= 1;

    actScriptName = "waitAction";
}

bool Sword_AttackJudge::Judge()
{
    //待機モーションのあとに実行する
    if (waitAction->actionState != BehaviorActionBase::ACTION_STATE::SUCCESS) {
        return false;
    }

    return true;
}


BehaviorActionBase::ACTION_STATE Sword_AttackAct::Run()
{
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    //一定フレーム経過で成功
    if (frame > 80) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //イージングで接近
    if (frame < 60) {
        float t = float(frame) / float(easeFrame);
        enemy->pos = Rv3Ease::OutQuad(startPos, endPos, t);
    }


    return actionState;
}

void Sword_AttackAct::Init()
{

    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy->pos;
    endPos = enemy->player->pos;

    actScriptName = "attackAct";

    frame = 0;
}

bool Sword_ChargeJudge::Judge()
{
    return true;
}

BehaviorActionBase::ACTION_STATE Sword_ChargeAct::Run()
{
    //突進
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    //一定フレーム経過で成功
    if (frame > 80) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //イージングで接近
    if (frame < 60) {
        float t = float(frame) / float(chargeFrame);
        enemy->pos = Rv3Ease::lerp(startPos, endPos, t);
    }

    return actionState;
}

void Sword_ChargeAct::Init()
{
    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy->pos;
    endPos = enemy->player->pos + (enemy->player->pos - enemy->pos);

    actScriptName = "chargeAct";

    frame = 0;
}

BehaviorActionBase::ACTION_STATE Sword_ApproachingAct::Run()
{
    //プレイヤーに一定距離まで接近

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    RVector3 moveVec;
    moveVec = enemy->player->pos - enemy->pos;
    moveVec = moveVec.norm() * 1.5;

    enemy->pos += moveVec;

    if (distance(enemy->pos, enemy->player->pos) < 50.0f) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    return actionState;
}

void Sword_ApproachingAct::Init()
{
    actScriptName = "approachAct";

}

bool Sword_ApproachJudge::Judge()
{
    //離れていたら候補に入る
    return  distance(enemy->pos, enemy->player->pos) > 75.0f;
}


