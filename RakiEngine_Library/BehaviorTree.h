#pragma once
#include "AI_BehaviorBaseNode.h"

//�r�w�C�r�A�c���[�N���X
class AI_BehaviorTree
{
public:
	//���[�g�m�[�h�|�C���^
	std::shared_ptr<BehaviorBaseNode> rootNode;

public:
	//�������i�ŏ��Ɏ��s����m�[�h��I���j
	void Init(std::shared_ptr<BehaviorBaseNode> firstNode);

	//�m�[�h�̐��_
	void Inference();

	//���[�g�m�[�h�̎��s
	void Run();
	
	//�擪�m�[�h
	std::shared_ptr<BehaviorBaseNode> firstNode;

	//�c���[��
	std::string treeName;

};