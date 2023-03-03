#pragma once
#include <BehaviorTree.h>
#include <BehaviorEditor.h>
#include <RVector.h>
#include <memory>
#include <BehaviorNodeBase.h>
#include <BehaviorTreeMother.h>

#include "Player.h"
#include "SwordEnemyNodes.h"
#include <ParticleManager.h>
#include "EnemyDefeatEffect.h"
#include "NewSwordEnemyNodes.h"

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


//操作する
class SwordEnemy
{
private:
	//ビヘイビアツリー
	AI_BehaviorTree swordEnemyTree;

	//行動構成ノード
	//先頭
	std::shared_ptr<BehaviorBaseNode> rootNode;
	//行動分岐
	std::shared_ptr<BehaviorBaseNode> actNode;
	std::shared_ptr<BehaviorBaseNode> waitNode;
	std::shared_ptr<BehaviorBaseNode> approachNode;
	std::shared_ptr<BehaviorBaseNode> chargeNode;

	//行動構成オブジェクト
	std::shared_ptr<Sword_WaitJudge> m_swordWaitJudge;
	std::shared_ptr<Sword_WaitAct> m_swordWaitAct;
	std::shared_ptr<Sword_AttackJudge> m_swordAttackJudge;
	std::shared_ptr<Sword_AttackAct> m_swordAttackAct;
	std::shared_ptr<Sword_ApproachJudge> m_approachJudge;
	std::shared_ptr<Sword_ApproachingAct> m_approahAct;
	std::shared_ptr<Sword_ChargeJudge> m_chargeJudge;
	std::shared_ptr<Sword_ChargeAct> m_chargeAct;

public:
	//新設計ビヘイビア
	std::shared_ptr<SwordApproachNode>		tree_swordApproachNode;
	std::shared_ptr<SwordAttackNode>		tree_swordAttackNode;
	std::shared_ptr<SwordChargeNode>		tree_swordChargeNode;
	std::shared_ptr<SwordAttackJudgeNode>	tree_swordAtkJudgeNode;
	std::shared_ptr<bTreesys::BehaviorSelectorNode> tree_selector;
	std::shared_ptr<bTreesys::BehaviorSequenceNode> tree_sequence;
	std::shared_ptr<bTreesys::BehaviorRepeatNode>	tree_repeat;

	BehaviorTreeMother treeMother;

	RVector3 pos, vec;
	RV3Colider::Sphere bodyColision;

	//モデルデータ
	static std::shared_ptr<fbxModel> swordModel;
	static bool isLoaded;

	NavMeshAstar *navAstar;

	ParticleManager *deathpm;
	DeathParticle *dp;
	int dptex;

	//描画オブジェクト
	std::unique_ptr<Object3d> swordObject;

	std::weak_ptr<Player> playerPtr;

public:
	SwordEnemy(std::shared_ptr<Player> player, std::shared_ptr<NavMeshAstar> astar);
	~SwordEnemy();

	void Init();

	void Update();

	void Draw();

	void ParticleDraw();

	void DebugDraw();
	
	//オブジェクト
	SwordEnemyObject s_object;

	BehaviorEditor editor;

	//複製
	SwordEnemy* clone(std::shared_ptr<Player> player, std::shared_ptr<NavMeshAstar> a);

};




