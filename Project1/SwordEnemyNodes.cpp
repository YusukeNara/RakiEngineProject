#include "SwordEnemyNodes.h"
#include <Behavior_ActionBase.h>


bool Sword_WaitJudge::Judge()
{
    //その他前提条件があればここに
    judgeScriptName = "WaitMoveJudge";

    //離れているとき
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
    enemy->swordObject->SetAffineParamRotate(degreeRotate(enemy->player->pos, enemy->pos));

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
    moveVec *= 0.8;

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
    if (frame == 60) { 
        isAtkEnable = true; 
        atkSphere.center = endPos;
    }
    if (isAtkEnable) {
        //攻撃命中
        if (RV3Colider::Colision2Sphere(atkSphere, enemy->player->bodyColider)) {
            isAtkEnable = false;
            enemy->player->hitpoint -= damage;
        }
    }


    return actionState;
}

void Sword_AttackAct::Init()
{

    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy->pos;
    endPos = startPos + (enemy->player->pos - enemy->pos) * 0.7f;

    actScriptName = "attackAct";

    isAtkEnable = false;

    frame = 0;
}

bool Sword_ChargeJudge::Judge()
{
    return distance(enemy->pos, enemy->player->pos) < 75.0f;
}

BehaviorActionBase::ACTION_STATE Sword_ChargeAct::Run()
{
    //突進
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    //一定フレーム経過で成功
    if (frame > 150) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //移動ベクトルを加算
    if (frame < 100 && frame > 40) {
        //float t = float(frame) / float(chargeFrame);
        //enemy->pos = Rv3Ease::lerp(startPos, endPos, t);
        enemy->pos += chargeVec * 2;
        //ダメージと判定無効化
        if (RV3Colider::Colision2Sphere(enemy->bodyColision, enemy->player->bodyColider) && isAtkEnable) {
            enemy->player->hitpoint -= damage;
            isAtkEnable = false;
        }
    }

    return actionState;
}

void Sword_ChargeAct::Init()
{
    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy->pos;
    endPos = enemy->player->pos + (enemy->player->pos - enemy->pos);

    //
    chargeVec = enemy->player->pos - enemy->pos;
    chargeVec = chargeVec.norm();

    actScriptName = "chargeAct";

    isAtkEnable = true;

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
    return  distance(enemy->pos, enemy->player->pos) > 50.0f;
}

SwordEnemyObject::~SwordEnemyObject()
{
    delete swordObject;
}

bool Sword_TolnadeJudge::Judge()
{
    return  distance(enemy->pos, enemy->player->pos) > 75.0f;
}

BehaviorActionBase::ACTION_STATE Sword_TolnadeAct::Run()
{
    return ACTION_STATE::FAILED;
}

void Sword_TolnadeAct::Init()
{
}
