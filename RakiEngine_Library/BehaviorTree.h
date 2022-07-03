#pragma once
#include "AI_BehaviorBaseNode.h"

//�r�w�C�r�A�c���[�N���X
class AI_BehaviorTree
{
public:
	//���[�g�m�[�h�|�C���^
	BehaviorBaseNode* rootNode;

public:
	//�������i�ŏ��Ɏ��s����m�[�h��I���j
	void Init(BehaviorBaseNode* firstNode);

	//�m�[�h�̐��_
	void Inference();

	//���[�g�m�[�h�̎��s
	void Run();
	
	//�擪�m�[�h
	BehaviorBaseNode *firstNode;

};