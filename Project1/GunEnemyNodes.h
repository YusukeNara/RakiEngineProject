#pragma once
#include <BehaviorTree.h>
#include <Behavior_JudgeBase.h>
#include <RVector.h>
#include <NY_Object3DMgr.h>
#include "Player.h"
#include <string>
#include <GameObject.h>

class Gun_WaitJudge : public BehaviorJudgeBase
{
public:
	Gun_WaitJudge(GameObject *object,Player *player,RVector3 *pos) {
		this->object = object;
		this->player = player;
		judgeScriptName = "waitjudge";
	};
	~Gun_WaitJudge() = default;

	virtual bool Judge() override;

	GameObject *object;
	Player* player;

	RVector3 *pos;
};

class Gun_WaitAct : public BehaviorActionBase
{
public:
	Gun_WaitAct(GameObject* object, Player* player, RVector3 *pos) {
		this->object = object;
		this->player = player;
		this->pos = pos;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	GameObject* object;
	Player* player;

	int frame = 0;

	RVector3 moveVec;

	RVector3 *pos;
};



class Gun_ShotJudge : public BehaviorJudgeBase
{
public:
	Gun_ShotJudge(GameObject* object, Player* player, RVector3* pos) {
		this->object = object;
		this->player = player;
		this->pos = pos;
		judgeScriptName = "waitjudge";
	};
	~Gun_ShotJudge() = default;

	virtual bool Judge() override;

	GameObject* object;
	Player* player;

	RVector3* pos;
};

class Gun_ShotAct : public BehaviorActionBase
{
public:
	Gun_ShotAct(GameObject* object, Player* player, RVector3* pos) {
		this->object = object;
		this->player = player;
		this->pos = pos;
		Init();
	}

	virtual ACTION_STATE Run() override;

	virtual void Init() override;

	GameObject* object;
	Player* player;

	int frame = 0;

	RVector3 moveVec;

	RVector3 *pos;
};

