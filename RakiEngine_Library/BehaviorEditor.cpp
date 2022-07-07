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
}

void BehaviorEditor::AddEditData_ActScript(BehaviorActionBase* actObject)
{
	//�R���e�i�ɒǉ�
	actScripts.push_back(actObject);
}

void BehaviorEditor::AddEditData_JudgeScript(BehaviorJudgeBase* judgeObject)
{
	//�R���e�i�ɒǉ�
	judgeScripts.push_back(judgeObject);
}

void BehaviorEditor::AddEditData_Node(BehaviorBaseNode* node)
{
	//�m�[�h�ǉ�
	nodes.push_back(node);
}

BehaviorBaseNode* BehaviorEditor::CreateNewNode(std::string nodeName)
{
	BehaviorBaseNode* result = new BehaviorBaseNode;
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
		if (ImGui::Button(nodelist->nodeName.c_str())) { 
			nodelist->isDisplay = true; 
		}
	}
	ImGui::EndChild();

	//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);
	//for (auto& judgelist : judgeScripts) {
	//	if(ImGui::Button(judgelist->judgeScriptName.c_str())){}
	//}
	//ImGui::EndChild();

	//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);
	//for (auto& actlist : actScripts) {
	//	ImGui::Button(actlist->actScriptName.c_str());
	//}
	//ImGui::EndChild();


	ImguiMgr::Get()->EndDrawImgui();
}

void BehaviorEditor::EditorDraw()
{
	//�f�[�^�����X�g�\�����A�I�����ꂽ���̂̏ڍׂ�\������
	ImguiMgr::Get()->StartDrawImgui("BehaviorEditorData", 250, 500);

	ImGui::Text("Behavior Tree Data");

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);

	treePointer->firstNode->DrawNodeInfo_withEditor();

	ImGui::EndChild();

	ImGui::Text("Root Node : %s", treePointer->rootNode->nodeName.c_str());

	//�G�f�B�^�[�E�B���h�E�̕`��I��
	ImguiMgr::Get()->EndDrawImgui();

	//�f�[�^�ꗗ



}

void BehaviorEditor::NodeDataDraw()
{
	//�\���t���O�������Ă�m�[�h��\��
	for (auto& list : nodes) {
		if (list->isDisplay) { 
			list->DrawNodeInfo(); 
		}
	}



}
