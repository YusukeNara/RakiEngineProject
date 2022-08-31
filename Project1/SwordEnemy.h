#pragma once
#include <BehaviorTree.h>
#include <BehaviorEditor.h>

#include "Player.h"
#include "SwordEnemyNodes.h"



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
	

public:
	SwordEnemy(Player* player);
	~SwordEnemy();

	void Init();

	void Update();

	void Draw();

	void DebugDraw();
	
	//�I�u�W�F�N�g
	SwordEnemyObject s_object;

	BehaviorEditor editor;
};



