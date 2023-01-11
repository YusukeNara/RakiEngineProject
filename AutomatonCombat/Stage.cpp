#include "Stage.h"
#include <iostream>
#include <algorithm>

void StageObjects::Init()
{
	FILE* fileHandle;
	errno_t err;
	char string[256];
	err = fopen_s(&fileHandle, "Resources/stage.csv", "r");
	if (err != 0)
	{
		return;
	}

	for (int y = 0; y < 10; y++)
	{
		bool end = false;
		fgets(string, 256, fileHandle);
		for (int x = 0, j = 0; x < 10; j++)
		{
			if (string[j] == '\0' || string[j] == '\n')
			{
				// 改行
				break;
			}
			if (ispunct(string[j]))
			{
				// 次の数字へ
				x++;
			}
			else
			{
				stageArray[y][x] *= 10;
				stageArray[y][x] += string[j] - '0';

				if (stageArray[y][x] == -1)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	int buildCount = 0;

	for (int y = 0; y < 10; y++) {
		buildCount += int(std::count(stageArray[y].begin(), stageArray[y].end(), 1));
	}

	buildObject.reserve(buildCount);

	for (int i = 0; i < buildCount; i++) {
		buildObject.emplace_back();
		buildObject.back().Init();
	}

	int stagePutNum = 0;

	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			if (stageArray[y][x] == 1) {
				buildObject[stagePutNum].SetBuildingPos(RVector3(float(100 * (x - 5)), 0.0f, float(100 * (y - 5))));
				stagePutNum++;
			}
		}
	}


}

void StageObjects::Update()
{
}

void StageObjects::Draw()
{
	for (int i = 0; i < buildObject.size(); i++) {
		buildObject[i].Draw();
	}
}
