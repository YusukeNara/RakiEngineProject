#pragma once
#include <BehaviorTree.h>
#include <BehaviorEditor.h>
#include <RVector.h>

#include "Player.h"
#include "SwordEnemyNodes.h"
#include <ParticleManager.h>
#include "EnemyDefeatEffect.h"

class DeathParticle :public ParticlePrototype
{
public:
	DeathParticle() {
		Init();
	};

	virtual void Init() override;

	virtual void Update() override;

	virtual ParticlePrototype* clone(RVector3 pos) override;

	void SetVec(RVector3 vec);

private:

	RVector3 vec;
};


//���삷��
class SwordEnemy
{
private:
	//�r�w�C�r�A�c���[
	AI_BehaviorTree swordEnemyTree;

	//�s���\���m�[�h
	//�擪
	BehaviorBaseNode* rootNode;
	//�s������
	BehaviorBaseNode* actNode;
	BehaviorBaseNode* waitNode;
	BehaviorBaseNode* approachNode;
	BehaviorBaseNode* chargeNode;

	//�s���\���I�u�W�F�N�g
	Sword_WaitJudge* m_swordWaitJudge;
	Sword_WaitAct* m_swordWaitAct;
	Sword_AttackJudge* m_swordAttackJudge;
	Sword_AttackAct* m_swordAttackAct;
	Sword_ApproachJudge* m_approachJudge;
	Sword_ApproachingAct* m_approahAct;
	Sword_ChargeJudge* m_chargeJudge;
	Sword_ChargeAct* m_chargeAct;

	//���f���f�[�^
	static std::shared_ptr<fbxModel> swordModel;
	static bool isLoaded;

	NavMeshAstar *navAstar;

	ParticleManager *deathpm;
	DeathParticle *dp;
	int dptex;

public:
	SwordEnemy(Player* player,NavMeshAstar *astar);
	~SwordEnemy();

	void Init();

	void Update();

	void Draw();

	void ParticleDraw();

	void DebugDraw();
	
	//�I�u�W�F�N�g
	SwordEnemyObject s_object;

	BehaviorEditor editor;

	//����
	SwordEnemy* clone(Player* player, NavMeshAstar* a);

};




