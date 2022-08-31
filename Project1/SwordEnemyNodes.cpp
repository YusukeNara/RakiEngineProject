#include "SwordEnemyNodes.h"
#include <Behavior_ActionBase.h>


bool Sword_WaitJudge::Judge()
{
    //���̑��O�����������΂�����
    judgeScriptName = "WaitMoveJudge";

    //�������߂��Ƃ�����
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



    return actionState;
}

void Sword_WaitAct::Init()
{
    frame = 0;

    moveVec.zero();

    moveVec = enemy->player->pos - enemy->pos;
    moveVec = moveVec.norm();

    //xz�ɂ��āA90�x�Ȃ����x�N�g�������
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


    return actionState;
}

void Sword_AttackAct::Init()
{

    //�v���C���[�Ƃ̋������v�Z���A�C�[�W���O�p���W��ݒ�
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
    //�ːi
    frame++;

    actionState = BehaviorActionBase::ACTION_STATE::RUNNING;

    //���t���[���o�߂Ő���
    if (frame > 80) {
        actionState = BehaviorActionBase::ACTION_STATE::SUCCESS;
    }

    //�C�[�W���O�Őڋ�
    if (frame < 60) {
        float t = float(frame) / float(chargeFrame);
        enemy->pos = Rv3Ease::lerp(startPos, endPos, t);
    }

    return actionState;
}

void Sword_ChargeAct::Init()
{
    //�v���C���[�Ƃ̋������v�Z���A�C�[�W���O�p���W��ݒ�
    startPos = enemy->pos;
    endPos = enemy->player->pos + (enemy->player->pos - enemy->pos);

    actScriptName = "chargeAct";

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

    return actionState;
}

void Sword_ApproachingAct::Init()
{
    actScriptName = "approachAct";

}

bool Sword_ApproachJudge::Judge()
{
    //����Ă�������ɓ���
    return  distance(enemy->pos, enemy->player->pos) > 75.0f;
}


