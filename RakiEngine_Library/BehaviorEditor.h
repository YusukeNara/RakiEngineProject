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
	void								AddEditData_ActScript(std::shared_ptr<BehaviorActionBase> actObject);
	void								AddEditData_JudgeScript(std::shared_ptr<BehaviorJudgeBase> judgeObject);
	void								AddEditData_Node(std::shared_ptr<BehaviorBaseNode> node);
	std::shared_ptr<BehaviorBaseNode>	CreateNewNode(std::string nodeName);
	
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
	std::vector<std::weak_ptr<BehaviorBaseNode>>		nodes;
	std::vector<std::shared_ptr<BehaviorActionBase>>		actScripts;
	std::vector<std::shared_ptr<BehaviorJudgeBase>>		judgeScripts;

};

