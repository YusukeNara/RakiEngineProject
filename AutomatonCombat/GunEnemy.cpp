#include "GunEnemy.h"

void GunEnemy::Init()
{
	nData.LoadNavMesh("Resources/NavMeshTestData.txt");
	astar.SetNavMeshData(nData.navMeshData);
	std::vector<NavMesh> result;
	//astar.NavMeshSearchAstar(&nData.navMeshData[0], &nData.navMeshData[6], result);

	rootNode = std::make_shared<BehaviorBaseNode>();
	shotNode = std::make_shared<BehaviorBaseNode>();
	waitNode = std::make_shared<BehaviorBaseNode>();

	gun_wjudgeNode	= std::make_shared<Gun_WaitJudge>(this, player,&this->pos);
	gun_wactNode	= std::make_shared<Gun_WaitAct>(this, player, &this->pos, &astar);
	gun_sactNode	= std::make_shared<Gun_ShotAct>(this, player, &this->pos);
	gunsjudgeNode	= std::make_shared<Gun_ShotJudge>(this, player, &this->pos);

	shotNode->CreateActionNode("ShotNode", gun_sactNode, gunsjudgeNode);
	waitNode->CreateActionNode("WaitNode", gun_wactNode, gun_wjudgeNode);

	rootNode->AddjudgeNodeChild(shotNode);
	rootNode->AddjudgeNodeChild(waitNode);
	
	gunEnemyTree.Init(rootNode);

	object3d.reset(LoadModel_ObjFile("enemy1"));
	object3d->SetAffineParamScale(RVector3(20.0f, 20.0f, 20.0f));

	bModel = std::make_shared<Model3D>();
	bModel->LoadObjModel("Sphere");

	hpBarModel.reset(NY_Object3DManager::Get()->CreateModel_Tile(1, 1, 10, 10, TexManager::LoadTexture("Resources/white.png")));
	hpBarModel->SetAffineParam(RVector3(5, 100, 1),
		RVector3(0, 0, 0),
		RVector3(pos.x, pos.y + 100, pos.z));
	hpBarModel->isBillBoard = true;

	isAlive = false;

	bullet.Init();


}

void GunEnemy::Update()
{
	if (!isAlive) {
		return;
	}

	gunEnemyTree.Run();

	bullet.Update();

	hpBarModel->SetAffineParamTranslate(RVector3(pos.x, pos.y + 100, pos.z));
}

void GunEnemy::Draw()
{
	if (!isAlive) {
		return;
	}

	object3d->DrawObject();

	hpBarModel->DrawObject();

	bullet.Draw();
}

void GunEnemy::Finalize()
{
}

void GunEnemy::Load()
{
}

void GunEnemy::OnCollision(const ColliderInfo* info)
{
}

void GunEnemy::SetPlayer(Player* player)
{
	this->player = player;
}

void GunEnemy::ShotBullet()
{
	bullet.Fire(object3d->position, RVector3(object3d->position - player->pos).norm(), -5.0f, 32, bModel);

}
