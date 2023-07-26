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
	std::unique_ptr<BehaviorBaseNode> rootNode;
	//�s������
	std::unique_ptr<BehaviorBaseNode> actNode;
	std::unique_ptr<BehaviorBaseNode> waitNode;
	std::unique_ptr<BehaviorBaseNode> approachNode;
	std::unique_ptr<BehaviorBaseNode> chargeNode;

	//�s���\���I�u�W�F�N�g
	std::unique_ptr<Sword_WaitJudge> m_swordWaitJudge;
	std::unique_ptr<Sword_WaitAct> m_swordWaitAct;
	std::unique_ptr<Sword_AttackJudge> m_swordAttackJudge;
	std::unique_ptr<Sword_AttackAct> m_swordAttackAct;
	std::unique_ptr<Sword_ApproachJudge> m_approachJudge;
	std::unique_ptr<Sword_ApproachingAct> m_approahAct;
	std::unique_ptr<Sword_ChargeJudge> m_chargeJudge;
	std::unique_ptr<Sword_ChargeAct> m_chargeAct;

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
	std::unique_ptr<SwordEnemyObject> s_object;

	BehaviorEditor editor;

	//����
	SwordEnemy* clone(Player* player, NavMeshAstar* a);

};




