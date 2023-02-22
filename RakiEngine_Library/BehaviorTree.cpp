#include "BehaviorTree.h"

#include <iostream>

void AI_BehaviorTree::Init(std::shared_ptr<BehaviorBaseNode> firstNode)
{
	if (firstNode == nullptr) {
		return;
	}
	this->firstNode = firstNode;

	//最初の推論を実行
	Inference();
}

void AI_BehaviorTree::Inference()
{
	//先頭ノード設定
	rootNode = firstNode;

	//ルートノードに行動が割り当てられていて、それが実行中の場合はルートノードはそのまま
	if (rootNode->actObject) {
		if (rootNode->actObject->actionState == BehaviorActionBase::ACTION_STATE::RUNNING) {
			return;
		}
	}

	std::weak_ptr<BehaviorBaseNode> rootNodeResult = rootNode;
	//ノード末端まで移動
	//子ノードがない = ノードの終端
	while (rootNodeResult.lock()->childs.empty())
	{
		rootNodeResult = rootNode->Inference();
	}

	//行動オブジェクト初期化
	if(rootNodeResult.lock()->actObject) { rootNodeResult.lock()->actObject->Init(); }
	rootNode = rootNodeResult.lock();
}

void AI_BehaviorTree::Run()
{
	//毎フレーム呼び出すことで勝手にやってくれるようにする

	std::shared_ptr<BehaviorActionBase> aptr = rootNode->actObject;

	//ルートノードの実行オブジェクトが空のときはすぐにノードの推論をする
	if (aptr) { 
		Inference(); 
	}

	//ルートノード実行
	BehaviorActionBase::ACTION_STATE state = rootNode->actObject->Run();
	rootNode->actObject->actionState = state;

	//行動終了時
	if (state != BehaviorActionBase::ACTION_STATE::RUNNING) {
		//推論開始
		Inference();
	}
}
