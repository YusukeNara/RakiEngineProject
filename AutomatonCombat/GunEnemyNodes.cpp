#include "GunEnemyNodes.h"
#include "GunEnemy.h"

bool Gun_WaitJudge::Judge()
{
    return distance(object->pos, player->pos) > 100.0f;
}

BehaviorActionBase::ACTION_STATE Gun_WaitAct::Run()
{
    //�v���C���[�Ɉ�苗���܂Őڋ�

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    RVector3 moveVec;
    moveVec = player->pos - *pos;
    RVector3 lookVec = moveVec;
    moveVec = moveVec.norm() * 1.5;

    *pos += moveVec;

    float angle = atan2f(lookVec.x, lookVec.z);
    object->object3d->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    object->object3d->SetAffineParamTranslate(*pos);
    object->pos = *pos;

    if (distance(*pos, player->pos) < 70.0f) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    return actionState;
}

void Gun_WaitAct::Init()
{
    //�v���C���[������




}

bool Gun_ShotJudge::Judge()
{
    return distance(object->pos, player->pos) < 100.0f;
}

BehaviorActionBase::ACTION_STATE Gun_ShotAct::Run()
{
    RVector3 lookVec;
    lookVec = player->pos - *pos;

    float angle = atan2f(lookVec.x, lookVec.z);
    object->object3d->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    object->object3d->SetAffineParamTranslate(*pos);
    object->pos = *pos;

    frame++;

    if (frame == 60) {
        dynamic_cast<GunEnemy*>(object)->ShotBullet();
    }

    else if(frame > 120){
        return ACTION_STATE::SUCCESS;
    }


    return ACTION_STATE::RUNNING;
}

void Gun_ShotAct::Init()
{
    frame = 0;
}
