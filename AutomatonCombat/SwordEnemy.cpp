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
	obj = std::make_shared<Object3d>();
	obj->SetLoadedModelData(swordModel);

	scale = RVector3(5.f, 5.f, 5.f);

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

	treeMother.TreeInit(tree_swordChargeNode);

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
	obj->SetAffineParam(scale, rot, pos);

	obj->DrawObject();
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
