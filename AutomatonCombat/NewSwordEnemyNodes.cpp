#include "NewSwordEnemyNodes.h"


using namespace bTreesys;

void SwordApproachNode::Init()
{
	
}

bTreesys::NODE_STATUS SwordApproachNode::Run()
{
	//プレイヤーの近くまで接近したら成功



	//何かしらの理由で接近できない場合は失敗



	return bTreesys::NODE_STATUS();
}

void SwordAttackNode::Init()
{
    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy.lock()->pos;
    endPos = startPos + (player.lock()->pos - enemy.lock()->pos) * 0.7f;

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
        enemy.lock()->pos = Rv3Ease::OutQuad(startPos, endPos, t);
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

    RVector3 lookVec = player.lock()->pos - enemy.lock()->pos;
    float angle = atan2f(lookVec.x, lookVec.z);
    enemy.lock()->swordObject->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    return actionState;
}

void SwordChargeNode::Init()
{
    //プレイヤーとの距離を計算し、イージング用座標を設定
    startPos = enemy.lock()->pos;
    endPos = player.lock()->pos + (player.lock()->pos - enemy.lock()->pos);

    //
    chargeVec = player.lock()->pos - enemy.lock()->pos;
    chargeVec = chargeVec.norm();

    isAtkEnable = true;

    RVector3 lookVec = player.lock()->pos - enemy.lock()->pos;

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
        enemy.lock()->pos += chargeVec * 2;
        //ダメージと判定無効化
        if (RV3Colider::Colision2Sphere(enemy.lock()->bodyColision, player.lock()->bodyColider) && isAtkEnable) {
            player.lock()->PlayerDamaged(damage);
            isAtkEnable = false;
        }
    }
}

void SwordAttackJudgeNode::Init()
{

}

bTreesys::NODE_STATUS SwordAttackJudgeNode::Run()
{
    if (distance(enemy.lock()->pos, player.lock()->pos) < 50.0f) {
        return bTreesys::STATE_SUCCESS;
    }
    else {
        return bTreesys::STATE_FAILED;
    }
}
