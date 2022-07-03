#include "BehaviorTree.h"

#include <iostream>

void AI_BehaviorTree::Init(BehaviorBaseNode* firstNode)
{
	if (firstNode == nullptr) {
		return;
	}
	this->firstNode = firstNode;

	//�ŏ��̐��_�����s
	Inference();
}

void AI_BehaviorTree::Inference()
{
	//���[�g�m�[�h�ɍs�������蓖�Ă��Ă��āA���ꂪ���s���̏ꍇ�̓��[�g�m�[�h�͂��̂܂�
	if (rootNode->actObject != nullptr) {
		if (rootNode->actObject->actionState == BehaviorActionBase::ACTION_STATE::RUNNING) {
			return;
		}
	}

	//�ċA�����ɂ���ăm�[�h���[�܂ňړ�

	//���[�g�m�[�h�ݒ�
	rootNode = firstNode;
	while (rootNode->type == BehaviorBaseNode::NODE_TYPE::TYPE_SELECTER)
	{
		//���[�g�m�[�h�����[�ɓ��B����܂Ń��[�v
		//����͍s���m�[�h���B�܂�
		rootNode = rootNode->Inference();
	}
}

void AI_BehaviorTree::Run()
{
	//���t���[���Ăяo�����Ƃŏ���ɂ���Ă����悤�ɂ���

	//���[�g�m�[�h���s
	BehaviorActionBase::ACTION_STATE state = rootNode->actObject->Run();

	//�s���I����
	if (state != BehaviorActionBase::ACTION_STATE::RUNNING) {
		//���_�J�n
		Inference();
	}
}
