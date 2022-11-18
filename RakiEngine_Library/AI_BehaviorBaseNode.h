#pragma once
#include "Behavior_ActionBase.h"


#include <string>
#include <vector>

class BehaviorJudgeBase;

//�r�w�C�r�A�̃m�[�h
//������actionBase,JudgeBase�̃|�C���^�����Ď��s����
//�A�v���P�[�V�������ł͂����̃|�C���^�ɃX�N���v�g�����Ď��s����
//���ʂ�����
class BehaviorBaseNode
{
public:
	//�m�[�h���胋�[��
	enum SELECT_RULE
	{
		RULE_RANDOM,	//�����_��
		RULE_PRIORITY,	//�D��x
		RULE_ONOFF,		//�I���I�t
		RULE_SEAQUENCE,	//�V�[�P���X
	};

	//�m�[�h�̎��
	enum NODE_TYPE
	{
		TYPE_EXECUTE,	//���s�m�[�h
		TYPE_SELECTER,	//�I���m�[�h
	};

public:
	BehaviorBaseNode() {};
	~BehaviorBaseNode() {
	};

	/// <summary>
	/// ����p�m�[�h�𐶐�
	/// </summary>
	/// <param name="nodeName">�m�[�h�̖��O</param>
	/// <param name="rule">�q�m�[�h�I�����[��</param>
	/// <param name="judgeObject">�m�[�h�I���������`�����h���N���X�I�u�W�F�N�g</param>
	void CreateJudgeNode(std::string nodeName, SELECT_RULE rule, BehaviorJudgeBase* judgeObject);

	/// <summary>
	/// ����m�[�h�Ɏq�m�[�h�ǉ�
	/// </summary>
	/// <param name="child"></param>
	void AddjudgeNodeChild(BehaviorBaseNode* child);

	/// <summary>
	/// ���s�m�[�h����
	/// </summary>
	/// <param name="nodeName">�m�[�h�̖��O</param>
	/// <param name="actObject">���s����h���N���X�I�u�W�F�N�g</param>
	void CreateActionNode(std::string nodeName, BehaviorActionBase* actObject, BehaviorJudgeBase* judgeObject);

	/// <summary>
	/// �m�[�h���_
	/// <para> �q�m�[�h�̔���N���X�����s���A���ƂȂ�m�[�h��I��</para>
	/// <para> ���m�[�h�̒�����I�����[���ɂ���đI�΂ꂽ�m�[�h��I��</para>
	/// <para> ������m�[�h�̖��[�܂ŌJ��Ԃ��֐�</para>
	/// </summary>
	/// <returns>���_�̌��ʂ���o�����[�g�m�[�h</returns>
	BehaviorBaseNode *Inference();

	/// <summary>
	/// �m�[�h���s
	/// <para>���s�N���X�̏������s��</para>
	/// <para>���s�N���X���Ȃ��ꍇ���s�����ɂȂ�</para>
	/// </summary>
	/// <returns>���s�N���X�̐i�s��</returns>
	BehaviorActionBase::ACTION_STATE Run();

	///�ȉ��̓m�[�h����p�֐�
	BehaviorBaseNode *Select_Random(std::vector<BehaviorBaseNode*> *lists);

	BehaviorBaseNode* Select_Priority(std::vector<BehaviorBaseNode*>* lists);


	//�f�o�b�O�p
	void DrawNodeInfo(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, 
		std::vector<BehaviorJudgeBase*> judgeObjects, bool checkIsDisplay = true);

	void DrawNodeInfo_Child(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, std::vector<BehaviorJudgeBase*> judgeObjects);

	void DrawNodeInfo_withEditor(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, std::vector<BehaviorJudgeBase*> judgeObjects);

public:
	//�m�[�h�̖��O
	std::string		nodeName;
	//�m�[�h�̎��
	NODE_TYPE		type;
	//�m�[�h�I�����[��
	SELECT_RULE		rule;
	//�e�m�[�h
	BehaviorBaseNode*				parent		= nullptr;
	//�q�m�[�h�z��
	std::vector<BehaviorBaseNode*>	childs;
	//����N���X
	BehaviorJudgeBase*				judgeObject = nullptr;
	//���s�N���X
	BehaviorActionBase*				actObject	= nullptr;

	//�m�[�h�D��x
	unsigned int	priority;

private:
	//�G�f�B�^�[��p
	bool isDisplay = false;
	bool isChangeChilds = false;
	bool isChangeScripts = false;


	friend class BehaviorEditor;
};


enum class NODE_STATUS
{
	RUNNING,	//���s��
	SUCCESS,	//����
	FAILED,		//���s
	STOP,
};

//������
//�m�[�h�̎�ނ�h���N���X�Œ�`��������
//����͊��N���X�ƂȂ�
class BehaviorNodeBase
{
public:
	BehaviorNodeBase(){}
	virtual ~BehaviorNodeBase(){}

	virtual void Init() = 0;

	virtual bool Judge() = 0;

	virtual NODE_STATUS Run() = 0;

};

