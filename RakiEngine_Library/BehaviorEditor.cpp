#include "BehaviorEditor.h"

#include <d3d12.h>

BehaviorEditor::BehaviorEditor()
{
}

BehaviorEditor::~BehaviorEditor()
{
}

void BehaviorEditor::Init(AI_BehaviorTree* treePointer)
{
	this->treePointer = treePointer;
	nodes.push_back(treePointer->firstNode);
}

void BehaviorEditor::AddEditData_ActScript(std::shared_ptr<BehaviorActionBase> actObject)
{
	//�R���e�i�ɒǉ�
	actScripts.push_back(actObject);
}

void BehaviorEditor::AddEditData_JudgeScript(std::shared_ptr<BehaviorJudgeBase> judgeObject)
{
	//�R���e�i�ɒǉ�
	judgeScripts.push_back(judgeObject);
}

void BehaviorEditor::AddEditData_Node(std::shared_ptr<BehaviorBaseNode> node)
{
	//�m�[�h�ǉ�
	nodes.push_back(node);

	//�m�[�h�̃I�u�W�F�N�g���i�[
	if (node->actObject) {
		AddEditData_ActScript(node->actObject);
	}

	if (node->judgeObject) {
		AddEditData_JudgeScript(node->judgeObject);
	}
}

std::shared_ptr<BehaviorBaseNode> BehaviorEditor::CreateNewNode(std::string nodeName)
{
	std::shared_ptr<BehaviorBaseNode> result = std::make_shared<BehaviorBaseNode>();
	result->nodeName = nodeName;

	nodes.push_back(result);

	return result;
}

void BehaviorEditor::ObjectDataDraw()
{
	//�f�[�^�����X�g�\�����A�I�����ꂽ���̂̏ڍׂ�\������
	ImguiMgr::Get()->StartDrawImgui("Behavior Objects", 200, 500);

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);
	for (auto& nodelist : nodes) {
		if (ImGui::Button(nodelist.lock()->nodeName.c_str())) {
			nodelist.lock()->isDisplay = true;
		}
	}
	ImGui::EndChild();

	ImguiMgr::Get()->EndDrawImgui();
}

void BehaviorEditor::EditorDraw()
{
	//�f�[�^�����X�g�\�����A�I�����ꂽ���̂̏ڍׂ�\������
	ImguiMgr::Get()->StartDrawImgui("BehaviorEditorData", 250, 500);

	ImGui::Text("Behavior Tree Data");

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);

	treePointer->firstNode->DrawNodeInfo_withEditor(nodes, actScripts, judgeScripts);

	ImGui::EndChild();

	ImGui::Text("Root Node : %s", treePointer->rootNode->nodeName.c_str());

	//�G�f�B�^�[�E�B���h�E�̕`��I��
	ImguiMgr::Get()->EndDrawImgui();
}

void BehaviorEditor::NodeDataDraw()
{
	//�\���t���O�������Ă�m�[�h��\���A�ҏW
	for (auto& list : nodes) {
		if (list.lock()->isDisplay) {
			//�m�[�h
			list.lock()->DrawNodeInfo(nodes, actScripts, judgeScripts);
			//�q�m�[�h
			list.lock()->DrawNodeInfo_Child(nodes, actScripts, judgeScripts);
		}
	}
}
