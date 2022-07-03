#pragma once
#include "Behavior_ActionBase.h"


#include <string>
#include <vector>

class BehaviorJudgeBase;

//ビヘイビアのノード
//ここにactionBase,JudgeBaseのポインタを入れて実行する
//アプリケーション側ではこれを継承したクラスのポインタ型オブジェクトで実行する
class BehaviorBaseNode
{
public:
	//ノード決定ルール
	enum SELECT_RULE
	{
		RULE_RANDOM,	//ランダム
		RULE_PRIORITY,	//優先度
		RULE_ONOFF,		//オンオフ
		RULE_SEAQUENCE,	//シーケンス
	};

	//ノードの種類
	enum NODE_TYPE
	{
		TYPE_EXECUTE,	//実行ノード
		TYPE_SELECTER,	//選択ノード
	};

public:
	BehaviorBaseNode() {};
	~BehaviorBaseNode() {};

	/// <summary>
	/// 判定用ノードを生成
	/// </summary>
	/// <param name="nodeName">ノードの名前</param>
	/// <param name="rule">子ノード選択ルール</param>
	/// <param name="judgeObject">ノード選択条件を定義した派生クラスオブジェクト</param>
	void CreateJudgeNode(std::string nodeName, SELECT_RULE rule, BehaviorJudgeBase* judgeObject);

	/// <summary>
	/// 判定ノードに子ノード追加
	/// </summary>
	/// <param name="child"></param>
	void AddjudgeNodeChild(BehaviorBaseNode* child);

	/// <summary>
	/// 実行ノード生成
	/// </summary>
	/// <param name="nodeName">ノードの名前</param>
	/// <param name="actObject">実行する派生クラスオブジェクト</param>
	void CreateActionNode(std::string nodeName, BehaviorActionBase* actObject);

	/// <summary>
	/// ノード推論
	/// <para> 子ノードの判定クラスを実行し、候補となるノードを選択</para>
	/// <para> 候補ノードの中から選択ルールによって選ばれたノードを選択</para>
	/// <para> これをノードの末端まで繰り返す関数</para>
	/// </summary>
	/// <returns>推論の結果から出たルートノード</returns>
	BehaviorBaseNode *Inference();

	/// <summary>
	/// ノード実行
	/// <para>実行クラスの処理を行う</para>
	/// <para>実行クラスがない場合失敗扱いになる</para>
	/// </summary>
	/// <returns>実行クラスの進行状況</returns>
	BehaviorActionBase::ACTION_STATE Run();

	///以下はノード決定用関数
	BehaviorBaseNode *Select_Random(std::vector<BehaviorBaseNode*> *lists);

	BehaviorBaseNode* Select_Priority(std::vector<BehaviorBaseNode*>* lists);


	//デバッグ用
	void DrawNodeInfo();


public:
	//ノードの名前
	std::string		nodeName;
	//ノードの種類
	NODE_TYPE		type;
	//ノード選択ルール
	SELECT_RULE		rule;
	//親ノード（これはツリーで実行時に自動で代入される）
	BehaviorBaseNode*				parent		= nullptr;
	//子ノード配列
	std::vector<BehaviorBaseNode*>	childs;
	//判定クラス
	BehaviorJudgeBase*				judgeObject = nullptr;
	//実行クラス
	BehaviorActionBase*				actObject	= nullptr;

	//ノード優先度
	unsigned int	priority;
};