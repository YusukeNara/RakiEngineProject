#include "EnemyManager.h"
#include <NY_random.h>
#include "WaveDirections.h"

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{
	delete swordEnemyMother;
}

void EnemyManager::Init(std::shared_ptr<Player> player, std::shared_ptr<NavMeshAstar> astar)
{
	//複製元のエネミーオブジェクトを生成
	this->player = player;
	swordEnemyMother = new SwordEnemy(player, astar);
	swordEnemyMother->obj->SetAffineParamScale(RVector3(5.f, 5.f, 5.f));

	//gunEnemy = std::make_unique<GunEnemy>();
	//gunEnemy->SetPlayer(player);
	//gunEnemy->Init();

	killCount = 0;
	waveCount = 1;
	waveKillCount = 1;

	NumSprite.CreateAndSetDivisionUVOffsets(10, 5, 2, 64, 64, TexManager::LoadTexture("Resources/zenNum.png"));
	WaveSprite.Create(TexManager::LoadTexture("Resources/waveFont.png"));

	easeFrame = 0;
	isWaveMoving = true;

	killedGroup = 0;
	waveKillGroupAssignment = 3;

	this->astar = astar;

	//最初の出現処理
	int spawnCount = NY_random::intrand_sl(WAVE_GROUP_MAX_SPAWN, WAVE_GROUP_MIN_SPAWN);
	groupSpawnPos = RVector3(NY_random::floatrand_sl(300, -300), 10, NY_random::floatrand_sl(300, 300));
	for (int i = 0; i < spawnCount; i++) {
		//出現座標のローカル座標を設定
		RVector3 spawnPos = groupSpawnPos - RVector3(NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad),
			0, NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad));

		swordEnemys.push_back(std::make_unique<SwordEnemy>(player, astar));
		swordEnemys[i]->pos = spawnPos;
	}

	m_defeatP = new DefeatParticle;
	m_defeatPM = ParticleManager::Create();
	m_defeatPM->Prototype_Set(m_defeatP);

	defeatPtex = TexManager::LoadTexture("Resources/effect1.png");

}

void EnemyManager::Reset()
{
	gameCleared = false;
	swordEnemys.clear();
	swordEnemys.shrink_to_fit();
	waveCount = 1;
	isWaveMoving = false;
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
	for (int i = 0; i < swordEnemys.size();i++) {
		swordEnemys[i]->Update();
	}
	//gunEnemy->Update();

	Colision();
	////敵の数が足りないとき補充
	EnemySpawn();

	//ウェーブ進行判定
	if (isWaveMoving) {

		//必要グループ数更新
		waveKillGroupAssignment = waveCount * 3;
		easeFrame++;
		if (waveCount < 300) {
			if (easeFrame - 300 < 45) {
				wfcenter = Rv3Ease::OutQuad(WF_S, WF_CENTER, float(easeFrame - 300.f) / 45.0f);
			}
			else if (easeFrame - 300 >= 60 && easeFrame - 300 < 105) {
				wfcenter = Rv3Ease::InQuad(WF_CENTER, WF_E, (float(easeFrame - 300.f) - 60.0f ) / 45.0f);
			}
			else if (easeFrame - 300 > 105) {
				easeFrame = 0;
				isWaveMoving = false;
			}
		}
	}

	m_defeatPM->Prototype_Update();
	
}

void EnemyManager::Draw()
{
	////すべての敵を描画
	//for (auto& se : swordEnemys) {
	//	se->Draw();
	//}

	for (int i = 0; i < swordEnemys.size(); i++) {
		swordEnemys[i]->Draw();
	}

	//gunEnemy->Draw();

	if (isDebugMode) { swordEnemyMother->Draw(); }


}

void EnemyManager::UIDraw()
{
	WaveSprite.DrawSprite(wfcenter.x - 128, wfcenter.y - 128);
	WaveSprite.DrawSprite(1280.f - 256.f - 64.f, 0);
	WaveSprite.Draw();

	if (waveCount >= 10) {
		NumSprite.uvOffsetHandle = waveCount - 10;
	}
	else {
		NumSprite.uvOffsetHandle = waveCount;
	}

	NumSprite.DrawSprite(wfcenter.x - 128 + WF_X_OFFSET, wfcenter.y - 128);
	NumSprite.DrawSprite(1280.f - 64.f,0);
	NumSprite.Draw();
}

void EnemyManager::ParticleDraw()
{
	m_defeatPM->Prototype_Draw(defeatPtex);
}

void EnemyManager::DebugExecution()
{
	//デバッグ用の敵を動かす
	swordEnemyMother->Update();

	//プレイヤーの体力を固定
	//player->hitpoint = 10;
}

void EnemyManager::DebugDraw()
{
	;
	//if (!isDebugMode) { return; }

	//swordEnemyMother->DebugDraw();

	if (swordEnemys.empty()) {
		return;
	}

	ImguiMgr::Get()->StartDrawImgui("Enemys Status", 100, 200);

	ImGui::Text("se1 pos:%f %f %f", swordEnemys[0].get()->pos.x, swordEnemys[0].get()->pos.y, swordEnemys[0].get()->pos.z);

	ImguiMgr::Get()->EndDrawImgui();
}

void EnemyManager::EnemySpawn()
{
	if (isWaveMoving) {
		return;
	}

	//敵が全滅するたび、撃破グループ数加算し出現処理
	if (swordEnemys.size() <= 0) {
		killedGroup++;

		//グループ数がノルマ以上
		if (killedGroup >= waveKillGroupAssignment) {
			//ウェーブ進行
			isWaveMoving = true;
			killedGroup = 0;
			waveCount++;
			
			if (waveCount == 3) {
				gameCleared = true;
			}
			else {
				WaveDirections::Get()->PlayNextWaveDir();
			}

			return;
		}

		int spawnCount = NY_random::intrand_sl(WAVE_GROUP_MAX_SPAWN, WAVE_GROUP_MIN_SPAWN);

		groupSpawnPos = RVector3(NY_random::floatrand_sl(300, -300), 10, NY_random::floatrand_sl(300, 300));

		for (int i = 0; i < spawnCount; i++) {
			//出現座標のローカル座標を設定
			RVector3 spawnPos = groupSpawnPos - RVector3(NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad),
				0, NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad));

			swordEnemys.push_back(std::make_unique<SwordEnemy>(player.lock(), astar.lock()));
			swordEnemys[i]->pos = spawnPos;
		}

		//最後のグループのとき、エリート召喚
		if (killedGroup == waveKillGroupAssignment - 1 && waveCount == 2) {
			//gunEnemy->isAlive = true;
		}

	}


}

void EnemyManager::Colision()
{

	for (int i = 0; i < 25;i++) {
		//死んでる弾は判定しない
		if (!player.lock()->bullets[i].isAlive) { continue; }

		if (swordEnemys.size() == 0) { return; }

		for (auto se = swordEnemys.begin(); se != swordEnemys.end();) {
			if (RV3Colider::Colision2Sphere((*se)->bodyColision,player.lock()->bullets[i].sphere)) {
				//弾を消滅させ、エネミーにダメージ処理(消去)
				player.lock()->bullets[i].isAlive = false;
				m_defeatPM->Prototype_Add(16, (*se)->pos);
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

	//敵同士の重なりを回避
}
