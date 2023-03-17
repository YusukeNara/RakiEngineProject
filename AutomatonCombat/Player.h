#pragma once
#include <NY_Object3DMgr.h>
#include <Sprite.h>
#include <NY_Camera.h>
#include <GameObject.h>
#include <vector>
#include <array>
#include <memory>
#include <ParticleManager.h>

#include "Bullet.h"

class Player : public GameObject
{
public:
	//UIリソース
	Sprite s_hpFont;
	Sprite s_hpBar;
	Sprite s_CtrlImage;
	Sprite s_sight;
	Sprite s_numFont;
	Sprite s_noammo;

	//弾関係
	std::shared_ptr<Model3D> bulletModel;
	//std::vector<Bullet> bullets;
	std::array<Bullet, 25> bullets;
	//残弾数と最大弾数
	const int MAX_BULLET = 25;
	int nowBullet = 25;

	//各種パラメータ
	RVector3 angleVec;		//回転速度
	RVector3 speedVec;		//速度ベクトル
	RVector3 speedScale;	//速度スケーリング
	RVector3 playerLookVec;	//向きベクトル
	RVector3 bVec;			//弾方向ベクトル
	RVector3 lVec;			//向きベクトル
	RV3Colider::Ray ray;

	RV3Colider::Rv3AABB boxAABB;

	//当たり判定
	RV3Colider::Sphere bodyColider;

	Sprite warningSprite;

	float damagedAlpha;

	//体力管理部
	int hitpoint = 100;
	const int HITPOINT_MAX = 100;
	int healCoolTime = 0;
	const int HEAL_COOLTIME = 180;
	float redEffectScale = 0.0f;

	RVector3 l_rot;

	//カメラ補完用
	struct CameraMoveParam
	{
		float camMovePower;
		const int camMovePowerMaxFrame = 15;
		int camMovePowerFrame;

		CameraMoveParam() = default;
		~CameraMoveParam() = default;
	};
	CameraMoveParam camMoveParam;


	Player();
	~Player();

	void CameraMove();

	void PlayerMove();

	//射撃
	void Shot();

	//再装填
	void Reload();

	void BulletManagement();
	//弾を消去
	
	void HealthManagement();
	
public:
	//初期化
	void Init()		override;
	void Reset();
	void Update()	override;
	void Draw()		override;
	void UiDraw();
	void ParticleDraw();
	void DebugDraw();
	//終了
	void Finalize() override;

	void Load() override;
	void OnCollision(const ColliderInfo* info) override;

	//ダメージ処理
	void PlayerDamaged(int damage);

};

