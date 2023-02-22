#pragma once

#include "SwordEnemy.h"
#include "GunEnemy.h"
#include "Player.h"
#include <Sprite.h>

#include <vector>

#include "EnemyDefeatEffect.h"

//�G�̐����A�X�V�A�`���S���N���X

enum class WAVE_STATUS
{
	WAVE_STANDBY,
	WAVE_RUN,
};

class EnemyManager
{
public:


	EnemyManager();
	~EnemyManager();

	void Init(Player *player,NavMeshAstar *astar);
	void Reset();

	void Update();

	

	//3d�`��
	void Draw();
	//2d�`��
	void UIDraw();
	void ParticleDraw();

	//�f�o�b�O���[�h
	void DebugExecution();

	void DebugDraw();

	//�L�����J�E���g
	int killCount;
	//�E�F�[�u���J�E���g
	int waveCount = 0;
	//���̃E�F�[�u�ŃL�����Ȃ��Ƃ����Ȃ��L����
	int waveKillCount = 1;
	//�O���[�v��
	int killedGroup = 0;
	int waveKillGroupAssignment = 0;

	//�N���A�t���O
	bool gameCleared = false;

private:

	Player* player;

	//�f�o�b�O�p������
	SwordEnemy* swordEnemyMother;

	std::unique_ptr<GunEnemy> gunEnemy;

	//�G���i�[�R���e�i
	std::vector<std::unique_ptr<SwordEnemy>> swordEnemys;


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

	//�E�F�[�u�Ǘ�

	const int WAVE_GROUP_MAX_SPAWN = 8;
	const int WAVE_GROUP_MIN_SPAWN = 4;
	//�O���[�v�o�����S
	RVector3	groupSpawnPos;
	const float groupSpawnRad = 64.0f;

	ParticleManager* m_defeatPM;
	DefeatParticle* m_defeatP;

	int defeatPtex;

	NavMeshAstar* astar;
};

