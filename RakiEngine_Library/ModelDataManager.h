#pragma once
#include "NY_Model.h"
#include "FbxLoader.h"

#include <string>
#include <vector>

class ModelDataManager
{
public:

	static int LoadOBJModel(const char *modelname);

	static int LoadFBXModel(const char *modelname);




private:
	//���f���f�[�^�R���e�i
	std::vector<Model3D> models;

	std::vector<fbxModel> fbxModels;




};