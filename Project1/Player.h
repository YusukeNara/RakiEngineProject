#pragma once
#include <NY_Object3DMgr.h>
#include <Sprite.h>
#include <NY_Camera.h>
#include <vector>
#include <array>
#include <memory>

#include "Bullet.h"

class Player
{
public:
	//プレイヤーの表示オブジェクト
	Object3d* object;

	//UIリソース
	Sprite s_hpFont;
	Sprite s_hpBar;
	Sprite s_CtrlImage;
	Sprite s_sight;

	//弾関係
	Model3D bulletModel;
	//std::vector<Bullet> bullets;
	std::array<Bullet, 25> bullets;

	//各種パラメータ
	RVector3 pos;	//座標
	RVector3 rot;		//回転
	RVector3 angleVec;	//回転速度
	RVector3 speedVec;		//速度ベクトル
	RVector3 speedScale;	//速度スケーリング
	RVector3 playerLookVec;	//向きベクトル
	RVector3 bVec;			//弾方向ベクトル
	RVector3 lVec;			//向きベクトル
	RV3Colider::Ray ray;

	RV3Colider::Rv3AABB boxAABB;

	//当たり判定
	RV3Colider::Sphere bodyColider;

	int hitpoint = 10;

	Player();
	~Player();

	void CameraMove();

	void PlayerMove();

	void Shot();

	void BulletManagement();
	//弾を消去
	
	
public:
	//初期化
	void Init();
	void Reset();
	//更新
	void Update();
	//描画
	void Draw();
	void UiDraw();
	void DebugDraw();
	//終了
	void Finalize();

};

