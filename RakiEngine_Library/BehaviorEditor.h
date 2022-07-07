#pragma once

#include "BehaviorTree.h"
#include "Behavior_ActionBase.h"
#include "Behavior_JudgeBase.h"
#include "Raki_imguiMgr.h"

#include <string>
#include <vector>

/// <summary>
/// �r�w�C�r�A�c���[�A�y�т���Ɏg�p����m�[�h��ҏW���邽�߂̃N���X
/// </summary>
class BehaviorEditor
{
public:
	BehaviorEditor();
	~BehaviorEditor();

	void Init(AI_BehaviorTree *treePointer);

	//�ҏW����f�[�^�̒ǉ��A�V�K�쐬
	void				AddEditData_ActScript(BehaviorActionBase* actObject);
	void				AddEditData_JudgeScript(BehaviorJudgeBase* judgeObject);
	void				AddEditData_Node(BehaviorBaseNode* node);
	BehaviorBaseNode*	CreateNewNode(std::string nodeName);
	
	//

	//�f�[�^�\��
	void ObjectDataDraw();

	//�G�f�B�^�[�`��i�S�f�[�^��\�����镔���j
	void EditorDraw();

	//�m�[�h���`��i�S�m�[�h�̒�����A�\���t���O�������Ă�����̂̂ݕ`��j
	void NodeDataDraw();


private:

	//�c���[�̃|�C���^
	AI_BehaviorTree* treePointer;

	//�e��ҏW�f�[�^�R���e�i
	std::vector<BehaviorBaseNode*>		nodes;
	std::vector<BehaviorActionBase*>	actScripts;
	std::vector<BehaviorJudgeBase*>		judgeScripts;

};

