#pragma once
#include <GameObject.h>
#include <array>

#include "BuildingObject.h"

class StageObjects
{
public:
	StageObjects() {};
	~StageObjects() {};

	//ステージ情報読み込み
	void Init();
	
	void Update();

	void Draw();




private:

	std::array<std::array<int, 10>, 10> stageArray;

	std::vector<BuildingObject> buildObject;

};

