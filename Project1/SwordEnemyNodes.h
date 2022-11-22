#pragma once

#include <BehaviorTree.h>
#include <Behavior_JudgeBase.h>

#include <RVector.h>
#include <NY_Object3DMgr.h>

#include "Player.h"

#include <string>
using namespace std;

//�m�[�h�ƍs����`
//���W�A�`��I�u�W�F�N�g�A�p�����[�^�[������
class SwordEnemyObject
{
public:

	//���W
	RVector3 pos;
	//�����x�N�g��
	RVector3 lookVec;
	//�`��I�u�W�F�N�g
	Object3d* swordObject;
	//�v���C���[���W
	Player* player;
	//�����蔻��
	RV3Colider::Sphere bodyColision;

	~SwordEnemyObject();
};

//�s�{�ӂ����A�����_�ł͓G�̍U���̏Փ˔���������ł�邵���Ȃ��H
//�ҋ@�m�[�h
class Sword_WaitJudge : public BehaviorJudgeBase
{
public:
	Sword_WaitJudge(SwordEnemyObject *enemy) {
		this->enemy = enemy;
		judgeScriptName = "waitjudge";
	};
	~Sword_WaitJudge() = default;

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_WaitAct : public BehaviorActionBase
{
public:
	Sword_WaitAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;
	
	SwordEnemyObject* enemy;

	int frame = 0;

	RVector3 moveVec;
	RVector3 lookVec;
};


//�ڋ�
class Sword_ApproachJudge : public BehaviorJudgeBase
{
public:
	Sword_ApproachJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		judgeScriptName = "approachJudge";
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_ApproachingAct : public BehaviorActionBase
{
public:
	Sword_ApproachingAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;
};



//�U��
class Sword_AttackJudge : public BehaviorJudgeBase
{
public:
	Sword_AttackJudge(SwordEnemyObject* enemy,BehaviorActionBase *waitAct) {
		this->enemy = enemy;
		waitAction = waitAct;
		judgeScriptName = "atkJudge";
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;

	BehaviorActionBase* waitAction;
};


//�ːi
class Sword_AttackAct : public BehaviorActionBase
{
public:
	Sword_AttackAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	//�ڋߗp�C�[�W���O
	RVector3 startPos;
	RVector3 endPos;

	//�U���͈́i�R���W�����}�l�[�W���[���������Ă���{�����j
	RV3Colider::Sphere atkSphere;

	const int easeFrame			= 60;
	const int atkMotionFrame	= 20;

	int damage = 1;
	bool isAtkEnable = false;

	int frame = 0;

};

class Sword_ChargeAct : public BehaviorActionBase
{
public:
	Sword_ChargeAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	int frame = 0;

	//�ڋߗp�C�[�W���O
	RVector3 startPos;
	RVector3 endPos;

	//�x�N�g��
	RVector3 chargeVec;



	//����L��
	bool isAtkEnable = false;

	int damage = 2;

	const int chargeFrame = 100;

};

class Sword_ChargeJudge : public BehaviorJudgeBase
{
public:
	Sword_ChargeJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		judgeScriptName = "chargeJudge";
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_TolnadeJudge : public BehaviorJudgeBase
{
public:
	Sword_TolnadeJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		judgeScriptName = "tolJudge";
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_TolnadeAct : public BehaviorActionBase {
public:
	Sword_TolnadeAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
		actScriptName = "tolJudge";

	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;
};

