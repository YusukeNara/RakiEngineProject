#include "BehaviorTree.h"

#include <iostream>

void AI_BehaviorTree::Init(std::shared_ptr<BehaviorBaseNode> firstNode)
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
	//�擪�m�[�h�ݒ�
	rootNode = firstNode;

	//���[�g�m�[�h�ɍs�������蓖�Ă��Ă��āA���ꂪ���s���̏ꍇ�̓��[�g�m�[�h�͂��̂܂�
	if (rootNode->actObject) {
		if (rootNode->actObject->actionState == BehaviorActionBase::ACTION_STATE::RUNNING) {
			return;
		}
	}

	std::weak_ptr<BehaviorBaseNode> rootNodeResult = rootNode;
	//�m�[�h���[�܂ňړ�
	//�q�m�[�h���Ȃ� = �m�[�h�̏I�[
	while (rootNodeResult.lock()->childs.empty())
	{
		rootNodeResult = rootNode->Inference();
	}

	//�s���I�u�W�F�N�g������
	if(rootNodeResult.lock()->actObject) { rootNodeResult.lock()->actObject->Init(); }
	rootNode = rootNodeResult.lock();
}

void AI_BehaviorTree::Run()
{
	//���t���[���Ăяo�����Ƃŏ���ɂ���Ă����悤�ɂ���

	std::shared_ptr<BehaviorActionBase> aptr = rootNode->actObject;

	//���[�g�m�[�h�̎��s�I�u�W�F�N�g����̂Ƃ��͂����Ƀm�[�h�̐��_������
	if (aptr) { 
		Inference(); 
	}

	//���[�g�m�[�h���s
	BehaviorActionBase::ACTION_STATE state = rootNode->actObject->Run();
	rootNode->actObject->actionState = state;

	//�s���I����
	if (state != BehaviorActionBase::ACTION_STATE::RUNNING) {
		//���_�J�n
		Inference();
	}
}
