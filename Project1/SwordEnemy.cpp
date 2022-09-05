#include "SwordEnemy.h"

SwordEnemy::SwordEnemy(Player* player)
{
	s_object.swordObject = LoadModel_ObjFile("enemy1");
	s_object.player = player;
	m_swordWaitJudge	= new Sword_WaitJudge(&s_object);
	m_swordWaitAct		= new Sword_WaitAct(&s_object);
	m_swordAttackJudge = new Sword_AttackJudge(&s_object, m_swordWaitAct);
	m_swordAttackAct = new Sword_AttackAct(&s_object);
	m_approachJudge = new Sword_ApproachJudge(&s_object);
	m_approahAct = new Sword_ApproachingAct(&s_object);
	m_chargeJudge = new Sword_ChargeJudge(&s_object);
	m_chargeAct = new Sword_ChargeAct(&s_object);

	rootNode	= new BehaviorBaseNode;
	actNode		= new BehaviorBaseNode;
	waitNode	= new BehaviorBaseNode;
	approachNode = new BehaviorBaseNode;
	chargeNode = new BehaviorBaseNode;

	//���s�m�[�h����
	waitNode->CreateActionNode("sword_wait", m_swordWaitAct, m_swordWaitJudge);
	actNode->CreateActionNode("sword_act", m_swordAttackAct, m_swordAttackJudge);
	approachNode->CreateActionNode("Sword_app", m_approahAct, m_approachJudge);
	chargeNode->CreateActionNode("Sword_Charge", m_chargeAct, m_chargeJudge);

	//���[�g�m�[�h�̑I������ǉ�
	rootNode->AddjudgeNodeChild(waitNode);
	rootNode->AddjudgeNodeChild(actNode);
	rootNode->AddjudgeNodeChild(approachNode);
	rootNode->AddjudgeNodeChild(chargeNode);

	swordEnemyTree.Init(rootNode);
	editor.Init(&swordEnemyTree);
	editor.AddEditData_Node(actNode);
	editor.AddEditData_Node(waitNode);
	editor.AddEditData_Node(approachNode);
	editor.AddEditData_Node(chargeNode);

}

SwordEnemy::~SwordEnemy()
{
	delete m_approachJudge;
	delete m_approahAct;
	delete m_chargeAct;
	delete m_chargeJudge;
	delete m_swordAttackAct;
	delete m_swordAttackJudge;
	delete m_swordWaitAct;
	delete m_swordWaitJudge;
	delete rootNode;
	delete actNode;
	delete waitNode;
	delete chargeNode;
	delete approachNode;
}

void SwordEnemy::Init()
{
	//�����p�����[�^�[�ݒ�
	s_object.pos.zero();
	s_object.pos.z = 100.0f;
	s_object.bodyColision.center = s_object.pos;
	s_object.bodyColision.rad = 10.0f;

}

void SwordEnemy::Update()
{
	//�c���[�̎��s
	swordEnemyTree.Run();
	s_object.bodyColision.center = s_object.pos;
}

void SwordEnemy::Draw()
{
	//�p�����[�^�Z�b�g
	s_object.swordObject->SetAffineParam(RVector3(10.0f, 10.0f, 10.0f), RVector3(0, 270, 0), s_object.pos);
	s_object.swordObject->DrawObject();
}

void SwordEnemy::DebugDraw()
{
	editor.EditorDraw();
	editor.NodeDataDraw();
	editor.ObjectDataDraw();
}

SwordEnemy* SwordEnemy::clone(Player* player)
{
	SwordEnemy* clone = new SwordEnemy(player);
	clone->Init();

	return clone;
}
