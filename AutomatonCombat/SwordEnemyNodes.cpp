#include "SwordEnemyNodes.h"
#include <Behavior_ActionBase.h>


bool Sword_WaitJudge::Judge()
{
    //���̑��O�����������΂�����
    judgeScriptName = "WaitMoveJudge";

    //����Ă���Ƃ�
    return distance(enemy->pos, enemy->player->pos) < 50.0f;
}

BehaviorActionBase::ACTION_STATE Sword_WaitAct::Run()
{
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    if (frame > 90) {
        //�s��������Ԃ�
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //�l�q������悤�ɂ��킶��ړ�����
    enemy->pos += moveVec;
    lookVec = enemy->player->pos - enemy->pos;

    float angle = atan2f(lookVec.x, lookVec.z);
    enemy->swordObject->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    return actionState;
}

void Sword_WaitAct::Init()
{
    frame = 0;

    moveVec.zero();

    moveVec = enemy->player->pos - enemy->pos;
    lookVec = moveVec;
    moveVec = moveVec.norm();

    //xz�ɂ��āA90�x�Ȃ����x�N�g�������
    RVector3 rotVec = moveVec;
    rotVec.x = moveVec.z;
    rotVec.z = -moveVec.x;

    moveVec = rotVec;
    moveVec.y = 0.0f;
    moveVec *= 0.8f;

    actScriptName = "waitAction";
}

bool Sword_AttackJudge::Judge()
{
    //�ҋ@���[�V�����̂��ƂɎ��s����
    if (waitAction->actionState != BehaviorActionBase::ACTION_STATE::SUCCESS) {
        return false;
    }

    return true;
}


BehaviorActionBase::ACTION_STATE Sword_AttackAct::Run()
{
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    //���t���[���o�߂Ő���
    if (frame > 80) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //�C�[�W���O�Őڋ�
    if (frame < 60) {
        float t = float(frame) / float(easeFrame);
        enemy->pos = Rv3Ease::OutQuad(startPos, endPos, t);
    }
    if (frame == 60) { 
        isAtkEnable = true; 
        atkSphere.center = endPos;
    }
    if (isAtkEnable) {
        //�U������
        if (RV3Colider::Colision2Sphere(atkSphere, enemy->player->bodyColider)) {
            isAtkEnable = false;
            enemy->player->PlayerDamaged(damage);
        }
    }

    RVector3 lookVec = enemy->player->pos - enemy->pos;
    float angle = atan2f(lookVec.x, lookVec.z);
    enemy->swordObject->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    return actionState;
}

void Sword_AttackAct::Init()
{

    //�v���C���[�Ƃ̋������v�Z���A�C�[�W���O�p���W��ݒ�
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
    //�ːi
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    //���t���[���o�߂Ő���
    if (frame > 150) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //�ړ��x�N�g�������Z
    if (frame < 100 && frame > 40) {
        //float t = float(frame) / float(chargeFrame);
        //enemy->pos = Rv3Ease::lerp(startPos, endPos, t);
        enemy->pos += chargeVec * 2;
        //�_���[�W�Ɣ��薳����
        if (RV3Colider::Colision2Sphere(enemy->bodyColision, enemy->player->bodyColider) && isAtkEnable) {
            enemy->player->PlayerDamaged(damage);
            isAtkEnable = false;
        }
    }

    return actionState;
}

void Sword_ChargeAct::Init()
{
    //�v���C���[�Ƃ̋������v�Z���A�C�[�W���O�p���W��ݒ�
    startPos = enemy->pos;
    endPos = enemy->player->pos + (enemy->player->pos - enemy->pos);

    //
    chargeVec = enemy->player->object3d->position - enemy->pos;
    chargeVec = chargeVec.norm();

    actScriptName = "chargeAct";

    isAtkEnable = true;

    RVector3 lookVec = enemy->player->pos - enemy->pos;

    float angle = atan2f(lookVec.x, lookVec.z);
    enemy->swordObject->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    frame = 0;
}

BehaviorActionBase::ACTION_STATE Sword_ApproachingAct::Run()
{
    //�v���C���[�Ɉ�苗���܂Őڋ�

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    RVector3 moveVec;
    moveVec = enemy->player->pos - enemy->pos;
    moveVec = moveVec.norm() * 1.5;

    enemy->pos += moveVec;

    if (distance(enemy->pos, enemy->player->pos) < 50.0f) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    RVector3 lookVec = enemy->player->pos - enemy->pos;
    float angle = atan2f(lookVec.x, lookVec.z);
    enemy->swordObject->SetAffineParamRotate(RVector3(0, (180.0f / 3.14f) * (angle + 3.14f), 0.0f));

    return actionState;
}

void Sword_ApproachingAct::Init()
{
    actScriptName = "approachAct";

}

bool Sword_ApproachJudge::Judge()
{
    //����Ă�������ɓ���
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