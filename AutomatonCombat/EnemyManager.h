#pragma once

#include "SwordEnemy.h"
#include "GunEnemy.h"
#include "Player.h"
#include <Sprite.h>

#include <vector>

#include "EnemyDefeatEffect.h"

//敵の生成、更新、描画を担うクラス

enum class WAVE_STATUS
{
	WAVE_STANDBY,
	WAVE_RUN,
};

class EnemyManager
{
public:


	EnemyManager();
	~EnemyManager();

	void Init(Player *player,NavMeshAstar *astar);
	void Reset();

	void Update();

	

	//3d描画
	void Draw();
	//2d描画
	void UIDraw();
	void ParticleDraw();

	//デバッグモード
	void DebugExecution();

	void DebugDraw();

	//キル数カウント
	int killCount;
	//ウェーブ数カウント
	int waveCount = 0;
	//そのウェーブでキルしないといけないキル数
	int waveKillCount = 1;
	//グループ数
	int killedGroup = 0;
	int waveKillGroupAssignment = 0;

	//クリアフラグ
	bool gameCleared = false;

private:

	Player* player;

	//デバッグ用複製元
	SwordEnemy* swordEnemyMother;

	std::unique_ptr<GunEnemy> gunEnemy;

	//敵情報格納コンテナ
	std::vector<std::unique_ptr<SwordEnemy>> swordEnemys;


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

	//ウェーブ管理

	const int WAVE_GROUP_MAX_SPAWN = 8;
	const int WAVE_GROUP_MIN_SPAWN = 4;
	//グループ出現中心
	RVector3	groupSpawnPos;
	const float groupSpawnRad = 64.0f;

	ParticleManager* m_defeatPM;
	DefeatParticle* m_defeatP;

	int defeatPtex;

	NavMeshAstar* astar;
};

