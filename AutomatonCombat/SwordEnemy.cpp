#include "SwordEnemy.h"

SwordEnemy::SwordEnemy(Player* player)
{
	s_object.swordObject = LoadModel_ObjFile("enemy_sword");
	s_object.player = player;
	m_swordWaitJudge	=	new Sword_WaitJudge(&s_object);
	m_swordWaitAct		=	new Sword_WaitAct(&s_object);
	m_swordAttackJudge =	new Sword_AttackJudge(&s_object, m_swordWaitAct);
	m_swordAttackAct =		new Sword_AttackAct(&s_object);
	m_approachJudge =		new Sword_ApproachJudge(&s_object);
	m_approahAct =			new Sword_ApproachingAct(&s_object);
	m_chargeJudge =			new Sword_ChargeJudge(&s_object);
	m_chargeAct =			new Sword_ChargeAct(&s_object);

	rootNode	= new BehaviorBaseNode;
	actNode		= new BehaviorBaseNode;
	waitNode	= new BehaviorBaseNode;
	approachNode = new BehaviorBaseNode;
	chargeNode = new BehaviorBaseNode;

	//実行ノード生成
	waitNode->CreateActionNode("sword_wait", m_swordWaitAct, m_swordWaitJudge);
	actNode->CreateActionNode("sword_act", m_swordAttackAct, m_swordAttackJudge);
	approachNode->CreateActionNode("Sword_app", m_approahAct, m_approachJudge);
	chargeNode->CreateActionNode("Sword_Charge", m_chargeAct, m_chargeJudge);

	//ルートノードの選択候補を追加
	rootNode->AddjudgeNodeChild(waitNode);
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
	if (m_approachJudge != nullptr) { delete m_approachJudge; }
	if(m_approahAct != nullptr){ delete m_approahAct; }
	if (m_chargeAct != nullptr) { delete m_chargeAct; }
	if (m_chargeJudge != nullptr) { delete m_chargeJudge; }
	if (m_swordAttackAct) { delete m_swordAttackAct; }
	if (m_swordAttackJudge ) { delete m_swordAttackJudge; }
	if (m_swordWaitAct ) { delete m_swordWaitAct; }
	if (m_swordWaitJudge ) { delete m_swordWaitJudge; }
	if (rootNode ) { delete rootNode; }
	if (actNode ) { delete actNode; }
	if (waitNode ) { delete waitNode; }
	if (chargeNode ) { delete chargeNode; }
	if (approachNode ) { delete approachNode;	}
}

void SwordEnemy::Init()
{
	//初期パラメーター設定
	s_object.pos.zero();
	s_object.pos.z = 100.0f;
	s_object.bodyColision.center = s_object.pos;
	s_object.bodyColision.rad = 10.0f;

	s_object.swordObject->SetAffineParamScale(RVector3(10.0f, 10.0f, 10.0f));
}

void SwordEnemy::Update()
{
	//ツリーの実行
	swordEnemyTree.Run();
	s_object.bodyColision.center = s_object.pos;
	if (s_object.bodyColision.center.y < 10) { 
		s_object.bodyColision.center.y = 10;
		s_object.pos.y = 10; 
	}
	else if (s_object.bodyColision.center.y > 10) {
		s_object.bodyColision.center.y = 10;
		s_object.pos.y = 10;
	}

}

void SwordEnemy::Draw()
{
	//パラメータセット
	s_object.swordObject->SetAffineParamTranslate(s_object.pos);
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
	clone->rootNode = rootNode;
	clone->actNode = actNode;
	clone->approachNode = approachNode;
	clone->chargeNode = chargeNode;
	clone->waitNode = waitNode;

	return clone;
}
