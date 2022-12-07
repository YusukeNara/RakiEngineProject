#pragma once
#include "RVector.h"

#include <list>
#include <array>
#include <vector>

class NavMesh
{
public:
	NavMesh(RVector3 v1, RVector3 v2, RVector3 v3);
	~NavMesh(){}

	//ナビメッシュ頂点情報
	std::array<RVector3, 3> navmashVertex;
	//ナビメッシュ重心点
	RVector3				center;
	//隣接メッシュ
	std::vector<NavMesh*>	navmeshptr;

	//識別番号
	int navMeshNo = -1;

	//ヒューリスティックコスト（エージェントが計算する）
	int heuristicCost = -1;

	bool operator > (const NavMesh& str) const
	{
		return (heuristicCost > str.heuristicCost);
	}
	inline bool operator==(const NavMesh& other) const
	{
		return &other == this;
	}
	auto operator<=>(const NavMesh &n) const = default;
	
};

//bool NavMesh::operator== (const NavMesh& m1, const NavMesh& m2) {
//	return m1.navMeshNo == m2.navMeshNo;
//}