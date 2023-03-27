#include "GunEnemyNodes.h"
#include "GunEnemy.h"

bool Gun_WaitJudge::Judge()
{
    return distance(object->pos, player->pos) > 100.0f;
}

BehaviorActionBase::ACTION_STATE Gun_WaitAct::Run()
{
    //プレイヤーに一定距離まで接近

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    bool isMoved = false;
    //
    RVector3 moveVec = navAstar->MoveWaypointDirectLine(0.0f, *pos, isMoved);
    RVector3 lookVec = moveVec;
    moveVec = moveVec * 2.0;

    *pos += moveVec;

    float angle = atan2f(lookVec.x, lookVec.z);
    object->object3d->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    object->object3d->SetAffineParamTranslate(*pos);
    object->pos = *pos;

    if (!isMoved) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    return actionState;
}

void Gun_WaitAct::Init()
{
    
    std::vector<RVector3> result;

    //ナビメッシュによる経路探索を実行
    navAstar->NavMeshSearchAster(*pos, player->pos, result);

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

//void GunEnemyShotNode::Init()
//{
//    atkSphere.center = enemy->pos;
//    atkSphere.rad = 10.0f;
//    frame = 0;
//}
//
//bTreesys::NODE_STATUS GunEnemyShotNode::Run()
//{
//    RVector3 lookVec;
//    lookVec = player.lock()->pos - enemy->pos;
//
//    float angle = atan2f(lookVec.x, lookVec.z);
//    enemy->rot = RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f);
//
//    frame++;
//}
//
//void GunEnemyMoveNode::Init()
//{
//}
//
//bTreesys::NODE_STATUS GunEnemyMoveNode::Run()
//{
//    return bTreesys::NODE_STATUS();
//}
