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

void EnemyManager::Init(Player *player,NavMeshAstar *astar)
{
	//�������̃G�l�~�[�I�u�W�F�N�g�𐶐�
	this->player = player;
	swordEnemyMother = new SwordEnemy(player, astar);
	swordEnemyMother->s_object.swordObject->SetAffineParamScale(RVector3(5.f, 5.f, 5.f));

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

	this->astar = astar;

	//�ŏ��̏o������
	int spawnCount = NY_random::intrand_sl(WAVE_GROUP_MAX_SPAWN, WAVE_GROUP_MIN_SPAWN);
	groupSpawnPos = RVector3(NY_random::floatrand_sl(300, -300), 10, NY_random::floatrand_sl(300, 300));
	for (int i = 0; i < spawnCount; i++) {
		//�o�����W�̃��[�J�����W��ݒ�
		RVector3 spawnPos = groupSpawnPos - RVector3(NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad),
			0, NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad));

		swordEnemys.push_back(swordEnemyMother->clone(player, astar));
		swordEnemys[i]->s_object.pos = spawnPos;
	}

	m_defeatP = new DefeatParticle;
	m_defeatPM = ParticleManager::Create();
	m_defeatPM->Prototype_Set(m_defeatP);

	m_spawnP = new EnemySpawnEffect;
	m_spawnPM.reset(ParticleManager::Create());
	m_spawnPM->Prototype_Set(m_spawnP);


	defeatPtex = TexManager::LoadTexture("Resources/effect1.png");

	gunEnemy->defeatPM = m_defeatPM;
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

	m_spawnPM->Prototype_Update();
	
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
	WaveSprite.DrawSprite(1280.f - 256.f - 64.f, 0);
	WaveSprite.Draw();

	if (waveCount >= 10) {
		NumSprite.uvOffsetHandle = waveCount - 10;
	}
	else {
		NumSprite.uvOffsetHandle = waveCount;
	}

	if (gunEnemy->isAlive) {
		gunEnemy->UIDraw();
	}

	NumSprite.DrawSprite(wfcenter.x - 128 + WF_X_OFFSET, wfcenter.y - 128);
	NumSprite.DrawSprite(1280.f - 64.f,0);
	NumSprite.Draw();
}

void EnemyManager::ParticleDraw()
{
	m_defeatPM->Prototype_Draw(defeatPtex);
	m_spawnPM->Prototype_Draw(defeatPtex);

	gunEnemy->ParticleDraw();
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

	if (!isDebugMode) { return; }

	swordEnemyMother->DebugDraw();

	ImguiMgr::Get()->StartDrawImgui("Enemys Status", 100, 200);

	ImGui::Text("mustKillGroup : %d  killedGroup : %d waveCount : %d", waveKillGroupAssignment, killedGroup, waveCount);

	ImguiMgr::Get()->EndDrawImgui();
}

void EnemyManager::EnemySpawn()
{
	if (isWaveMoving) {
		return;
	}

	//�G���S�ł��邽�сA���j�O���[�v�����Z���o������
	if (swordEnemys.size() <= 0) {
		killedGroup++;

		//�O���[�v�����m���}�ȏ�
		if (killedGroup >= waveKillGroupAssignment) {
			//�E�F�[�u�i�s
			isWaveMoving = true;
			killedGroup = 0;
			waveCount++;
			
			if (waveCount >= 3 && !gunEnemy->isAlive) {
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
			//�o�����W�̃��[�J�����W��ݒ�
			RVector3 spawnPos = groupSpawnPos - RVector3(NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad),
				0, NY_random::floatrand_sl(groupSpawnRad, -groupSpawnRad));

			swordEnemys.push_back(swordEnemyMother->clone(player, astar));
			swordEnemys[i]->s_object.pos = spawnPos;
		}

		//�Ō�̃O���[�v�̂Ƃ��A�G���[�g����
		if (killedGroup == waveKillGroupAssignment - 1 && waveCount == 2) {
			gunEnemy->isAlive = true;
		}

		//�G�t�F�N�g����
		m_spawnPM->Prototype_Add(50, groupSpawnPos);

	}

	//�G�t�F�N�g����
	m_spawnPM->Prototype_Add(10, groupSpawnPos);
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
				m_defeatPM->Prototype_Add(16, (*se)->s_object.pos);
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

		if (RV3Colider::Colision2Sphere(gunEnemy->bodyCollider, player->bullets[i].sphere)) {
			gunEnemy->Damage(2);
		}
	}

	//�G���m�̏d�Ȃ�����
	for (int i = 0; i < swordEnemys.size(); i++) {
		for (int j = 0; j < swordEnemys.size(); j++) {
			if (i == j) {
				continue;
			}

			RV3Colider::Sphere jsphere(swordEnemys[j]->s_object.pos + swordEnemys[j]->s_object.mVec,
				swordEnemys[j]->s_object.bodyColision.rad);

			if (RV3Colider::Colision2Sphere(
				swordEnemys[i]->s_object.bodyColision,
				jsphere))
			{
				swordEnemys[j]->s_object.mVec = RVector3(0, 0, 0);
			}
		}
	}



}
