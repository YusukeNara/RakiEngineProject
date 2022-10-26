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

	killCount = 0;
	waveCount = 0;
	waveKillCount = 1;

	NumSprite.CreateAndSetDivisionUVOffsets(10, 5, 2, 64, 64, TexManager::LoadTexture("Resources/zenNum.png"));
	WaveSprite.Create(TexManager::LoadTexture("Resources/waveFont.png"));

	easeFrame = 0;
	isWaveMoving = false;

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
	////敵の数が足りないとき補充
	EnemySpawn();

	if (waveKillCount <= killCount) {
		waveCount++;
		waveKillCount = waveCount;
		killCount = 0;
	}

	if (isWaveMoving) {
		easeFrame++;
		if (easeFrame < 45) {
			wfcenter = Rv3Ease::OutQuad(WF_S, WF_CENTER, float(easeFrame) / 45.0f);
		}
		else if(easeFrame >= 60 && easeFrame < 105) {
			wfcenter = Rv3Ease::InQuad(WF_CENTER, WF_E, (float(easeFrame) - 60.0f) / 45.0f);
		}
		else if(easeFrame > 105) {
			easeFrame = 0;
			isWaveMoving = false;
		}
	}
	
}

void EnemyManager::Draw()
{
	//すべての敵を描画
	for (auto& se : swordEnemys) {
		se->Draw();
	}

	if (isDebugMode) { swordEnemyMother->Draw(); }
}

void EnemyManager::UIDraw()
{
	WaveSprite.DrawSprite(wfcenter.x - 128, wfcenter.y - 128);
	WaveSprite.Draw();

	if (waveCount >= 10) {
		NumSprite.uvOffsetHandle = waveCount - 10;
	}
	else {
		NumSprite.uvOffsetHandle = waveCount;
	}

	NumSprite.DrawSprite(wfcenter.x - 128 + WF_X_OFFSET, wfcenter.y - 128);
	NumSprite.Draw();
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

	ImguiMgr::Get()->StartDrawImgui("Enemys Status", 100, 200);

	ImGui::Text("Kill : %d  Alive : %d", killCount, swordEnemys.size());

	ImguiMgr::Get()->EndDrawImgui();
}

void EnemyManager::EnemySpawn()
{
	//敵がいないとき、ランダムな位置に一定の数出現させる
	if (swordEnemys.size() <= 0) {
		for (int i = 0; i < waveKillCount; i++) {
			swordEnemys.push_back(swordEnemyMother->clone(player));
			swordEnemys[i]->s_object.pos = RVector3(NY_random::floatrand_sl(300, -300), 0, NY_random::floatrand_sl(300, 150));
		}
		//ウェーブ進行
		isWaveMoving = true;
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
				
				killCount++;
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
