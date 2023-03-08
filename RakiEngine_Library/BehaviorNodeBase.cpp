#include "BehaviorNodeBase.h"

using namespace bTreesys;

void bTreesys::BehaviorSequenceNode::Init()
{
	activeArrayNum = 0;

	//�q�m�[�h������
	for (auto& c : childs) {
		c->Init();
	}
}

NODE_STATUS bTreesys::BehaviorSequenceNode::Run()
{
	//�q�m�[�h�������珇�ԂɎ��s����
	NODE_STATUS result =  childs.at(activeArrayNum)->Run();

	if (result == STATE_SUCCESS) {
		activeArrayNum++;
	}
	else if (result == STATE_FAILED) {
		//�q�m�[�h�����s��Ԃ����瑦���Ɏ��s��Ԃ�
		return STATE_FAILED;
	}
	
	if (activeArrayNum >= childs.size()) {
		//�q�m�[�h�����܂Ŏ��s������A������Ԃ�
		return STATE_SUCCESS;
	}

	//���s��
	return STATE_RUN;
}

void bTreesys::BehaviorSelectorNode::Init()
{
	activeArrayNum = 0;

	//�q�m�[�h������
	for (auto& c : childs) {
		c->Init();
	}
}

NODE_STATUS bTreesys::BehaviorSelectorNode::Run()
{
	//�q�m�[�h�������珇�ԂɎ��s����
	NODE_STATUS result = childs.at(activeArrayNum)->Run();

	if (result == STATE_FAILED) {
		activeArrayNum++;
	}
	else if (result == STATE_SUCCESS) {
		//�q�m�[�h��������Ԃ����瑦���ɐ�����Ԃ�
		return STATE_SUCCESS;
	}

	if (activeArrayNum >= childs.size()) {
		//�q�m�[�h�����܂Ŏ��s������A���s��Ԃ�
		return STATE_FAILED;
	}

	//���s��
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
