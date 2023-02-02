#pragma once
#include "NavMash.h"
#include "NavMeshData.h"

class NavMeshAstar
{
public:

	//�T������i�r���b�V���f�[�^��ݒ�
	void SetNavMeshData(std::vector<NavMesh> &navMeshes);

	void NavMeshSearchAstar(NavMesh *start, NavMesh *goal, std::vector<RVector3> &result);

	void NavMeshSearchAster(RVector3 nowPos, RVector3 targetPos, std::vector<RVector3>& result);

	bool CheckNavMeshAddOpenAdd(std::vector<NavMesh*> &open, std::vector<NavMesh*> &close, NavMesh* checkMesh, int cost);

	int CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh);

	//�^����ꂽ���[�g�ƈړ������������W�����ƂɎ��̕���������
	RVector3 MoveWaypointDirectLine(float permissible, RVector3& nowpos, bool &isMoved);
	RVector3 MoveWaypointDirectLine(std::vector<NavMesh> &root, RVector3& nowpos);
	RVector3 MoveWaypointDirectLine(std::vector<RVector3>& root, RVector3& nowpos, float permissible, int& index, bool& usMoved);
	//�X�v���C���Ȑ��ł̈ړ�


	//���W����߂����b�V����Ԃ�
	NavMesh* NowStandingMesh(RVector3 targetPos);

	//�i�r���b�V���f�[�^�ۑ���
	std::vector<NavMesh*> navMeshData;

	//�o�H���ۑ���
	std::vector<NavMesh*> rootData;

};

