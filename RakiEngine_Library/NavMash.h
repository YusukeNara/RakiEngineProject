#pragma once
#include "RVector.h"

#include <string>
#include <list>
#include <array>
#include <vector>
#include <compare>

class NavMesh
{
public:
	NavMesh(RVector3 v1, RVector3 v2, RVector3 v3);
	NavMesh() {}
	~NavMesh(){}


	//�i�r���b�V�����_���
	std::array<RVector3, 3> navmashVertex;
	//�i�r���b�V���d�S�_
	RVector3				center;
	//�C���f�b�N�X�ԍ�
	std::array<unsigned short, 3> indices;
	std::string indexNumStr;

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
	bool operator==(const NavMesh& other) const
	{
		return other.indexNumStr == this->indexNumStr;
	}
	bool operator==(const NavMesh* om) const{
		return om->indexNumStr == this->indexNumStr;
	}
	const bool operator==(NavMesh& other) { return this->indexNumStr == other.indexNumStr; }

	auto operator<=>(const NavMesh &n) const = default;
	
};

//bool NavMesh::operator== (const NavMesh& m1, const NavMesh& m2) {
//	return m1.navMeshNo == m2.navMeshNo;
//}