#pragma once
#include "NavMash.h"
#include "NavMeshData.h"

class NavMeshAstar
{
public:

	//探索するナビメッシュデータを設定
	void SetNavMeshData(std::vector<NavMesh> &navMeshes);

	void NavMeshSearchAstar(NavMesh start, NavMesh goal, std::vector<NavMesh> &result);

	void NavMeshSearchAster(RVector3 nowPos, RVector3 targetPos, std::vector<NavMesh>& result);

	bool CheckNavMeshAddOpenAdd(std::vector<NavMesh*> open, std::vector<NavMesh*> close, NavMesh* checkMesh, int cost);

	int CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh);

	//与えられたルートと移動させたい座標をもとに次の方向を示す
	RVector3 MoveWaypointDirectLine(float permissible, RVector3& nowpos, bool &isMoved);
	RVector3 MoveWaypointDirectLine(std::vector<NavMesh> &root, RVector3& nowpos);
	//スプライン曲線での移動


	//座標から近いメッシュを返す
	void NowStandingMesh(RVector3 targetPos, NavMesh& resultData);

	//ナビメッシュデータ保存先
	std::vector<NavMesh> navMeshData;

	std::vector<NavMesh> rootData;

};

