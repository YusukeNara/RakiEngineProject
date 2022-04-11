#pragma once
#include "NY_Object3D.h"

struct ParticleBase {
	Object3d *obj; //粒オブジェクト
	XMFLOAT3 vec;      //方向ベクトル
	int startFlame;  //開始フレーム
	int endFlame;    //終了フレーム
	bool  isAlive;     //描画フラグ
};
