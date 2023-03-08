#include "SwordEnemy.h"

#include <FbxLoader.h>
#include <NY_random.h>

std::shared_ptr<fbxModel> SwordEnemy::swordModel;
bool SwordEnemy::isLoaded = false;

SwordEnemy::SwordEnemy(std::shared_ptr<Player> player, std::shared_ptr<NavMeshAstar> astar)
{
	if (!isLoaded) {
		swordModel = std::make_shared<fbxModel>();
		swordModel.reset(FbxLoader::GetInstance()->LoadFBXFile("FiringRifle"));
		isLoaded = true;
	}
	object3d = std::make_unique<Object3d>();
	object3d->SetLoadedModelData(swordModel);
	//s_object.player = player.get();
	//m_swordWaitJudge	=	std::make_shared<Sword_WaitJudge>(&s_object);
	//m_swordWaitAct		=	std::make_shared<Sword_WaitAct>(&s_object);
	//m_swordAttackJudge =	std::make_shared<Sword_AttackJudge>(&s_object, m_swordWaitAct.get());
	//m_swordAttackAct =		std::make_shared<Sword_AttackAct>(&s_object);
	//m_approachJudge =		std::make_shared<Sword_ApproachJudge>(&s_object);
	//m_approahAct =			std::make_shared<Sword_ApproachingAct>(&s_object,astar);
	//m_chargeJudge =			std::make_shared<Sword_ChargeJudge>(&s_object);
	//m_chargeAct =			std::make_shared<Sword_ChargeAct>(&s_object);

	//rootNode	= std::make_shared<BehaviorBaseNode>();
	//actNode		= std::make_shared<BehaviorBaseNode>();
	//waitNode	= std::make_shared<BehaviorBaseNode>();
	//approachNode = std::make_shared<BehaviorBaseNode>();
	//chargeNode = std::make_shared<BehaviorBaseNode>();

	////実行ノード生成
	//waitNode->CreateActionNode("sword_wait", m_swordWaitAct, m_swordWaitJudge);
	//actNode->CreateActionNode("sword_act", m_swordAttackAct, m_swordAttackJudge);
	//approachNode->CreateActionNode("Sword_app", m_approahAct, m_approachJudge);
	//chargeNode->CreateActionNode("Sword_Charge", m_chargeAct, m_chargeJudge);

	////ルートノードの選択候補を追加
	//rootNode->AddjudgeNodeChild(waitNode);
	//rootNode->AddjudgeNodeChild(approachNode);
	//rootNode->AddjudgeNodeChild(chargeNode);

	//swordEnemyTree.Init(rootNode);
	//editor.Init(&swordEnemyTree);
	//editor.AddEditData_Node(actNode);
	//editor.AddEditData_Node(waitNode);
	//editor.AddEditData_Node(approachNode);
	//editor.AddEditData_Node(chargeNode);

	object3d->SetAffineParamScale(RVector3(5.f, 5.f, 5.f));

	//新設計ビヘイビア
	tree_swordApproachNode	= std::make_shared<SwordApproachNode>();
	tree_swordAtkJudgeNode	= std::make_shared<SwordAttackJudgeNode>();
	tree_swordAttackNode	= std::make_shared<SwordAttackNode>();
	tree_swordChargeNode	= std::make_shared<SwordChargeNode>();
	tree_sequence = std::make_shared<bTreesys::BehaviorSequenceNode>();
	tree_selector = std::make_shared<bTreesys::BehaviorSelectorNode>();
	tree_repeat = std::make_shared<bTreesys::BehaviorRepeatNode>();

	tree_swordApproachNode->SetPointer(this, player, astar);
	tree_swordAtkJudgeNode->SetPointer(this, player);
	tree_swordAttackNode->SetPointer(this, player);
	tree_swordChargeNode->SetPointer(this, player);

	tree_selector->AddChild(tree_sequence);
	tree_selector->AddChild(tree_swordApproachNode);
	tree_sequence->AddChild(tree_swordAtkJudgeNode);
	tree_sequence->AddChild(tree_swordChargeNode);

	treeMother.TreeInit(tree_selector);

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
	//初期パラメーター設定
	bodyColision.center = pos;
	bodyColision.rad = 10.0f;
}

void SwordEnemy::Update()
{
	//ツリーの実行
	treeMother.Run();

}

void SwordEnemy::Draw()
{
	//パラメータセット
	object3d->SetAffineParamTranslate(pos);

	object3d->UpdateObject3D();

	object3d->DrawObject();
}

void SwordEnemy::DebugDraw()
{
	editor.EditorDraw();
	editor.NodeDataDraw();
	editor.ObjectDataDraw();
}

SwordEnemy* SwordEnemy::clone(const std::shared_ptr<Player> &player, const std::shared_ptr<NavMeshAstar> &a)
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
