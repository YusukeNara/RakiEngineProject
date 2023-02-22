#pragma once
#include <GameObject.h>
#include <BehaviorTree.h>
#include <BehaviorEditor.h>
#include <RVector.h>
#include "GunEnemyNodes.h"
#include "Bullet.h"

class GunEnemy : public GameObject
{
public:

	void Init()		override;
	void Update()	override;
	void Draw()		override;
	void Finalize() override;
	void Load() override;
	void OnCollision(const ColliderInfo* info) override;

	void SetPlayer(Player* player);

	void ShotBullet();

	//�r�w�C�r�A�c���[
	AI_BehaviorTree gunEnemyTree;
	//�擪
	std::shared_ptr<BehaviorBaseNode> rootNode;
	std::shared_ptr<BehaviorBaseNode> shotNode;
	std::shared_ptr<BehaviorBaseNode> waitNode;

	std::shared_ptr<Gun_WaitJudge>	gun_wjudgeNode;
	std::shared_ptr<Gun_WaitAct>	gun_wactNode;
	std::shared_ptr<Gun_ShotJudge>	gunsjudgeNode;
	std::shared_ptr<Gun_ShotAct>	gun_sactNode;

	Player* player;

	RVector3 pos;

	Bullet bullet;

	std::shared_ptr<Model3D> bModel;

	std::shared_ptr<Object3d> hpBarModel;

	NavMeshData nData;
	NavMeshAstar astar;

	bool isAlive = false;

	int Hitpoint;
};

