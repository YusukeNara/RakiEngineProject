#pragma once

#include "SwordEnemy.h"
#include "Player.h"
#include <Sprite.h>

#include <vector>

//敵の生成、更新、描画を担うクラス

class EnemyManager
{
public:


	EnemyManager();
	~EnemyManager();

	void Init(Player *player);
	void Reset();

	void Update();

	

	//3d描画
	void Draw();
	//2d描画
	void UIDraw();

	//デバッグモード
	void DebugExecution();

	void DebugDraw();

	//キル数カウント
	int killCount;
	//ウェーブ数カウント
	int waveCount = 0;
	//ウェーブごとのキルカウント
	int waveKillCount = 1;

private:

	Player* player;

	//デバッグ用複製元
	SwordEnemy* swordEnemyMother;

	//敵情報格納コンテナ
	std::vector<SwordEnemy*> swordEnemys;


	//デバッグフラグ
	bool isDebugMode = false;

	//敵出現処理
	void EnemySpawn();

	//プレイヤー弾と敵の衝突判定
	void Colision();
	//敵の攻撃の衝突判定はビヘイビア内部に実装（ゴリ押し）

	//ui表示
	Sprite WaveSprite;
	Sprite NumSprite;

	RVector3 wfcenter;
	const RVector3 WF_CENTER = RVector3(1280 / 3, 400, 0);
	const RVector3 WF_S = RVector3(1280 / 3, 900, 0);
	const RVector3 WF_E = RVector3(1280 / 3, -100, 0);
	const float WF_X_OFFSET = 256;

	int easeFrame;
	const int WF_E_MAXFRAME = 45;

	bool isWaveMoving = false;


};

