#include "NavMeshAstar.h"

#include <algorithm>

void NavMeshAstar::SetNavMeshData(std::vector<NavMesh> &navMeshes)
{
	navMeshData = navMeshes;
}

void NavMeshAstar::NavMeshSearchAstar(NavMesh start, NavMesh goal, std::vector<NavMesh> &result)
{
	//�X�e�[�W����������(�q���[���X�e�B�b�N�R�X�g��)

	//a*�����Ńi�r���b�V���̌o�H�T�������s

	std::vector<NavMesh*> openMesh;
	std::vector<NavMesh*> closeMesh;

	//�X�^�[�g���b�V����openMesh�ɒǉ�
	openMesh.push_back(&start);

	//openmesh���Ȃ��Ȃ�܂Ń��[�v
	while (!openMesh.empty()) {

		//openmesh����擾
		auto searcingMesh = openMesh.begin();

		//�S�[���Ȃ�I��
		if ((*searcingMesh)->indexNumStr == goal.indexNumStr) {
			closeMesh.push_back(*searcingMesh);
			break;
		}
		//�����łȂ��Ȃ�A�擾���b�V���̗אڃ��b�V�����I�[�v�����X�g��
		else {
			for (auto& m : (*searcingMesh)->navmeshptr) {
				int costtmp = -1;
				//���̂Ƃ��A�q���[���X�e�B�b�N�R�X�g���Ȃ��Ȃ�v�Z����
				if (m->heuristicCost == -1) {
					costtmp = CaclHeuristicCost(&goal, (*searcingMesh));
				}
				
				//�ǉ���������s���Aok�Ȃ�open�ɒǉ�
				if (CheckNavMeshAddOpenAdd(openMesh, closeMesh, (*searcingMesh), costtmp)) {
					openMesh.push_back((*searcingMesh));
					//���̂Ƃ��A�R�X�g��������
					(*searcingMesh)->heuristicCost = costtmp;
					//�\�[�g
					std::sort(openMesh.begin(), openMesh.end(), [](const auto& lm, const auto& rm) {
						return lm->heuristicCost < rm->heuristicCost;
					});
				}
			}
		}

		//open���珜�O
		openMesh.erase(searcingMesh);
	}

	//�o�H��������Ȃ������ꍇ�͏I��

	//�o�H����
	std::vector<NavMesh> result_route;

	NavMesh resultSearcingMesh = goal;
	result_route.push_back(resultSearcingMesh);
	//�X�^�[�g�ʒu�܂Ŗ߂�
	while (1) {
		
		NavMesh* nextMesh = nullptr;

		if (resultSearcingMesh.indexNumStr == start.indexNumStr) {
			result_route.push_back(resultSearcingMesh);

			break;
		}
		else {
			//�אڃ��b�V���ōł��R�X�g�����Ȃ����b�V�������X�g��
			//�\�[�g���Đ擪���擾
			std::sort(resultSearcingMesh.navmeshptr.begin(), resultSearcingMesh.navmeshptr.end(), [](const auto& lm, const auto& rm) {
				return lm->heuristicCost < rm->heuristicCost;
				});
			result_route.push_back((*resultSearcingMesh.navmeshptr[0]));
			NavMesh tmp = (*resultSearcingMesh.navmeshptr[0]);
			resultSearcingMesh = tmp;

		}
	}

	

	rootData = result_route;

	result = result_route;
}

void NavMeshAstar::NavMeshSearchAster(RVector3 nowPos, RVector3 targetPos, std::vector<NavMesh>& result)
{
	//���W���烁�b�V��������
	NavMesh startMesh, goalMesh;
	NowStandingMesh(nowPos, startMesh);
	NowStandingMesh(targetPos, goalMesh);

	NavMeshSearchAstar(startMesh, goalMesh, result);
}

bool NavMeshAstar::CheckNavMeshAddOpenAdd(std::vector<NavMesh*> open, std::vector<NavMesh*> close, NavMesh* checkMesh, int cost)
{
	//�N���[�Y�ɂ���ꍇ�A�R�X�g�Œ�l���X�V�ł���Ȃ�폜���A�ǉ�����
	if (close.size() > 0) {
		auto c_result = std::find_if(close.begin(), close.end(), [checkMesh](NavMesh *m) {return m->indexNumStr == checkMesh->indexNumStr; });
		if (c_result != close.end()) {
			if ((*c_result)->heuristicCost > cost) {
				close.erase(c_result);
				return true;
			}
			else {
				return false;
			}
		}
	}

	if (open.size() > 0) {
		//�I�[�v���ɂ���ꍇ�A�R�X�g���X�V�ł���Ȃ�폜�A�ǉ�����
		auto o_result = std::find_if(open.begin(), open.end(), [checkMesh](NavMesh *m) {return m->indexNumStr == checkMesh->indexNumStr; });
		if (o_result != open.end()) {
			if ((*o_result)->heuristicCost > cost) {
				open.erase(o_result);
				return true;
			}
			else {
				return false;
			}
		}
	}

	//�ǂ���ɂ��Ȃ��ꍇ��ok
	return true;
}

int NavMeshAstar::CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh)
{
	//�������R�X�g��
	int result = int(distance(goalMesh->center, targetMesh->center));

	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(float permissible, RVector3& nowpos, bool &isMoved)
{
	//�덷�l�g��Ȃ��C������
	//todo �g��Ȃ��Ă悭�Ȃ��������

	RVector3 result(0.0f, 0.0f, 0.0f);

	//���݂��郁�b�V��������
	NavMesh nowStand;
	NowStandingMesh(nowpos, nowStand);

	//�S�[���n�_�̂Ƃ�
	if (nowStand == *(rootData.begin())) {
		isMoved = true;
		return result;
	}

	auto itr = std::find(rootData.begin(), rootData.end(), nowStand);

	//���[�g��ɑ���
	if (itr != rootData.end()) {
		//�C���f�b�N�X�ԍ����擾
		int indexNum = int(std::distance(rootData.begin(), itr));
		//���̃C���f�b�N�X�̕������擾
		int nextIndexNum = indexNum - 1;
		//�������v�Z
		result = rootData[nextIndexNum].center - nowpos;
		result = result.norm();
	}
	else {
		//�����łȂ��Ȃ疖���m�[�h��
		//�ł������֌��������[�g���Čv�Z���Ȃ��Ƃ����Ȃ��C������...
		RVector3 beginPos = (rootData.end() - 1)->center;
		result = beginPos - nowpos;
		result = result.norm();
	}


	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(std::vector<NavMesh>& root, RVector3& nowpos)
{
	RVector3 result(0.0f, 0.0f, 0.0f);

	//���݂��郁�b�V��������
	NavMesh nowStand;
	NowStandingMesh(nowpos, nowStand);

	//�S�[���n�_�̂Ƃ�
	if (nowStand == *(root.begin())) {
		return result;
	}

	auto itr = std::find(root.begin(), root.end(), nowStand);

	//���[�g��ɑ���
	if (itr != root.end()) {
		//�C���f�b�N�X�ԍ����擾
		int indexNum = int(std::distance(root.begin(), itr));
		//���̃C���f�b�N�X�̕������擾
		int nextIndexNum = indexNum - 1;
		//�������v�Z
		result = root[nextIndexNum].center - nowpos;
		result = result.norm();
	}
	else {
		//�����łȂ��Ȃ疖���m�[�h��
		//�ł������֌��������[�g���Čv�Z���Ȃ��Ƃ����Ȃ��C������...


	}


	return result;
}

void NavMeshAstar::NowStandingMesh(RVector3 targetPos, NavMesh& resultData)
{

	float dist = distance(targetPos, navMeshData[0].center);
	float comparition = 0;
	int targetIndex = 0;
	//��ԋ߂��i�r���b�V��������
	for (int i = 0; i < navMeshData.size();i++) {
		comparition = distance(targetPos, navMeshData[i].center);
		if (dist > comparition) {
			dist = comparition;
			targetIndex = i;
		}
	}

	resultData = navMeshData[targetIndex];
}
