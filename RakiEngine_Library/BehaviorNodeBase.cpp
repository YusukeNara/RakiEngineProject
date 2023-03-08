#include "BehaviorNodeBase.h"

using namespace bTreesys;

void bTreesys::BehaviorSequenceNode::Init()
{
	activeArrayNum = 0;

	//子ノード初期化
	for (auto& c : childs) {
		c->Init();
	}
}

NODE_STATUS bTreesys::BehaviorSequenceNode::Run()
{
	//子ノード末尾から順番に実行する
	NODE_STATUS result =  childs.at(activeArrayNum)->Run();

	if (result == STATE_SUCCESS) {
		activeArrayNum++;
	}
	else if (result == STATE_FAILED) {
		//子ノードが失敗を返したら即座に失敗を返す
		return STATE_FAILED;
	}
	
	if (activeArrayNum >= childs.size()) {
		//子ノード末尾まで実行したら、成功を返す
		return STATE_SUCCESS;
	}

	//実行中
	return STATE_RUN;
}

void bTreesys::BehaviorSelectorNode::Init()
{
	activeArrayNum = 0;

	//子ノード初期化
	for (auto& c : childs) {
		c->Init();
	}
}

NODE_STATUS bTreesys::BehaviorSelectorNode::Run()
{
	//子ノード末尾から順番に実行する
	NODE_STATUS result = childs.at(activeArrayNum)->Run();

	if (result == STATE_FAILED) {
		activeArrayNum++;
	}
	else if (result == STATE_SUCCESS) {
		//子ノードが成功を返したら即座に成功を返す
		return STATE_SUCCESS;
	}

	if (activeArrayNum >= childs.size()) {
		//子ノード末尾まで実行したら、失敗を返す
		return STATE_FAILED;
	}

	//実行中
	return STATE_RUN;
}

void bTreesys::BehaviorRepeatNode::Init()
{
	child->Init();
}

NODE_STATUS bTreesys::BehaviorRepeatNode::Run()
{
	auto result = child->Run();

	if (result != STATE_RUN) {
		Init();
	}

	return result;
}
