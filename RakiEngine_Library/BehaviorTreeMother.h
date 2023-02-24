#pragma once
#include "BehaviorNodeBase.h"

class BehaviorTreeMother
{
public:

	void TreeInit(std::shared_ptr<bTreesys::BehaviorNodeBase> rootNode);

	void Run();



private:

	//ビヘイビア先頭ノード
	std::weak_ptr<bTreesys::BehaviorNodeBase> rootNode;

	//判定ノード保管用



};

