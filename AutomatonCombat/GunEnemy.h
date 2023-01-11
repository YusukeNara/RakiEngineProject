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

	//ビヘイビアツリー
	AI_BehaviorTree gunEnemyTree;
	//先頭
	BehaviorBaseNode* rootNode;
	BehaviorBaseNode* shotNode;
	BehaviorBaseNode* waitNode;

	Gun_WaitJudge* gun_wjudgeNode;
	Gun_WaitAct* gun_wactNode;
	Gun_ShotJudge* gunsjudgeNode;
	Gun_ShotAct* gun_sactNode;

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

