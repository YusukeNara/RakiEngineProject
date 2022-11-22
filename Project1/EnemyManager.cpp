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
	//�������̃G�l�~�[�I�u�W�F�N�g�𐶐�
	this->player = player;
	swordEnemyMother = new SwordEnemy(player);

	gunEnemy = new GunEnemy();
	gunEnemy->SetPlayer(player);
	gunEnemy->Init();

	killCount = 0;
	waveCount = 1;
	waveKillCount = 1;

	NumSprite.CreateAndSetDivisionUVOffsets(10, 5, 2, 64, 64, TexManager::LoadTexture("Resources/zenNum.png"));
	WaveSprite.Create(TexManager::LoadTexture("Resources/waveFont.png"));

	easeFrame = 0;
	isWaveMoving = true;

	killedGroup = 0;
	waveKillGroupAssignment = 3;

	//�ŏ��̏o������
	int spawnCount = NY_random::intrand_sl(WAVE_GROUP_MAX_SPAWN, WAVE_GROUP_MIN_SPAWN);
	groupSpawnPos = RVector3(NY_random::floatrand_sl(300, -300), 10, NY_random::floatrand_sl(300, 300));
	for (int i = 0; i < spawnCount; i++) {
		//�o�����W�̃��[�J�����W��ݒ�
		RVector3 spawnPos = groupSpawnPos - RVector3(NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad),
			0, NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad));

		swordEnemys.push_back(swordEnemyMother->clone(player));
		swordEnemys[i]->s_object.pos = spawnPos;
	}

}

void EnemyManager::Reset()
{
	gameCleared = false;
	swordEnemys.clear();
	swordEnemys.shrink_to_fit();
}

void EnemyManager::Update()
{
	if (Input::isKeyTrigger(DIK_F1)) {
		if (isDebugMode) { isDebugMode = false; }
		else { isDebugMode = true; }
	}

	//�f�o�b�O�����̂ݎ��s
	if (isDebugMode) {
		DebugExecution();
		return;
	}

	//���ׂĂ̓G���X�V
	for (auto& se : swordEnemys) {
		se->Update();
	}
	gunEnemy->Update();

	Colision();
	////�G�̐�������Ȃ��Ƃ���[
	EnemySpawn();

	//�E�F�[�u�i�s����
	if (isWaveMoving) {
		//�K�v�O���[�v���X�V
		waveKillGroupAssignment = waveCount * 3;
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
	////���ׂĂ̓G��`��
	//for (auto& se : swordEnemys) {
	//	se->Draw();
	//}

	for (int i = 0; i < swordEnemys.size(); i++) {
		swordEnemys[i]->Draw();
	}

	gunEnemy->Draw();

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
	//�f�o�b�O�p�̓G�𓮂���
	swordEnemyMother->Update();

	//�v���C���[�̗̑͂��Œ�
	player->hitpoint = 10;
}

void EnemyManager::DebugDraw()
{
	ImguiMgr::Get()->StartDrawImgui("Enemys Status", 100, 200);

	ImGui::Text("mustKillGroup : %d  killedGroup : %d waveCount : %d", waveKillGroupAssignment, killedGroup, waveCount);

	ImguiMgr::Get()->EndDrawImgui();

	if (!isDebugMode) { return; }

	swordEnemyMother->DebugDraw();


}

void EnemyManager::EnemySpawn()
{
	//�G���S�ł��邽�сA���j�O���[�v�����Z���o������
	if (swordEnemys.size() <= 0) {
		killedGroup++;

		//�O���[�v�����m���}�ȏ�
		if (killedGroup >= waveKillGroupAssignment) {
			//�E�F�[�u�i�s
			isWaveMoving = true;
			killedGroup = 0;
			waveCount++;
			if (waveCount == 3) {
				gameCleared = true;
			}

			return;
		}

		int spawnCount = NY_random::intrand_sl(WAVE_GROUP_MAX_SPAWN, WAVE_GROUP_MIN_SPAWN);

		groupSpawnPos = RVector3(NY_random::floatrand_sl(300, -300), 10, NY_random::floatrand_sl(300, 300));

		for (int i = 0; i < spawnCount; i++) {
			//�o�����W�̃��[�J�����W��ݒ�
			RVector3 spawnPos = groupSpawnPos - RVector3(NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad),
				0, NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad));

			swordEnemys.push_back(swordEnemyMother->clone(player));
			swordEnemys[i]->s_object.pos = spawnPos;
		}

		//�Ō�̃O���[�v�̂Ƃ��A�G���[�g����
		if (killedGroup == waveKillGroupAssignment - 1 && waveCount == 2) {
			gunEnemy->isAlive = true;
		}

	}


}

void EnemyManager::Colision()
{

	for (int i = 0; i < 25;i++) {
		//����ł�e�͔��肵�Ȃ�
		if (!player->bullets[i].isAlive) { continue; }

		if (swordEnemys.size() == 0) { return; }

		for (auto se = swordEnemys.begin(); se != swordEnemys.end();) {
			if (RV3Colider::Colision2Sphere((*se)->s_object.bodyColision,player->bullets[i].sphere)) {
				//�e�����ł����A�G�l�~�[�Ƀ_���[�W����(����)
				player->bullets[i].isAlive = false;
				se = swordEnemys.erase(se);
				
				killCount++;
				//���̃��[�v�Ŕ��肷��e�͏������̂Ŕ�����
				break;
			}
			else {
				//���̓G�𔻒�
				se++;
			}
		}
	}

}
