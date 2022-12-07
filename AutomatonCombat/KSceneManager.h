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

	//コンストラクタ
	KSceneManager(int start) {
		Scene = start;
	}
	//シーン番号取得
	int GetNowScene() {
		return Scene;
	}
	//シーンチェンジ
	void ChangeScene(int sceneNum) {
		Scene = sceneNum;
	}
	//時間差シーンチェンジ
	void ChangeScene(int sceneNum, int waitTime) {
		timer++;
		if (timer > waitTime) {
			Scene = sceneNum;
			timer = 0;
		}
	}

};

