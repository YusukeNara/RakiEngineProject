#pragma once
#include <string>

#include "RVector.h"

class BaseCollider;
class Object3d;
class GameObject;

class ColliderInfo
{
public:
	ColliderInfo(RVector3 inter,std::string tag);

	//�Փ˓_
	RVector3 inter;
	//�^�O��
	std::string tag;

};

