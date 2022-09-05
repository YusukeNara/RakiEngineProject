#pragma once

#include "SwordEnemy.h"
#include "Player.h"

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

	void Draw();

	//デバッグモード
	void DebugExecution();

	void DebugDraw();

private:

	Player* player;

	//デバッグ用複製元
	SwordEnemy* swordEnemyMother;

	//敵情報格納コンテナ
	std::vector<SwordEnemy*> swordEnemys;

	//キル数カウント
	int killCount;

	//デバッグフラグ
	bool isDebugMode = false;

	//敵出現処理
	void EnemySpawn();

	//プレイヤー弾と敵の衝突判定
	void Colision();
	//敵の攻撃の衝突判定はビヘイビア内部に実装（ゴリ押し）

};

