#include "NavMeshAstar.h"

#include <algorithm>

void NavMeshAstar::SetNavMeshData(std::vector<NavMesh> navMeshes, std::vector<unsigned short> navIndices)
{
}

void NavMeshAstar::NavMeshSearchAstar(NavMesh* start, NavMesh* goal)
{
	//�X�e�[�W����������

	//a*�����Ńi�r���b�V���̌o�H�T�������s

	std::vector<NavMesh*> openMesh;
	std::vector<NavMesh*> closeMesh;

	//�X�^�[�g���b�V����openMesh�ɒǉ�
	openMesh.push_back(start);

	//openmesh���Ȃ��Ȃ�܂Ń��[�v
	while (!openMesh.empty()) {

		//openmesh����擾
		NavMesh* searcingMesh = (*openMesh.begin());

		//open���珜�O
		openMesh.erase(openMesh.begin());

		//�S�[���Ȃ�I��
		if (searcingMesh == goal) {
			closeMesh.push_back(searcingMesh);
			break;
		}
		//�����łȂ��Ȃ�A�擾���b�V���̗אڃ��b�V�����I�[�v�����X�g��
		else {
			for (auto& m : searcingMesh->navmeshptr) {
				int costtmp = -1;
				//���̂Ƃ��A�q���[���X�e�B�b�N�R�X�g���Ȃ��Ȃ�v�Z����
				if (m->heuristicCost == -1) {
					costtmp = CaclHeuristicCost(goal, searcingMesh);
				}
				
				//�ǉ���������s���Aok�Ȃ�open�ɒǉ�
				if (CheckNavMeshAddOpenAdd(openMesh, closeMesh, searcingMesh, costtmp)) {
					openMesh.push_back(searcingMesh);
					//���̂Ƃ��A�R�X�g��������
					searcingMesh->heuristicCost = costtmp;
					//�\�[�g
					std::sort(openMesh.begin(), openMesh.end(), [](const auto& lm, const auto& rm) {
						return lm->heuristicCost < rm->heuristicCost;
						});
				}
			}
		}
	}

	//�o�H��������Ȃ������ꍇ�͏I��

	//�o�H����
	std::list<NavMesh*> result_route;

	NavMesh* resultSearcingMesh = goal;
	result_route.push_back(resultSearcingMesh);
	//�X�^�[�g�ʒu�܂Ŗ߂�
	while (1) {
		
		NavMesh* nextMesh = nullptr;

		if (resultSearcingMesh == start) {
			result_route.push_back(resultSearcingMesh);

			break;
		}
		else {
			//�אڃ��b�V���ōł��R�X�g�����Ȃ����b�V�������X�g��
			//�\�[�g���Đ擪���擾
			std::sort(resultSearcingMesh->navmeshptr.begin(), resultSearcingMesh->navmeshptr.end(), [](const auto& lm, const auto& rm) {
				return lm->heuristicCost < rm->heuristicCost;
				});
			result_route.push_back(resultSearcingMesh->navmeshptr[0]);
			NavMesh* tmp = resultSearcingMesh->navmeshptr[0];
			resultSearcingMesh = tmp;

		}
	}

}

bool NavMeshAstar::CheckNavMeshAddOpenAdd(std::vector<NavMesh*>& open, std::vector<NavMesh*>& close, NavMesh* checkMesh, int cost)
{
	//�N���[�Y�ɂ���ꍇ�A�R�X�g�Œ�l���X�V�ł���Ȃ�폜���A�ǉ�����
	//auto c_result = std::find(close.begin(), close.end(), checkMesh);
	//if (c_result != close.end()) {
	//	if ((*c_result)->heuristicCost > cost) {
	//		std::erase(close, c_result);
	//		return true;
	//	}
	//	else {
	//		return false;
	//	}
	//}
	//�I�[�v���ɂ���ꍇ�A�R�X�g���X�V�ł���Ȃ�폜�A�ǉ�����
	//auto o_result = std::find(open.begin(), open.end(), checkMesh);
	//if (o_result != open.end()) {
	//	if ((*o_result)->heuristicCost > cost) {
	//		std::erase(close, o_result);
	//		return true;
	//	}
	//	else {
	//		return false;
	//	}
	//}
	//�ǂ���ɂ��Ȃ��ꍇ��ok
	return true;
}

int NavMeshAstar::CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh)
{
	//�������R�X�g��
	int result = int(distance(goalMesh->center, targetMesh->center));

	return result;
}
