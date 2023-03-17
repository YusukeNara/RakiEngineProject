#include "NewSwordEnemyNodes.h"
#include "SwordEnemy.h"

using namespace bTreesys;

void SwordApproachNode::Init()
{
    astar.lock()->NavMeshSearchAster(enemy->pos, player.lock()->pos, result);
}

bTreesys::NODE_STATUS SwordApproachNode::Run()
{
    bTreesys::NODE_STATUS status = STATE_RUN;
    bool isMoved = false;
    int index = 0;

	//プレイヤーの近くまで接近したら成功
    if (distance(player.lock()->pos, enemy->pos) < 50.0f) {
        status = STATE_SUCCESS;
    }

    RVector3 vec(0.f, 0.f, 0.f);
    vec = astar.lock()->MoveWaypointDirectLine(result, enemy->pos, 20.0f, index, isMoved);

    enemy->pos += vec * 2;

    RVector3 lookVec = player.lock()->pos - enemy->pos;
    float angle = atan2f(lookVec.x, lookVec.z);
    enemy->object3d->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

	//何かしらの理由で接近できない場合は失敗
	return status;
}

void SwordAttackNode::Init()
{
    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy->pos;
    endPos = startPos + (player.lock()->pos - enemy->pos) * 0.7f;

    isAtkEnable = false;

    frame = 0;
}

bTreesys::NODE_STATUS SwordAttackNode::Run()
{
    frame++;

    auto actionState = bTreesys::NODE_STATUS::STATE_RUN;

    //一定フレーム経過で成功
    if (frame > 80) {
        actionState = bTreesys::NODE_STATUS::STATE_SUCCESS;
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
        if (RV3Colider::Colision2Sphere(atkSphere, player.lock()->bodyColider)) {
            isAtkEnable = false;
            player.lock()->PlayerDamaged(damage);
        }
    }

    RVector3 lookVec = player.lock()->pos - enemy->pos;
    float angle = atan2f(lookVec.x, lookVec.z);
    enemy->object3d->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    return actionState;
}

void SwordChargeNode::Init()
{
    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy->pos;
    endPos = player.lock()->pos + (player.lock()->pos - enemy->pos);

    //
    chargeVec = player.lock()->pos - enemy->pos;
    chargeVec = chargeVec.norm();

    isAtkEnable = true;

    RVector3 lookVec = player.lock()->pos - enemy->pos;

    float angle = atan2f(lookVec.x, lookVec.z);

    frame = 0;
}

bTreesys::NODE_STATUS SwordChargeNode::Run()
{
    //突進
    frame++;

    bTreesys::NODE_STATUS actionState = bTreesys::NODE_STATUS::STATE_RUN;

    //一定フレーム経過で成功
    if (frame > 150) {
        actionState = bTreesys::STATE_SUCCESS;
    }

    //移動ベクトルを加算
    if (frame < 100 && frame > 40) {
        enemy->pos += chargeVec * 2;
        //ダメージと判定無効化
        //if (RV3Colider::Colision2Sphere(enemy->bodyColision, player.lock()->bodyColider) && isAtkEnable) {
        //    player.lock()->PlayerDamaged(damage);
        //    isAtkEnable = false;
        //}
    }

    return actionState;
}

void SwordAttackJudgeNode::Init()
{

}

bTreesys::NODE_STATUS SwordAttackJudgeNode::Run()
{
    if (distance(enemy->pos, player.lock()->pos) < 50.0f) {
        return bTreesys::STATE_SUCCESS;
    }
    else {
        return bTreesys::STATE_FAILED;
    }
}
