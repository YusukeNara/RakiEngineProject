#pragma once
#include <string>

#include "RVector.h"

class BaseCollider;
class Object3d;

class ColliderInfo
{
public:
	ColliderInfo(Object3d* obj, BaseCollider* colider, RVector3 inter,std::string tag);

	//衝突したオブジェクト
	Object3d* object;
	//衝突したコライダー
	BaseCollider* colider;
	//衝突点
	RVector3 inter;
	//タグ名
	std::string tag;

};

