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
	//ルートノードに行動が割り当てられていて、それが実行中の場合はルートノードはそのまま
	if (rootNode->actObject != nullptr) {
		if (rootNode->actObject->actionState == BehaviorActionBase::ACTION_STATE::RUNNING) {
			return;
		}
	}

	//再帰処理によってノード末端まで移動

	//ルートノード設定
	rootNode = firstNode;
	while (rootNode->type == BehaviorBaseNode::NODE_TYPE::TYPE_SELECTER)
	{
		//ルートノードが末端に到達するまでループ
		//現状は行動ノード到達まで
		rootNode = rootNode->Inference();
	}
}

void AI_BehaviorTree::Run()
{
	//毎フレーム呼び出すことで勝手にやってくれるようにする

	//ルートノード実行
	BehaviorActionBase::ACTION_STATE state = rootNode->actObject->Run();

	//行動終了時
	if (state != BehaviorActionBase::ACTION_STATE::RUNNING) {
		//推論開始
		Inference();
	}
}
