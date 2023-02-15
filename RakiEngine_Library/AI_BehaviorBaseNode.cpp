
#include "AI_BehaviorBaseNode.h"
#include "Behavior_JudgeBase.h"
#include "NY_random.h"

#include <d3d12.h>
#include "Raki_imguiMgr.h"

void BehaviorBaseNode::CreateJudgeNode(std::string nodeName, SELECT_RULE rule, BehaviorJudgeBase* judgeObject)
{
    ////�m�[�h�̖��O
    //this->nodeName = nodeName;
    ////���[��
    //this->rule = rule;
    ////����I�u�W�F�N�g
    //this->judgeObject.reset(judgeObject);
    ////�m�[�h�̎��
    //type = NODE_TYPE::TYPE_SELECTER;
}

void BehaviorBaseNode::AddjudgeNodeChild(std::shared_ptr<BehaviorBaseNode> child)
{
    //�q�m�[�h�Ɏ��g��e�Ƃ��ēo�^
    //child->parent = ;
    //�q�m�[�h�ǉ�
    childs.push_back(std::weak_ptr<BehaviorBaseNode>(child));
}

//void BehaviorBaseNode::CreateActionNode(std::string nodeName, BehaviorActionBase* actObject,BehaviorJudgeBase *judgeObject)
//{
//    if (actObject == nullptr) { return; }
//
//    this->nodeName = nodeName;
//
//    this->actObject.reset(actObject);
//
//    this->judgeObject.reset(judgeObject);
//
//    type = NODE_TYPE::TYPE_EXECUTE;
//}

void BehaviorBaseNode::CreateActionNode(std::string nodeName, std::shared_ptr<BehaviorActionBase> actObject, std::shared_ptr<BehaviorJudgeBase> judgeObject)
{
    if (actObject == nullptr) { return; }

    this->nodeName = nodeName;

    this->actObject = actObject;

    this->judgeObject = judgeObject;

    type = NODE_TYPE::TYPE_EXECUTE;
}

std::weak_ptr<BehaviorBaseNode> BehaviorBaseNode::Inference()
{
    //�q�m�[�h���X�g
    std::vector<std::weak_ptr<BehaviorBaseNode>> list;
    //�z��T�C�Y�m��
    list.reserve(childs.size());
    //�ԋp�p�ϐ� 
    std::weak_ptr<BehaviorBaseNode> result;

    //�q�m�[�h���I���\�����m�F
    for (auto &n : childs)
    {
        //����X�N���v�g��null�̏ꍇ�͖������Ŋi�[
        if (n.lock()) {
            list.push_back(n);
        }
        //�q�m�[�h�̔���N���X��true�̂Ƃ�
        else if (n.lock()->judgeObject.lock()->Judge()) {
            //��⃊�X�g�Ɋi�[
            list.push_back(n);
        }
    }

    if (list.size() == size_t(0) ) {
        //������Ă͂܂�Ȃ��ꍇ�A���ׂẴm�[�h������
        //�������Ӑ}���Ȃ��ꍇ������̂ŁA�I�v�V�����Őݒ�\�ɂ���
        for (auto& n : childs)
        {
            //��⃊�X�g�Ɋi�[
            list.push_back(n);
        }
    }

    //��⃊�X�g�̒�����I�����������ƂɕԂ��m�[�h������
    switch (rule)
    {
    case BehaviorBaseNode::RULE_RANDOM:
        //�����_���Ƀm�[�h������
        result = Select_Random(list);
        return result;
        break;

    case BehaviorBaseNode::RULE_PRIORITY:
        //�D��x�Ńm�[�h����
        result = Select_Priority(list);
        return result;
        break;

    case BehaviorBaseNode::RULE_ONOFF:
        //�����_���Ƀm�[�h������
        result = Select_Random(list);
        return result;
        break;

    case BehaviorBaseNode::RULE_SEAQUENCE:
        //�����_���Ƀm�[�h������
        result = Select_Random(list);
        return result;
        break;
    default:
        //�����_���Ƀm�[�h������
        result = Select_Random(list);
        return result;
        break;
    }

    //�����_���Ƀm�[�h������
    result = Select_Random(list);
    return result;
}

BehaviorActionBase::ACTION_STATE BehaviorBaseNode::Run()
{
    //�s�����s
    return actObject.lock()->Run();
}

std::weak_ptr<BehaviorBaseNode>BehaviorBaseNode::Select_Random(std::vector<std::weak_ptr<BehaviorBaseNode>> lists)
{
    int selectNumber = NY_random::intrand_sl(static_cast<int>(lists.size() - 1), 0);

    std::shared_ptr<BehaviorBaseNode> ptr = std::make_shared<BehaviorBaseNode>(lists[0]);

    return lists[selectNumber];
}

std::weak_ptr<BehaviorBaseNode> BehaviorBaseNode::Select_Priority(std::vector<std::weak_ptr<BehaviorBaseNode>> lists)
{
    std::weak_ptr<BehaviorBaseNode> result;

    unsigned int priority = -100;

    for (auto itr = lists.begin(); itr != lists.end(); itr++) {
        if (priority < itr->lock()->priority) {
            result = *itr;
            priority = itr->lock()->priority;
        }
    }

    return result;
}

void BehaviorBaseNode::DrawNodeInfo(std::vector<std::weak_ptr<BehaviorBaseNode>> editNodes,
    std::vector<std::weak_ptr<BehaviorActionBase>> actObjects,
    std::vector<std::weak_ptr<BehaviorJudgeBase>> judgeObjects,
    bool checkIsDisplay)
{
    //�f�o�b�O�p�A�ȈՓI�����ҏW���\��

    //�\���t���O�������ĂȂ�
    if (checkIsDisplay && !isDisplay) { return; }
    

    //�ҏW�p�ϐ�
    int nowSelectRule = static_cast<int>(rule);


    ImguiMgr::Get()->StartDrawImgui(nodeName.c_str(), 100, 300);

    if (ImGui::Button("Close Node Info")) { isDisplay = false; }

    switch (type)
    {
    case BehaviorBaseNode::TYPE_EXECUTE:
        ImGui::Text("Node Type : EXECUTE\n");

        ImGui::Text("Act script name : %s", actObject.lock()->actScriptName.c_str());

        ImGui::Text("Judge script Name : %s", judgeObject.lock()->judgeScriptName.c_str());

        ImGui::Text("Node Status\n");

        if (actObject.lock()) {
            switch (actObject.lock()->actionState)
            {
            case BehaviorActionBase::ACTION_STATE::STANDBY:
                ImGui::Text("Process is standby.");
                break;

            case BehaviorActionBase::ACTION_STATE::RUNNING:
                ImGui::Text("Process is running.");
                break;

            case BehaviorActionBase::ACTION_STATE::SUCCESS:
                ImGui::Text("Process is successed.");
                break;

            case BehaviorActionBase::ACTION_STATE::FAILED:
                ImGui::Text("Process is failed.");
                break;

            default:
                break;
            }
        }


        break;
    case BehaviorBaseNode::TYPE_SELECTER:
        ImGui::Text("Node Type : SELECTER");

        ImGui::Text("Select rules");

        ImGui::RadioButton("Random", &nowSelectRule, static_cast<int>(RULE_RANDOM));
        ImGui::SameLine();
        ImGui::RadioButton("Priority", &nowSelectRule, static_cast<int>(RULE_PRIORITY));

        break;
    default:
        break;
    }


    ImGui::Checkbox("Edit Scripts Pointer", &isChangeScripts);
    if (isChangeScripts) {
        std::weak_ptr<BehaviorActionBase> acttemp;
        ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(150, 250), ImGuiWindowFlags_NoTitleBar);
        for (auto& acts : actObjects) {
            if (ImGui::Button(acts.lock()->actScriptName.c_str())) {
                acttemp = acts;
            }
        }
        ImGui::EndChild();

        if (acttemp.lock() != nullptr) { this->actObject = acttemp; }

        std::weak_ptr<BehaviorJudgeBase> judgetemp;
        ImGui::BeginChild(ImGui::GetID((void*)1), ImVec2(150, 250), ImGuiWindowFlags_NoTitleBar);
        for (auto& judges : judgeObjects) {
            if (ImGui::Button(judges.lock()->judgeScriptName.c_str())) {
                judgetemp = judges;
            }
        }
        ImGui::EndChild();

        if (judgetemp.lock()) { this->judgeObject = judgetemp; }
    }

    ImguiMgr::Get()->EndDrawImgui();

    //�ݒ�K�p
    rule = static_cast<SELECT_RULE>(nowSelectRule);
}

void BehaviorBaseNode::DrawNodeInfo_Child(std::vector<std::weak_ptr<BehaviorBaseNode>> editNodes,
    std::vector<std::weak_ptr<BehaviorActionBase>> actObjects,
    std::vector<std::weak_ptr<BehaviorJudgeBase>> judgeObjects)
{
    if (this->isDisplay) {
        for (auto& child : childs) {
            ImGui::BeginChild(ImGui::GetID((void*)2), ImVec2(150, 250), ImGuiWindowFlags_NoTitleBar);
            child.lock()->DrawNodeInfo_withEditor(editNodes, actObjects, judgeObjects);
            ImGui::EndChild();
        }
    }
}

void BehaviorBaseNode::DrawNodeInfo_withEditor(std::vector<std::weak_ptr<BehaviorBaseNode>> editNodes,
    std::vector<std::weak_ptr<BehaviorActionBase>> actObjects,
    std::vector<std::weak_ptr<BehaviorJudgeBase>> judgeObjects)
{
    //�ҏW�p�ϐ�
    int nowSelectRule = static_cast<int>(rule);

    switch (type)
    {
    case BehaviorBaseNode::TYPE_EXECUTE:
        ImGui::Text("Node Type : EXECUTE\n");

        ImGui::Text("Node Status\n");

        if (actObject.lock()) {
            switch (actObject.lock()->actionState)
            {
            case BehaviorActionBase::ACTION_STATE::STANDBY:
                ImGui::Text("Process is standby.");
                break;

            case BehaviorActionBase::ACTION_STATE::RUNNING:
                ImGui::Text("Process is running.");
                break;

            case BehaviorActionBase::ACTION_STATE::SUCCESS:
                ImGui::Text("Process is successed.");
                break;

            case BehaviorActionBase::ACTION_STATE::FAILED:
                ImGui::Text("Process is failed.");
                break;

            default:
                break;
            }
        }

        break;
    case BehaviorBaseNode::TYPE_SELECTER:
        ImGui::Text("Node Type : SELECTER");

        ImGui::Text("Select rules");

        ImGui::RadioButton("Random", &nowSelectRule, static_cast<int>(RULE_RANDOM));
        ImGui::SameLine();
        ImGui::RadioButton("Priority", &nowSelectRule, static_cast<int>(RULE_PRIORITY));
        ImGui::SameLine();

        break;
    default:
        break;
    }
}
