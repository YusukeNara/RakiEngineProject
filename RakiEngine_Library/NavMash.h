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

	//�i�r���b�V�����_���
	std::array<RVector3, 3> navmashVertex;
	//�i�r���b�V���d�S�_
	RVector3				center;
	//�אڃ��b�V��
	std::vector<NavMesh*>	navmeshptr;

	//���ʔԍ�
	int navMeshNo = -1;

	//�q���[���X�e�B�b�N�R�X�g�i�G�[�W�F���g���v�Z����j
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