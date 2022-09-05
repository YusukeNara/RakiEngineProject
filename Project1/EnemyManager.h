#pragma once

#include "SwordEnemy.h"
#include "Player.h"

#include <vector>

//�G�̐����A�X�V�A�`���S���N���X

class EnemyManager
{
public:


	EnemyManager();
	~EnemyManager();

	void Init(Player *player);
	void Reset();

	void Update();

	void Draw();

	//�f�o�b�O���[�h
	void DebugExecution();

	void DebugDraw();

private:

	Player* player;

	//�f�o�b�O�p������
	SwordEnemy* swordEnemyMother;

	//�G���i�[�R���e�i
	std::vector<SwordEnemy*> swordEnemys;

	//�L�����J�E���g
	int killCount;

	//�f�o�b�O�t���O
	bool isDebugMode = false;

	//�G�o������
	void EnemySpawn();

	//�v���C���[�e�ƓG�̏Փ˔���
	void Colision();
	//�G�̍U���̏Փ˔���̓r�w�C�r�A�����Ɏ����i�S�������j

};

