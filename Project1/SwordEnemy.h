#pragma once
#include <BehaviorTree.h>
#include <BehaviorEditor.h>

#include "Player.h"
#include "SwordEnemyNodes.h"



//操作する
class SwordEnemy
{
private:
	//ビヘイビアツリー
	AI_BehaviorTree swordEnemyTree;


	//行動構成ノード

	//先頭
	BehaviorBaseNode* rootNode;
	//行動分岐
	BehaviorBaseNode* actNode;
	BehaviorBaseNode* waitNode;
	BehaviorBaseNode* approachNode;
	BehaviorBaseNode* chargeNode;


	//行動構成オブジェクト
	Sword_WaitJudge* m_swordWaitJudge;
	Sword_WaitAct* m_swordWaitAct;
	Sword_AttackJudge* m_swordAttackJudge;
	Sword_AttackAct* m_swordAttackAct;
	Sword_ApproachJudge* m_approachJudge;
	Sword_ApproachingAct* m_approahAct;
	Sword_ChargeJudge* m_chargeJudge;
	Sword_ChargeAct* m_chargeAct;
	

public:
	SwordEnemy(Player* player);
	~SwordEnemy();

	void Init();

	void Update();

	void Draw();

	void DebugDraw();
	
	//オブジェクト
	SwordEnemyObject s_object;

	BehaviorEditor editor;
};



