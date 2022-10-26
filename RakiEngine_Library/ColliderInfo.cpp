#include "ColliderInfo.h"

ColliderInfo::ColliderInfo(Object3d* obj, BaseCollider* colider, RVector3 inter, std::string tag)
{
	this->object = obj;
	this->colider = colider;
	this->inter = inter;
	this->tag = tag;
}
