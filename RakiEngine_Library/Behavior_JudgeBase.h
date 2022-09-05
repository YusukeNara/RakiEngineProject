#pragma once
#include <string>

//����m�[�h���N���X
//�p�������N���X�̃|�C���^�^�I�u�W�F�N�g���g�p
class BehaviorJudgeBase
{
public:
	BehaviorJudgeBase() = default;
	~BehaviorJudgeBase(){};

	//�m�[�h���g�p���鏃�����z�֐�
	//�m�[�h�I��O�ɂ��̃N���X�����s����A�q�m�[�h�̎g�p�\���𔻒肷��
	//�m�[�h�I���̔���X�N���v�g�͔h���N���X�Œ�`
	virtual bool Judge() = 0;

	std::string judgeScriptName;
};