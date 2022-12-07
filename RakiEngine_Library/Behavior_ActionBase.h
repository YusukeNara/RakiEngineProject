#pragma once

#include <string>

//�A�N�V�������N���X
class BehaviorActionBase
{
public:
	BehaviorActionBase() = default;
	virtual ~BehaviorActionBase() = default;

	//���s�N���X�̏��
	enum ACTION_STATE
	{
		STANDBY,
		RUNNING,	//���s��
		SUCCESS,	//����
		FAILED,		//���s
	};

	ACTION_STATE actionState = STANDBY;

	//���s�֐�
	virtual ACTION_STATE Run() = 0;

	virtual void Init() = 0;

	std::string actScriptName;
};