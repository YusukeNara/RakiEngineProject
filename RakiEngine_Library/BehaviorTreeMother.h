#pragma once
#include "BehaviorNodeBase.h"

class BehaviorTreeMother
{
public:

	void TreeInit(std::shared_ptr<bTreesys::BehaviorNodeBase> rootNode);

	void Run();



private:

	//�r�w�C�r�A�擪�m�[�h
	std::weak_ptr<bTreesys::BehaviorNodeBase> rootNode;

	//����m�[�h�ۊǗp



};

