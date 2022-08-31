#pragma once

#include "NY_Object3DMgr.h"
#include "RVector.h"

enum COLIDER_TYPE
{
	COLIDER_TYPE_UNKNOWN,
	COLIDER_TYPE_AABB,
	COLIDER_TYPE_SPHERE,
	COLIDER_TYPE_POLYGON,
	COLIDER_TYPE_PLANE



};

//コライダー基底クラス
class BaseColider
{
public:
	BaseColider()			= default;
	virtual ~BaseColider()	= default;

	inline void SetObject(Object3d* object) {
		this->object = object;
	}

	inline Object3d *GetObject3d() { return object; }

	virtual void Update();


protected:
	// コライダー登録オブジェクト
	Object3d* object;

	// コライダーの種類
	COLIDER_TYPE type = COLIDER_TYPE_UNKNOWN;





};