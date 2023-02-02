#include "NavMeshAstar.h"

#include <algorithm>
#include <iostream>

void NavMeshAstar::SetNavMeshData(std::vector<NavMesh> &navMeshes)
{
	for (auto& nm : navMeshes) {
		navMeshData.push_back(&nm);
	}
}

void NavMeshAstar::NavMeshSearchAstar(NavMesh *start, NavMesh *goal, std::vector<RVector3> &result)
{
	//�X�e�[�W����������(�q���[���X�e�B�b�N�R�X�g���Čv�Z)
	for (int i = 0; i < navMeshData.size(); i++) {
		navMeshData[i]->InitCost();
	}

	//rootData�����
	rootData.clear();

	//a*�����Ńi�r���b�V���̌o�H�T�������s

	std::vector<NavMesh*> openMesh;
	std::vector<NavMesh*> closeMesh;
	NavMesh* last_update_mesh = nullptr;

	//�X�^�[�g���b�V����openMesh�ɒǉ�
	openMesh.push_back(start);

	//�X�^�[�g�ʒu�̃R�X�g�v�Z
	start->heuristicCost = CaclHeuristicCost(goal, start);
	start->totalCost = start->heuristicCost;


	//openmesh���Ȃ��Ȃ�܂Ń��[�v
	while (!openMesh.empty()) {

		//openmesh����擾
		auto searcingMesh = (*openMesh.begin());
		openMesh.erase(openMesh.begin());

		//�S�[���Ȃ�I��
		if (searcingMesh->indexNumStr == goal->indexNumStr) {
			closeMesh.push_back(searcingMesh);
			break;
		}
		//�����łȂ��Ȃ�A�擾���b�V���̗אڃ��b�V�����I�[�v�����X�g��
		for (auto &m : searcingMesh->navmeshptr) {
			int hctmp = -1, tctmp = 0;
			//���̂Ƃ��A�q���[���X�e�B�b�N�R�X�g���Ȃ��Ȃ�v�Z����
			if (m->heuristicCost < 0) {
				m->heuristicCost = CaclHeuristicCost(goal, m);
			}

			tctmp = m->heuristicCost + searcingMesh->totalCost;

			//�ǉ���������s���Aok�Ȃ�open�ɒǉ�
			if (CheckNavMeshAddOpenAdd(openMesh, closeMesh, m, tctmp)) {
				//�g�[�^���R�X�g�ɑ��
				m->totalCost = tctmp;
				openMesh.push_back(m);
				//�\�[�g
				std::sort(openMesh.begin(), openMesh.end(), [](const auto& lm, const auto& rm) {
					return lm->totalCost < rm->totalCost;
					});
				//�ŏI�X�V��ۑ�
				last_update_mesh = m;
			}
		}

		bool add_close = true;
		//�N���[�Y�ɓ������b�V�������邩�m�F
		for (auto itr = closeMesh.begin(); itr != closeMesh.end(); itr++) {
			if ((*itr) == searcingMesh) {
				add_close = false;
			}
		}
		if (add_close) {
			closeMesh.push_back(searcingMesh);
		}
	}

	//�o�H��������Ȃ������ꍇ�͏I��
	if (last_update_mesh == nullptr) {
		return;
	}
	//open����ŏI������ꍇ�͏I��
	if (openMesh.empty()) {
		return;
	}


	//�o�H����
	std::vector<NavMesh*> result_route;

	NavMesh *resultSearcingMesh = last_update_mesh;
	rootData.push_back(resultSearcingMesh);
	//�X�^�[�g�ʒu�܂Ŗ߂�
	while (1) {
		
		NavMesh* nextMesh = nullptr;

		if (resultSearcingMesh->indexNumStr == start->indexNumStr) {
			rootData.push_back(resultSearcingMesh);

			break;
		}
		else {
			//�אڃ��b�V���ɃS�[��������

			//�אڃ��b�V���ōł��R�X�g�����Ȃ����b�V�������X�g��
			 
			//�g�[�^�����v�Z����Ă��Ȃ��A���łɒʂ������b�V������Ȃ�
			std::vector<NavMesh*> contenderMesh;
			for (auto itr = resultSearcingMesh->navmeshptr.begin(); 
				itr != resultSearcingMesh->navmeshptr.end(); itr++) {
				if ((*itr)->totalCost > 0 && 
					std::find(rootData.begin(), rootData.end(),(*itr)) == rootData.end()) {
					contenderMesh.push_back((*itr));
				}
			}

			if (contenderMesh.empty()) {
				std::cout << "ERROR : NavMeshAstar : Route not found" << std::endl;
				return;
			}
			
			//�\�[�g���Đ擪���擾
			//std::sort(contenderMesh.begin(), contenderMesh.end(), [](const auto& lm, const auto& rm) {
			//	return lm->totalCost < rm->totalCost;
			//	});
			//std::sort(contenderMesh.begin(), contenderMesh.end());

			NavMesh* m = contenderMesh[0];
			for (auto& cm : contenderMesh) {
				//�ʉߍσ��b�V�����X�g�ɂȂ��āA�R�X�g���X�V����
				if (cm->totalCost < m->totalCost) {
					m = cm;
				}
			}

			//����σ��b�V��

			//auto minmesh = *std::max_element(contenderMesh.begin(), contenderMesh.end());

			rootData.push_back(m);
			resultSearcingMesh = m;
		}
	}

	//for (auto& nm : rootData) {
	//	result.push_back(*nm);
	//}

	for (auto& nm : rootData) {
		result.push_back(nm->center);
	}

	//std::copy(result_route.begin(), result_route.end(), rootData.begin());

	return;
}

void NavMeshAstar::NavMeshSearchAster(RVector3 nowPos, RVector3 targetPos, std::vector<RVector3>& result)
{
	//���W���烁�b�V��������
	NavMesh* startMesh = nullptr, * goalMesh = nullptr;
	startMesh = NowStandingMesh(nowPos);
	goalMesh = NowStandingMesh(targetPos);

	NavMeshSearchAstar(startMesh, goalMesh, result);
}

bool NavMeshAstar::CheckNavMeshAddOpenAdd(std::vector<NavMesh*> &open, std::vector<NavMesh*> &close, NavMesh* checkMesh, int cost)
{
	auto c_result = std::find_if(close.begin(), close.end(), [&](NavMesh* m) {return m->indexNumStr == checkMesh->indexNumStr; });
	auto o_result = std::find_if(open.begin(), open.end(), [&](NavMesh* m) {return m->indexNumStr == checkMesh->indexNumStr; });
	if (c_result == close.end() && o_result == open.end()) {
		return true;
	}
	else if (c_result != close.end()) {
		if ((*c_result)->totalCost > cost) {
			close.erase(c_result);
			return true;
		}
		else {
			return false;
		}
	}
	else if (o_result != open.end()) {
		if ((*o_result)->totalCost > cost) {
			open.erase(o_result);
			return true;
		}
		else {
			return false;
		}
	}

	//�����ɂ��邱�Ƃ͂��肦�Ȃ�
	return false;
}

int NavMeshAstar::CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh)
{
	//���m�[�h�g�[�^���R�X�g + �m�[�h�Ԉړ��R�X�g + �q���[���X�e�B�b�N�R�X�g = �g�[�^���R�X�g
	int result = int(distance(goalMesh->center, targetMesh->center));

	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(float permissible, RVector3& nowpos, bool &isMoved)
{
	if (rootData.empty()) {
		std::cout << "ERROR : NavMeshAstar : Route not found" << std::endl;
		return RVector3(0, 0, 0);
	}

	//�덷�l�g��Ȃ��C������
	//todo �g��Ȃ��Ă悭�Ȃ��������

	RVector3 result(0.0f, 0.0f, 0.0f);

	//���݂��郁�b�V��������
	NavMesh *nowStand;
	nowStand = NowStandingMesh(nowpos);

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
		result = rootData[nextIndexNum]->center - nowpos;
		result = result.norm();
	}
	else {
		//�����łȂ��Ȃ疖���m�[�h��
		//�ł������֌��������[�g���Čv�Z���Ȃ��Ƃ����Ȃ��C������...
		RVector3 beginPos = (*rootData.end() - 1)->center;
		result = beginPos - nowpos;
		result = result.norm();
	}


	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(std::vector<NavMesh>& root, RVector3& nowpos)
{
	RVector3 result(0.0f, 0.0f, 0.0f);

	//���݂��郁�b�V��������
	NavMesh *nowStand;
	nowStand = NowStandingMesh(nowpos);

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

RVector3 NavMeshAstar::MoveWaypointDirectLine(std::vector<RVector3>& root, RVector3& nowpos, float permissible, int& index, bool& usMoved)
{
	RVector3 result(0.0f, 0.0f, 0.0f);

	//�͈͊O
	if (index < 0 || index > root.size()) { return result; }

	//�w�������Ă���C���f�b�N�X�����ƂɃx�N�g�������߂�
	result = root[index] - nowpos;
	result.norm();

	//�������݈ʒu�ƖړI�n�̋������덷�͈̔͂Ȃ�
	if (distance(root[index], nowpos) < permissible) {
		if (index == root.size()) {
			usMoved = true;
		}
		else {
			index++;
		}
	}

	return result;
}

NavMesh* NavMeshAstar::NowStandingMesh(RVector3 targetPos)
{

	float dist = distance(targetPos, navMeshData[0]->center);
	float comparition = 0;
	int targetIndex = 0;
	//��ԋ߂��i�r���b�V��������
	for (int i = 0; i < navMeshData.size();i++) {
		comparition = distance(targetPos, navMeshData[i]->center);
		if (dist > comparition) {
			dist = comparition;
			targetIndex = i;
		}
	}

	return navMeshData[targetIndex];
}
