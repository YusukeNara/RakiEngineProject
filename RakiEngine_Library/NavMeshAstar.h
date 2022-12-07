#pragma once
#include "NavMash.h"

class NavMeshAstar
{
public:

	void SetNavMeshData(std::vector<NavMesh> navMeshes, std::vector<unsigned short> navIndices);

	void NavMeshSearchAstar(NavMesh* start, NavMesh* goal);

	bool CheckNavMeshAddOpenAdd(std::vector<NavMesh*>& open, std::vector<NavMesh*>& close, NavMesh* checkMesh, int cost);

	int CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh);



};

