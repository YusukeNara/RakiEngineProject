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
};

//�ҋ@�m�[�h
class Sword_WaitJudge : public BehaviorJudgeBase
{
public:
	Sword_WaitJudge(SwordEnemyObject *enemy) {
		this->enemy = enemy;
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
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;
	
	SwordEnemyObject* enemy;

	int frame = 0;

	RVector3 moveVec;
};


//�ڋ�
class Sword_ApproachJudge : public BehaviorJudgeBase
{
public:
	Sword_ApproachJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

class Sword_ApproachingAct : public BehaviorActionBase
{
public:
	Sword_ApproachingAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
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
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	//�ڋߗp�C�[�W���O
	RVector3 startPos;
	RVector3 endPos;

	//�U���͈́i�R���W�����}�l�[�W���[���������Ă���{�����j
	
	//AABB


	const int easeFrame			= 60;
	const int atkMotionFrame	= 20;

	int frame = 0;

};

class Sword_ChargeAct : public BehaviorActionBase
{
public:
	Sword_ChargeAct(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	SwordEnemyObject* enemy;

	int frame = 0;

	//�ڋߗp�C�[�W���O
	RVector3 startPos;
	RVector3 endPos;

	const int chargeFrame = 100;

};

class Sword_ChargeJudge : public BehaviorJudgeBase
{
public:
	Sword_ChargeJudge(SwordEnemyObject* enemy) {
		this->enemy = enemy;
	}

	virtual bool Judge() override;

	SwordEnemyObject* enemy;
};

