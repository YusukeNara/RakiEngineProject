#pragma once
#include <GameObject.h>
#include <array>

#include "BuildingObject.h"

class StageObjects
{
public:
	StageObjects() {};
	~StageObjects() {};

	//�X�e�[�W���ǂݍ���
	void Init();
	
	void Update();

	void Draw();




private:

	std::array<std::array<int, 10>, 10> stageArray;

	std::vector<BuildingObject> buildObject;

};

