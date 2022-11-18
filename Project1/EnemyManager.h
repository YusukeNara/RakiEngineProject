#pragma once

#include "SwordEnemy.h"
#include "Player.h"
#include <Sprite.h>

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

	

	//3d�`��
	void Draw();
	//2d�`��
	void UIDraw();

	//�f�o�b�O���[�h
	void DebugExecution();

	void DebugDraw();

	//�L�����J�E���g
	int killCount;
	//�E�F�[�u���J�E���g
	int waveCount = 0;
	//�E�F�[�u���Ƃ̃L���J�E���g
	int waveKillCount = 1;

private:

	Player* player;

	//�f�o�b�O�p������
	SwordEnemy* swordEnemyMother;

	//�G���i�[�R���e�i
	std::vector<SwordEnemy*> swordEnemys;


	//�f�o�b�O�t���O
	bool isDebugMode = false;

	//�G�o������
	void EnemySpawn();

	//�v���C���[�e�ƓG�̏Փ˔���
	void Colision();
	//�G�̍U���̏Փ˔���̓r�w�C�r�A�����Ɏ����i�S�������j

	//ui�\��
	Sprite WaveSprite;
	Sprite NumSprite;

	RVector3 wfcenter;
	const RVector3 WF_CENTER = RVector3(1280 / 3, 400, 0);
	const RVector3 WF_S = RVector3(1280 / 3, 900, 0);
	const RVector3 WF_E = RVector3(1280 / 3, -100, 0);
	const float WF_X_OFFSET = 256;

	int easeFrame;
	const int WF_E_MAXFRAME = 45;

	bool isWaveMoving = false;


};

