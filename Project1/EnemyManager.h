#pragma once

#include "SwordEnemy.h"
#include "Player.h"

#include <vector>

//�G�̐����A�X�V�A�`���S���N���X

class EnemyManager
{
public:

	void Init();

	void Update();

	void Draw();

private:


	//�G���i�[�R���e�i
	std::vector<SwordEnemy*> swordEnemys;

	//�G�o��
	void EnemySpawn();

	//


};

