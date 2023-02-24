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

	swordObject = std::make_unique<Object3d>();
	swordObject->SetLoadedModelData(swordModel);
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

	//���s�m�[�h����
	waitNode->CreateActionNode("sword_wait", m_swordWaitAct, m_swordWaitJudge);
	actNode->CreateActionNode("sword_act", m_swordAttackAct, m_swordAttackJudge);
	approachNode->CreateActionNode("Sword_app", m_approahAct, m_approachJudge);
	chargeNode->CreateActionNode("Sword_Charge", m_chargeAct, m_chargeJudge);

	//���[�g�m�[�h�̑I������ǉ�
	rootNode->AddjudgeNodeChild(waitNode);
	rootNode->AddjudgeNodeChild(approachNode);
	rootNode->AddjudgeNodeChild(chargeNode);

	swordEnemyTree.Init(rootNode);
	editor.Init(&swordEnemyTree);
	editor.AddEditData_Node(actNode);
	editor.AddEditData_Node(waitNode);
	editor.AddEditData_Node(approachNode);
	editor.AddEditData_Node(chargeNode);

	deathpm = ParticleManager::Create();
	dptex = TexManager::LoadTexture("Resources/effect1.png");
	dp = new DeathParticle;
	deathpm->Prototype_Set(dp);


}

SwordEnemy::~SwordEnemy()
{

}

void SwordEnemy::Init()
{
	//�����p�����[�^�[�ݒ�
	bodyColision.center = pos;
	bodyColision.rad = 10.0f;
}

void SwordEnemy::Update()
{
	//�c���[�̎��s

}

void SwordEnemy::Draw()
{
	//�p�����[�^�Z�b�g
	swordObject->SetAffineParamTranslate(pos);
	swordObject->DrawObject();
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
