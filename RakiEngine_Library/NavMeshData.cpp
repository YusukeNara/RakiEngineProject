#include "NavMeshData.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void NavMeshData::LoadNavMesh(std::string file)
{
	//FileStream
	std::ifstream stageFile;

	//obj�t�@�C���I�[�v��
	stageFile.open(file);
	//�I�[�v�����s���`�F�b�N
	if (stageFile.fail() == true)
	{
		return;
	}
	
	std::vector<RVector3> vertexPos;
	std::vector<std::array<unsigned short, 3>> indexes;

	std::string line;

	while (std::getline(stageFile, line)) {

		//1�s���̕�������X�g���[���ɕϊ����ēǂ݂₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X�ōs�̐擪������擾
		string key;
		getline(line_stream, key, ' ');

		if (key == "v") {

			RVector3 loadedPos{};
			line_stream >> loadedPos.x;
			line_stream >> loadedPos.y;
			line_stream >> loadedPos.z;

			vertexPos.emplace_back(loadedPos);
		}

		if (key == "m") {
			string index_string;
			array<unsigned short, 3> index;
			while (getline(line_stream, index_string, ' ')) {
				istringstream index_stream(index_string);
				index_stream >> index[0];
				index_stream.seekg(1, ios_base::cur);
				index_stream >> index[1];
				index_stream.seekg(1, ios_base::cur);
				index_stream >> index[2];
			}

			indexes.push_back(index);
		}
	}

	//���o���������i�r���b�V���z��ɓ���

	//�C���f�b�N�X�z��̐� = �i�r���b�V���̑�������
	for (int i = 0; i < indexes.size(); i++) {
		NavMesh mesh;

		for (int j = 0; j < 3; j++) {
			//���b�V�����ێ�
			mesh.indices[j] = indexes[i][j];
			mesh.indexNumStr += (std::to_string(indexes[i][j]) + " ");
			mesh.navmashVertex[j] = vertexPos[indexes[i][j]];
		}
		mesh.center = CalcTriangleCenter(mesh.navmashVertex[0], mesh.navmashVertex[1], mesh.navmashVertex[2]);
		//�����ɑ}��
		navMeshData.push_back(mesh);
	}

	//�ۑ������i�r���b�V���̃C���f�b�N�X�����ƂɁA�e���b�V���ɗאڃ��b�V���̏�������
	for (int i = 0; i < navMeshData.size(); i++) {

		//���b�V���C���f�b�N�X3�̂����A2����v���郁�b�V����3������

		//�ꖇ��
		auto nm1 = std::find_if(navMeshData.begin(), navMeshData.end(), [&](
			NavMesh& rm) {return ChackIsNearMesh(&navMeshData[i], &rm); });
		if (nm1 != navMeshData.end()) { 
			navMeshData[i].navmeshptr.push_back(&(*nm1));
			nm1++;
		}

		//�񖇖�
		auto nm2 = std::find_if(nm1, navMeshData.end(), [&](
			NavMesh& rm) {return ChackIsNearMesh(&navMeshData[i], &rm); });
		if (nm2 != navMeshData.end()) { 
			navMeshData[i].navmeshptr.push_back(&(*nm2)); 
			nm2++;
		}

		//�O����
		auto nm3 = std::find_if(nm2, navMeshData.end(), [&](
			NavMesh& rm) {return ChackIsNearMesh(&navMeshData[i], &rm); });
		if (nm3 != navMeshData.end()) {
			navMeshData[i].navmeshptr.push_back(&(*nm3));
		}
	}

}

bool NavMeshData::ChackIsNearMesh(NavMesh *lm, NavMesh *rm)
{
	std::array<bool, 3> isMatchIndex = { false,false,false };

	for (int i = 0; i < 3; i++) {
		auto idx = std::find(rm->indices.begin(), rm->indices.end(),lm->indices[i]);
		if (idx != rm->indices.end()) { isMatchIndex.at(i) = true; }
	}

	//3�̂���2����v
	return std::count(isMatchIndex.begin(), isMatchIndex.end(), true) == 2;
}

void NavMeshData::InitNavMeshes()
{
	for (auto& m : navMeshData) {
		m.InitCost();
	}
}
