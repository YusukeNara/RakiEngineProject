#pragma once
#include <NY_Object3DMgr.h>
#include <NY_Camera.h>

class Player
{
public:
	//プレイヤーの表示オブジェクト
	Object3d* object;

	//各種パラメータ
	RVector3 pos;	//座標

	RVector3 rot;		//回転
	RVector3 angleVec;	//回転速度

	RVector3 speedVec;		//速度ベクトル
	RVector3 speedScale;	//速度スケーリング

	RVector3 campos;

	Player();
	~Player();
	
public:
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();
	//終了
	void Finalize();


};

