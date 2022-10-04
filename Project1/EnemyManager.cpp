#include "EnemyManager.h"
#include <NY_random.h>

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{
	delete swordEnemyMother;
}

void EnemyManager::Init(Player *player)
{
	//複製元のエネミーオブジェクトを生成
	this->player = player;
	swordEnemyMother = new SwordEnemy(player);
}

void EnemyManager::Reset()
{
	swordEnemys.clear();
	swordEnemys.shrink_to_fit();
}

void EnemyManager::Update()
{
	if (Input::isKeyTrigger(DIK_F1)) {
		if (isDebugMode) { isDebugMode = false; }
		else { isDebugMode = true; }
	}

	//デバッグ処理のみ実行
	if (isDebugMode) {
		DebugExecution();
		return;
	}

	//すべての敵を更新
	for (auto& se : swordEnemys) {
		se->Update();
	}
	Colision();
	//敵の数が足りないとき補充
	EnemySpawn();
}

void EnemyManager::Draw()
{
	//すべての敵を描画
	for (auto& se : swordEnemys) {
		se->Draw();
	}

	if (isDebugMode) { swordEnemyMother->Draw(); }
}

void EnemyManager::DebugExecution()
{
	//デバッグ用の敵を動かす
	swordEnemyMother->Update();

	//プレイヤーの体力を固定
	player->hitpoint = 10;
}

void EnemyManager::DebugDraw()
{
	if (!isDebugMode) { return; }

	swordEnemyMother->DebugDraw();
}

void EnemyManager::EnemySpawn()
{
	//敵がいないとき、ランダムな位置に一定の数出現させる
	if (swordEnemys.size() <= 0) {
		for (int i = 0; i < 3; i++) {
			swordEnemys.push_back(swordEnemyMother->clone(player));
			swordEnemys[i]->s_object.pos = RVector3(NY_random::floatrand_sl(300, -300), 0, NY_random::floatrand_sl(300, 150));
		}
	}

}

void EnemyManager::Colision()
{

	for (int i = 0; i < 25;i++) {
		//死んでる弾は判定しない
		if (!player->bullets[i].isAlive) { continue; }

		for (auto se = swordEnemys.begin(); se != swordEnemys.end();) {
			if (RV3Colider::Colision2Sphere((*se)->s_object.bodyColision, player->bullets[i].coliSphere)) {
				//弾を消滅させ、エネミーにダメージ処理(消去)
				player->bullets[i].isAlive = false;
				se = swordEnemys.erase(se);
				//このループで判定する弾は消えたので抜ける
				break;
			}
			else {
				//次の敵を判定
				se++;
			}
		}
	}

}
