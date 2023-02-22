#pragma once

#include "BehaviorTree.h"
#include "Behavior_ActionBase.h"
#include "Behavior_JudgeBase.h"
#include "Raki_imguiMgr.h"

#include <string>
#include <vector>

/// <summary>
/// ビヘイビアツリー、及びそれに使用するノードを編集するためのクラス
/// </summary>
class BehaviorEditor
{
public:
	BehaviorEditor();
	~BehaviorEditor();

	void Init(AI_BehaviorTree *treePointer);

	//編集するデータの追加、新規作成
	void								AddEditData_ActScript(std::shared_ptr<BehaviorActionBase> actObject);
	void								AddEditData_JudgeScript(std::shared_ptr<BehaviorJudgeBase> judgeObject);
	void								AddEditData_Node(std::shared_ptr<BehaviorBaseNode> node);
	std::shared_ptr<BehaviorBaseNode>	CreateNewNode(std::string nodeName);
	
	//

	//データ表示
	void ObjectDataDraw();

	//エディター描画（全データを表示する部分）
	void EditorDraw();

	//ノード情報描画（全ノードの中から、表示フラグが立っているもののみ描画）
	void NodeDataDraw();


private:

	//ツリーのポインタ
	AI_BehaviorTree* treePointer;

	//各種編集データコンテナ
	std::vector<std::weak_ptr<BehaviorBaseNode>>		nodes;
	std::vector<std::shared_ptr<BehaviorActionBase>>		actScripts;
	std::vector<std::shared_ptr<BehaviorJudgeBase>>		judgeScripts;

};

