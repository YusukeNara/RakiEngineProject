
#include "AI_BehaviorBaseNode.h"
#include "Behavior_JudgeBase.h"
#include "NY_random.h"

#include <d3d12.h>
#include "Raki_imguiMgr.h"

void BehaviorBaseNode::CreateJudgeNode(std::string nodeName, SELECT_RULE rule, BehaviorJudgeBase* judgeObject)
{
    //ノードの名前
    this->nodeName = nodeName;
    //ルール
    this->rule = rule;
    //判定オブジェクト
    this->judgeObject = judgeObject;
    //ノードの種類
    type = NODE_TYPE::TYPE_SELECTER;
}

void BehaviorBaseNode::AddjudgeNodeChild(BehaviorBaseNode* child)
{
    //子ノード追加
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
    //子ノードリスト
    std::vector<BehaviorBaseNode*> list;
    //返却用変数 
    BehaviorBaseNode* result = nullptr;

    //子ノードが選択可能かを確認
    for (auto &n : childs)
    {
        //子ノードの判定クラスがtrueのとき
        if (n->judgeObject->Judge()){
            //候補リストに格納
            list.push_back(n);
        }
    }

    if (list.size() == size_t(0) ) {
        //一つも当てはまらない場合、すべてのノードを入れる
        for (auto& n : childs)
        {
            //候補リストに格納
            list.push_back(n);
        }
    }

    //候補リストの中から選択条件をもとに返すノードを決定
    switch (rule)
    {
    case BehaviorBaseNode::RULE_RANDOM:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_PRIORITY:
        //優先度でノード決定
        result = Select_Priority(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_ONOFF:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_SEAQUENCE:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;
    default:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;
    }

    //ランダムにノードを決定
    result = Select_Random(&list);
    return result;
}

BehaviorActionBase::ACTION_STATE BehaviorBaseNode::Run()
{
    //行動実行
    return actObject->Run();
}

BehaviorBaseNode *BehaviorBaseNode::Select_Random(std::vector<BehaviorBaseNode*> *lists)
{
    int selectNumber = NY_random::intrand_sl(static_cast<int>(lists->size()), 0);

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
    //デバッグ用、簡易的だが編集も可能に
    

    //編集用変数
    int nowSelectRule = static_cast<int>(rule);

    ImguiMgr::Get()->StartDrawImgui(nodeName.c_str(), 100, 300);

    switch (type)
    {
    case BehaviorBaseNode::TYPE_EXECUTE:
        ImGui::Text("Node Type : EXECUTE");

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

    //設定適用
    rule = static_cast<SELECT_RULE>(nowSelectRule);
}
