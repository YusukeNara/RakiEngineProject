#include "BehaviorTree.h"

#include <iostream>

void AI_BehaviorTree::Init(BehaviorBaseNode* firstNode)
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
	//ルートノード設定
	rootNode = firstNode;

	//ルートノードに行動が割り当てられていて、それが実行中の場合はルートノードはそのまま
	if (rootNode->actObject != nullptr) {
		if (rootNode->actObject->actionState == BehaviorActionBase::ACTION_STATE::RUNNING) {
			return;
		}
	}

	//再帰処理によってノード末端まで移動
	while (rootNode->type == BehaviorBaseNode::NODE_TYPE::TYPE_SELECTER)
	{
		rootNode = rootNode->Inference();
	}

	rootNode->actObject->Init();
}

void AI_BehaviorTree::Run()
{
	//毎フレーム呼び出すことで勝手にやってくれるようにする

	//ルートノード実行
	BehaviorActionBase::ACTION_STATE state = rootNode->actObject->Run();
	rootNode->actObject->actionState = state;

	//行動終了時
	if (state != BehaviorActionBase::ACTION_STATE::RUNNING) {
		//推論開始
		Inference();
	}
}
