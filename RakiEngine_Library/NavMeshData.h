#pragma once
#include <string>
#include <vector>

#include "NavMash.h"

class NavMeshData
{
public:
	void LoadNavMesh(std::string file);

	bool ChackIsNearMesh(NavMesh *lm, NavMesh *rm);

public:
	//�i�r���b�V���R���e�i
	std::vector<NavMesh> navMeshData;


};