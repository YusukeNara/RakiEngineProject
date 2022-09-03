#pragma once

#include "SwordEnemy.h"
#include "Player.h"

#include <vector>

//敵の生成、更新、描画を担うクラス

class EnemyManager
{
public:

	void Init();

	void Update();

	void Draw();

private:


	//敵情報格納コンテナ
	std::vector<SwordEnemy*> swordEnemys;

	//敵出現
	void EnemySpawn();

	//


};

