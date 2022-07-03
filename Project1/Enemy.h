#pragma once

#include <NY_Object3D.h>
#include <RVector.h>
#include <BehaviorTree.h>

#include <Behavior_JudgeBase.h>


class Enemy
{
public:
	//���W
	RVector3 pos;
	//�ړ��x�N�g��
	RVector3 vec;
	//�`��I�u�W�F�N�g
	Object3d* object;

	void Init(Object3d* enemyObject);

	//�`��
	void Draw();

};

//�ȉ��A�r�w�C�r�A�c���[�h���N���X

//�ړ����邩�ǂ������肷��I�u�W�F�N�g
class MoveJudgeNode : public BehaviorJudgeBase
{
public:
	//�R���X�g���N�^
	MoveJudgeNode(Enemy* enemy,RVector3 *playerPos);

	//����N���X
	virtual bool Judge() override;

	//�G�̃|�C���^
	Enemy* enemy;

	//�v���C���[�̍��W
	RVector3 *playerPos;

};
//�ړ��ōs���s�����`�����h���N���X

//�ڋߍs��
class ApproachingMoveAct : public BehaviorActionBase
{
public:
	ApproachingMoveAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	//�G�̃|�C���^
	Enemy* enemy;

	//�v���C���[�̍��W
	RVector3* playerPos;

	//�s������
	int actFrame = 180;
};

//�ޔ��s��
class RetreatMoveAct : public BehaviorActionBase
{
public:

	RetreatMoveAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	//�G�̃|�C���^
	Enemy* enemy;

	//�v���C���[�̍��W
	RVector3* playerPos;

	//�s������
	int actFrame = 180;
};

//�l�q��
class WaitAct : public BehaviorActionBase
{
public:

	WaitAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	//�G�̃|�C���^
	Enemy* enemy;

	//�v���C���[�̍��W
	RVector3* playerPos;

	//�s������
	int actFrame = 120;
};

//�U�����邩�ǂ������肷��I�u�W�F�N�g
class AttackJudge : public BehaviorJudgeBase
{
public:
	//�R���X�g���N�^
	AttackJudge(Enemy* enemy, RVector3* playerPos);

	//����N���X
	virtual bool Judge() override;

	//�G�̃|�C���^
	Enemy* enemy;

	//�v���C���[�̍��W
	RVector3* playerPos;
};

//�ˌ��U������I�u�W�F�N�g
class ShotAttackAct : public BehaviorActionBase
{
public:

	ShotAttackAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	//�G�̃|�C���^
	Enemy* enemy;

	//�v���C���[�̍��W
	RVector3* playerPos;

};

//�ːi�U������I�u�W�F�N�g
class ChargeAttackAct : public BehaviorActionBase
{
	ChargeAttackAct(Enemy* enemy, RVector3* playerPos);

	virtual ACTION_STATE Run() override;

	//�G�̃|�C���^
	Enemy* enemy;

	//�v���C���[�̍��W
	RVector3* playerPos;
};
