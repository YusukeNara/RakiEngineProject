#pragma once
#include <string>

#include "RVector.h"

class BaseCollider;
class Object3d;

class ColliderInfo
{
public:
	ColliderInfo(Object3d* obj, BaseCollider* colider, RVector3 inter,std::string tag);

	//�Փ˂����I�u�W�F�N�g
	Object3d* object;
	//�Փ˂����R���C�_�[
	BaseCollider* colider;
	//�Փ˓_
	RVector3 inter;
	//�^�O��
	std::string tag;

};

