#include "BehaviorTreeMother.h"

void BehaviorTreeMother::TreeInit(std::shared_ptr<bTreesys::BehaviorNodeBase> rootNode)
{
	this->rootNode = rootNode;

	this->rootNode.lock()->Init();
}

void BehaviorTreeMother::Run()
{
	auto node = rootNode.lock()->Run();

	if (node != bTreesys::STATE_RUN) {
		rootNode.lock()->Init();
	}
}
