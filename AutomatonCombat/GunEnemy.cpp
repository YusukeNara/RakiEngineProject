#include "GunEnemy.h"

void GunEnemy::Init()
{
	Hitpoint = 100;

	nData.LoadNavMesh("Resources/NavMeshTestData.txt");
	astar.SetNavMeshData(nData.navMeshData);
	std::vector<NavMesh> result;
	//astar.NavMeshSearchAstar(&nData.navMeshData[0], &nData.navMeshData[6], result);

	rootNode = new BehaviorBaseNode;
	shotNode = new BehaviorBaseNode;
	waitNode = new BehaviorBaseNode;

	gun_wjudgeNode = new Gun_WaitJudge(this, player,&this->pos);
	gun_wactNode = new Gun_WaitAct(this, player, &this->pos, &astar);
	gun_sactNode = new Gun_ShotAct(this, player, &this->pos);
	gunsjudgeNode = new Gun_ShotJudge(this, player, &this->pos);

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

	BarSprite.Create(TexManager::LoadTexture("Resources/white.png"));

	bossText.Create(TexManager::LoadTexture("Resources/BossText.png"));

	bodyCollider.center = pos;
	bodyCollider.rad = 30.f;
}

void GunEnemy::Update()
{
	if (!isAlive) {
		return;
	}

	gunEnemyTree.Run();

	bullet.Update();

	hpBarModel->SetAffineParamTranslate(RVector3(pos.x, pos.y + 100, pos.z));
	bodyCollider.center = pos;
}

void GunEnemy::Draw()
{
	if (!isAlive) {
		return;
	}

	object3d->DrawObject();

	hpBarModel->DrawObject();
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

void GunEnemy::UIDraw()
{
	float center = 1280.f / 2.f;

	float hpbarsize = (HP_BAR_MAXSIZE * float(Hitpoint) / 100.f) * 3;

	BarSprite.DrawExtendSprite(center - hpbarsize, 50.f, center + hpbarsize, 100.f);
	BarSprite.Draw();

	bossText.DrawExtendSprite(center - 100.f, 0.f, center + 100.f, 50.f);
	bossText.Draw();
}

void GunEnemy::ParticleDraw()
{
	bullet.Draw();
}

void GunEnemy::ShotBullet()
{
	bullet.Fire(object3d->position, RVector3(object3d->position - player->pos).norm(), -5.0f, 32, bModel);

}

void GunEnemy::Damage(int dmg)
{
	Hitpoint -= dmg;

	defeatPM->Prototype_Add(3, pos);

	if (Hitpoint < 0) {
		Hitpoint = 0;
		isAlive = false;
	}
}
