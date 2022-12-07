#include "NavMeshAstar.h"

#include <algorithm>

void NavMeshAstar::SetNavMeshData(std::vector<NavMesh> navMeshes, std::vector<unsigned short> navIndices)
{
}

void NavMeshAstar::NavMeshSearchAstar(NavMesh* start, NavMesh* goal)
{
	//ステージ情報を初期化

	//a*方式でナビメッシュの経路探索を実行

	std::vector<NavMesh*> openMesh;
	std::vector<NavMesh*> closeMesh;

	//スタートメッシュをopenMeshに追加
	openMesh.push_back(start);

	//openmeshがなくなるまでループ
	while (!openMesh.empty()) {

		//openmeshから取得
		NavMesh* searcingMesh = (*openMesh.begin());

		//openから除外
		openMesh.erase(openMesh.begin());

		//ゴールなら終了
		if (searcingMesh == goal) {
			closeMesh.push_back(searcingMesh);
			break;
		}
		//そうでないなら、取得メッシュの隣接メッシュをオープンリストに
		else {
			for (auto& m : searcingMesh->navmeshptr) {
				int costtmp = -1;
				//このとき、ヒューリスティックコストがないなら計算する
				if (m->heuristicCost == -1) {
					costtmp = CaclHeuristicCost(goal, searcingMesh);
				}
				
				//追加判定を実行し、okならopenに追加
				if (CheckNavMeshAddOpenAdd(openMesh, closeMesh, searcingMesh, costtmp)) {
					openMesh.push_back(searcingMesh);
					//このとき、コストを代入する
					searcingMesh->heuristicCost = costtmp;
					//ソート
					std::sort(openMesh.begin(), openMesh.end(), [](const auto& lm, const auto& rm) {
						return lm->heuristicCost < rm->heuristicCost;
						});
				}
			}
		}
	}

	//経路が見つからなかった場合は終了

	//経路復元
	std::list<NavMesh*> result_route;

	NavMesh* resultSearcingMesh = goal;
	result_route.push_back(resultSearcingMesh);
	//スタート位置まで戻る
	while (1) {
		
		NavMesh* nextMesh = nullptr;

		if (resultSearcingMesh == start) {
			result_route.push_back(resultSearcingMesh);

			break;
		}
		else {
			//隣接メッシュで最もコストが少ないメッシュをリストに
			//ソートして先頭を取得
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
	//クローズにある場合、コスト最低値を更新できるなら削除し、追加許可
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
	//オープンにある場合、コストを更新できるなら削除、追加許可
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
	//どちらにもない場合はok
	return true;
}

int NavMeshAstar::CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh)
{
	//距離をコストに
	int result = int(distance(goalMesh->center, targetMesh->center));

	return result;
}
