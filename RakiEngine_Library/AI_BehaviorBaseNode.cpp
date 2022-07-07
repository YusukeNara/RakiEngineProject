
#include "AI_BehaviorBaseNode.h"
#include "Behavior_JudgeBase.h"
#include "NY_random.h"

#include <d3d12.h>
#include "Raki_imguiMgr.h"

void BehaviorBaseNode::CreateJudgeNode(std::string nodeName, SELECT_RULE rule, BehaviorJudgeBase* judgeObject)
{
    //�m�[�h�̖��O
    this->nodeName = nodeName;
    //���[��
    this->rule = rule;
    //����I�u�W�F�N�g
    this->judgeObject = judgeObject;
    //�m�[�h�̎��
    type = NODE_TYPE::TYPE_SELECTER;
}

void BehaviorBaseNode::AddjudgeNodeChild(BehaviorBaseNode* child)
{
    //�q�m�[�h�Ɏ��g��e�Ƃ��ēo�^
    child->parent = this;
    //�q�m�[�h�ǉ�
    childs.push_back(child);
}

void BehaviorBaseNode::CreateActionNode(std::string nodeName, BehaviorActionBase* actObject,BehaviorJudgeBase *judgeObject)
{
    if (actObject == nullptr) { return; }

    this->nodeName = nodeName;

    this->actObject = actObject;

    this->judgeObject = judgeObject;

    type = NODE_TYPE::TYPE_EXECUTE;
}

BehaviorBaseNode* BehaviorBaseNode::Inference()
{
    //�q�m�[�h���X�g
    std::vector<BehaviorBaseNode*> list;
    //�z��T�C�Y�m��
    list.reserve(childs.size());
    //�ԋp�p�ϐ� 
    BehaviorBaseNode* result = nullptr;

    //�q�m�[�h���I���\�����m�F
    for (auto &n : childs)
    {
        //����X�N���v�g��null�̏ꍇ�͖������Ŋi�[
        if (n->judgeObject == nullptr) {
            list.push_back(n);
        }
        //�q�m�[�h�̔���N���X��true�̂Ƃ�
        else if (n->judgeObject->Judge()){
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
        result = Select_Random(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_PRIORITY:
        //�D��x�Ńm�[�h����
        result = Select_Priority(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_ONOFF:
        //�����_���Ƀm�[�h������
        result = Select_Random(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_SEAQUENCE:
        //�����_���Ƀm�[�h������
        result = Select_Random(&list);
        return result;
        break;
    default:
        //�����_���Ƀm�[�h������
        result = Select_Random(&list);
        return result;
        break;
    }

    //�����_���Ƀm�[�h������
    result = Select_Random(&list);
    return result;
}

BehaviorActionBase::ACTION_STATE BehaviorBaseNode::Run()
{
    //�s�����s
    return actObject->Run();
}

BehaviorBaseNode *BehaviorBaseNode::Select_Random(std::vector<BehaviorBaseNode*> *lists)
{
    int selectNumber = NY_random::intrand_sl(static_cast<int>(lists->size() - 1), 0);

    return (*lists)[selectNumber];
}

BehaviorBaseNode* BehaviorBaseNode::Select_Priority(std::vector<BehaviorBaseNode*>* lists)
{
    BehaviorBaseNode* result = nullptr;

    unsigned int priority = -100;

    for (auto itr = lists->begin(); itr != lists->end(); itr++) {
        if (priority < (*itr)->priority) {
            result = (*itr);
            priority = (*itr)->priority;
        }
    }

    return result;
}

void BehaviorBaseNode::DrawNodeInfo()
{
    //�f�o�b�O�p�A�ȈՓI�����ҏW���\��

    //�\���t���O�������ĂȂ�
    if (!isDisplay) { return; }
    

    //�ҏW�p�ϐ�
    int nowSelectRule = static_cast<int>(rule);
    std::string parentName;
    if (parent != nullptr) {
        parentName = parent->nodeName;
    }


    ImguiMgr::Get()->StartDrawImgui(nodeName.c_str(), 100, 300);

    if (ImGui::Button("Close Node Info")) { isDisplay = false; }

    switch (type)
    {
    case BehaviorBaseNode::TYPE_EXECUTE:
        ImGui::Text("Node Type : EXECUTE\n");

        ImGui::Text("Node Status\n");

        switch (actObject->actionState)
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

        ImGui::Text(parentName.c_str());


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

    ImguiMgr::Get()->EndDrawImgui();

    //�ݒ�K�p
    rule = static_cast<SELECT_RULE>(nowSelectRule);
}

void BehaviorBaseNode::DrawNodeInfo_withEditor()
{
    //�ҏW�p�ϐ�
    int nowSelectRule = static_cast<int>(rule);
    std::string parentName;
    if (parent != nullptr) {
        parentName = parent->nodeName;
    }

    switch (type)
    {
    case BehaviorBaseNode::TYPE_EXECUTE:
        ImGui::Text("Node Type : EXECUTE\n");

        ImGui::Text("Node Status\n");

        switch (actObject->actionState)
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

        ImGui::Text(parentName.c_str());

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
