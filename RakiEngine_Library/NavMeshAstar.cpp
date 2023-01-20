#include "NavMeshAstar.h"

#include <algorithm>

void NavMeshAstar::SetNavMeshData(std::vector<NavMesh> &navMeshes)
{
	navMeshData = navMeshes;
}

void NavMeshAstar::NavMeshSearchAstar(NavMesh start, NavMesh goal, std::vector<NavMesh> &result)
{
	//ステージ情報を初期化(ヒューリスティックコストを)

	//a*方式でナビメッシュの経路探索を実行

	std::vector<NavMesh*> openMesh;
	std::vector<NavMesh*> closeMesh;

	//スタートメッシュをopenMeshに追加
	openMesh.push_back(&start);

	//openmeshがなくなるまでループ
	while (!openMesh.empty()) {

		//openmeshから取得
		auto searcingMesh = openMesh.begin();

		//ゴールなら終了
		if ((*searcingMesh)->indexNumStr == goal.indexNumStr) {
			closeMesh.push_back(*searcingMesh);
			break;
		}
		//そうでないなら、取得メッシュの隣接メッシュをオープンリストに
		else {
			for (auto& m : (*searcingMesh)->navmeshptr) {
				int costtmp = -1;
				//このとき、ヒューリスティックコストがないなら計算する
				if (m->heuristicCost == -1) {
					costtmp = CaclHeuristicCost(&goal, (*searcingMesh));
				}
				
				//追加判定を実行し、okならopenに追加
				if (CheckNavMeshAddOpenAdd(openMesh, closeMesh, (*searcingMesh), costtmp)) {
					openMesh.push_back((*searcingMesh));
					//このとき、コストを代入する
					(*searcingMesh)->heuristicCost = costtmp;
					//ソート
					std::sort(openMesh.begin(), openMesh.end(), [](const auto& lm, const auto& rm) {
						return lm->heuristicCost < rm->heuristicCost;
					});
				}
			}
		}

		//openから除外
		openMesh.erase(searcingMesh);
	}

	//経路が見つからなかった場合は終了

	//経路復元
	std::vector<NavMesh> result_route;

	NavMesh resultSearcingMesh = goal;
	result_route.push_back(resultSearcingMesh);
	//スタート位置まで戻る
	while (1) {
		
		NavMesh* nextMesh = nullptr;

		if (resultSearcingMesh.indexNumStr == start.indexNumStr) {
			result_route.push_back(resultSearcingMesh);

			break;
		}
		else {
			//隣接メッシュで最もコストが少ないメッシュをリストに
			//ソートして先頭を取得
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
	//座標からメッシュを検索
	NavMesh startMesh, goalMesh;
	NowStandingMesh(nowPos, startMesh);
	NowStandingMesh(targetPos, goalMesh);

	NavMeshSearchAstar(startMesh, goalMesh, result);
}

bool NavMeshAstar::CheckNavMeshAddOpenAdd(std::vector<NavMesh*> open, std::vector<NavMesh*> close, NavMesh* checkMesh, int cost)
{
	//クローズにある場合、コスト最低値を更新できるなら削除し、追加許可
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
		//オープンにある場合、コストを更新できるなら削除、追加許可
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

	//どちらにもない場合はok
	return true;
}

int NavMeshAstar::CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh)
{
	//距離をコストに
	int result = int(distance(goalMesh->center, targetMesh->center));

	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(float permissible, RVector3& nowpos, bool &isMoved)
{
	//誤差値使わない気がする
	//todo 使わなくてよくなったら消す

	RVector3 result(0.0f, 0.0f, 0.0f);

	//現在いるメッシュを検索
	NavMesh nowStand;
	NowStandingMesh(nowpos, nowStand);

	//ゴール地点のとき
	if (nowStand == *(rootData.begin())) {
		isMoved = true;
		return result;
	}

	auto itr = std::find(rootData.begin(), rootData.end(), nowStand);

	//ルート上に存在
	if (itr != rootData.end()) {
		//インデックス番号を取得
		int indexNum = int(std::distance(rootData.begin(), itr));
		//次のインデックスの方向を取得
		int nextIndexNum = indexNum - 1;
		//方向を計算
		result = rootData[nextIndexNum].center - nowpos;
		result = result.norm();
	}
	else {
		//そうでないなら末尾ノードへ
		//でも末尾へ向かうルートを再計算しないといけない気がする...
		RVector3 beginPos = (rootData.end() - 1)->center;
		result = beginPos - nowpos;
		result = result.norm();
	}


	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(std::vector<NavMesh>& root, RVector3& nowpos)
{
	RVector3 result(0.0f, 0.0f, 0.0f);

	//現在いるメッシュを検索
	NavMesh nowStand;
	NowStandingMesh(nowpos, nowStand);

	//ゴール地点のとき
	if (nowStand == *(root.begin())) {
		return result;
	}

	auto itr = std::find(root.begin(), root.end(), nowStand);

	//ルート上に存在
	if (itr != root.end()) {
		//インデックス番号を取得
		int indexNum = int(std::distance(root.begin(), itr));
		//次のインデックスの方向を取得
		int nextIndexNum = indexNum - 1;
		//方向を計算
		result = root[nextIndexNum].center - nowpos;
		result = result.norm();
	}
	else {
		//そうでないなら末尾ノードへ
		//でも末尾へ向かうルートを再計算しないといけない気がする...


	}


	return result;
}

void NavMeshAstar::NowStandingMesh(RVector3 targetPos, NavMesh& resultData)
{

	float dist = distance(targetPos, navMeshData[0].center);
	float comparition = 0;
	int targetIndex = 0;
	//一番近いナビメッシュを検索
	for (int i = 0; i < navMeshData.size();i++) {
		comparition = distance(targetPos, navMeshData[i].center);
		if (dist > comparition) {
			dist = comparition;
			targetIndex = i;
		}
	}

	resultData = navMeshData[targetIndex];
}
