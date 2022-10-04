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

	//�f�o�b�O�����̂ݎ��s
	if (isDebugMode) {
		DebugExecution();
		return;
	}

	//���ׂĂ̓G���X�V
	for (auto& se : swordEnemys) {
		se->Update();
	}
	Colision();
	//�G�̐�������Ȃ��Ƃ���[
	EnemySpawn();
}

void EnemyManager::Draw()
{
	//���ׂĂ̓G��`��
	for (auto& se : swordEnemys) {
		se->Draw();
	}

	if (isDebugMode) { swordEnemyMother->Draw(); }
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
}

void EnemyManager::EnemySpawn()
{
	//�G�����Ȃ��Ƃ��A�����_���Ȉʒu�Ɉ��̐��o��������
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
		//����ł�e�͔��肵�Ȃ�
		if (!player->bullets[i].isAlive) { continue; }

		for (auto se = swordEnemys.begin(); se != swordEnemys.end();) {
			if (RV3Colider::Colision2Sphere((*se)->s_object.bodyColision, player->bullets[i].coliSphere)) {
				//�e�����ł����A�G�l�~�[�Ƀ_���[�W����(����)
				player->bullets[i].isAlive = false;
				se = swordEnemys.erase(se);
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
