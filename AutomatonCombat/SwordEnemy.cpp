#include "SwordEnemy.h"

#include <FbxLoader.h>
#include <NY_random.h>

std::shared_ptr<fbxModel> SwordEnemy::swordModel;
bool SwordEnemy::isLoaded = false;

SwordEnemy::SwordEnemy(Player* player,NavMeshAstar *astar)
{
	if (!isLoaded) {
		swordModel = std::make_shared<fbxModel>();
		swordModel.reset(FbxLoader::GetInstance()->LoadFBXFile("FiringRifle"));
		isLoaded = true;
	}

	s_object.swordObject = std::make_unique<Object3d>();
	s_object.swordObject->SetLoadedModelData(swordModel);
	s_object.player = player;
	m_swordWaitJudge	=	std::make_shared<Sword_WaitJudge>(&s_object);
	m_swordWaitAct		=	std::make_shared<Sword_WaitAct>(&s_object);
	m_swordAttackJudge =	std::make_shared<Sword_AttackJudge>(&s_object, m_swordWaitAct.get());
	m_swordAttackAct =		std::make_shared<Sword_AttackAct>(&s_object);
	m_approachJudge =		std::make_shared<Sword_ApproachJudge>(&s_object);
	m_approahAct =			std::make_shared<Sword_ApproachingAct>(&s_object,astar);
	m_chargeJudge =			std::make_shared<Sword_ChargeJudge>(&s_object);
	m_chargeAct =			std::make_shared<Sword_ChargeAct>(&s_object);

	rootNode	= std::make_shared<BehaviorBaseNode>();
	actNode		= std::make_shared<BehaviorBaseNode>();
	waitNode	= std::make_shared<BehaviorBaseNode>();
	approachNode = std::make_shared<BehaviorBaseNode>();
	chargeNode = std::make_shared<BehaviorBaseNode>();

	//実行ノード生成
	waitNode->CreateActionNode("sword_wait", m_swordWaitAct.get(), m_swordWaitJudge.get());
	actNode->CreateActionNode("sword_act", m_swordAttackAct.get(), m_swordAttackJudge.get());
	approachNode->CreateActionNode("Sword_app", m_approahAct.get(), m_approachJudge.get());
	chargeNode->CreateActionNode("Sword_Charge", m_chargeAct.get(), m_chargeJudge.get());

	//ルートノードの選択候補を追加
	rootNode->AddjudgeNodeChild(waitNode.get());
	rootNode->AddjudgeNodeChild(approachNode.get());
	rootNode->AddjudgeNodeChild(chargeNode.get());

	swordEnemyTree.Init(rootNode.get());
	editor.Init(&swordEnemyTree);
	editor.AddEditData_Node(actNode.get());
	editor.AddEditData_Node(waitNode.get());
	editor.AddEditData_Node(approachNode.get());
	editor.AddEditData_Node(chargeNode.get());

	deathpm = ParticleManager::Create();
	dptex = TexManager::LoadTexture("Resources/effect1.png");
	dp = new DeathParticle;
	deathpm->Prototype_Set(dp);
}

SwordEnemy::~SwordEnemy()
{
	//if (m_approachJudge != nullptr) { delete m_approachJudge; }
	//if(m_approahAct != nullptr){ delete m_approahAct; }
	//if (m_chargeAct != nullptr) { delete m_chargeAct; }
	//if (m_chargeJudge != nullptr) { delete m_chargeJudge; }
	//if (m_swordAttackAct) { delete m_swordAttackAct; }
	//if (m_swordAttackJudge ) { delete m_swordAttackJudge; }
	//if (m_swordWaitAct ) { delete m_swordWaitAct; }
	//if (m_swordWaitJudge ) { delete m_swordWaitJudge; }
	//if (rootNode ) { delete rootNode; }
	//if (actNode ) { delete actNode; }
	//if (waitNode ) { delete waitNode; }
	//if (chargeNode ) { delete chargeNode; }
	//if (approachNode ) { delete approachNode;	}
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

	s_object.pos += s_object.mVec;

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

SwordEnemy* SwordEnemy::clone(Player* player,NavMeshAstar *a)
{
	SwordEnemy* clone = new SwordEnemy(player, a);
	clone->Init();
	clone->rootNode = rootNode;
	clone->actNode = actNode;
	clone->approachNode = approachNode;
	clone->chargeNode = chargeNode;
	clone->waitNode = waitNode;

	return clone;
}

void SwordEnemy::ParticleDraw()
{
	deathpm->Draw(dptex);
}

void DeathParticle::Init()
{
	nowFrame = 0;

	endFrame = 60;

	scale = 5.0f;

	color = { 1.0f,1.0f,1.0f,1.0f };

	vel = RVector3(NY_random::floatrand_sl(3.0f, -3.0f), NY_random::floatrand_sl(3.0f, -3.0f), NY_random::floatrand_sl(3.0f, -3.0f));
}

void DeathParticle::Update()
{
	pos += vel;
	nowFrame++;

}

ParticlePrototype* DeathParticle::clone(RVector3 pos)
{
	DeathParticle* p = new DeathParticle();

	p->pos = pos;
	p->vel = vel;

	return p;
}

void DeathParticle::SetVec(RVector3 vec)
{
	vel = vec;
}
