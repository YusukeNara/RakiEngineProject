#pragma once

enum SceneNum {
	Title,
	Game,
};

class KSceneManager
{
private:

	int Scene;
	int timer = 0;

public:

	//�R���X�g���N�^
	KSceneManager(int start) {
		Scene = start;
	}
	//�V�[���ԍ��擾
	int GetNowScene() {
		return Scene;
	}
	//�V�[���`�F���W
	void ChangeScene(int sceneNum) {
		Scene = sceneNum;
	}
	//���ԍ��V�[���`�F���W
	void ChangeScene(int sceneNum, int waitTime) {
		timer++;
		if (timer > waitTime) {
			Scene = sceneNum;
			timer = 0;
		}
	}

};

