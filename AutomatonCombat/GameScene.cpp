#include "GameScene.h"

GameScene::GameScene(ISceneChanger *changer) : BaseScene(changer) {


}

//������
void GameScene::Initialize() {

}

void GameScene::Finalize()
{
}

//�X�V
void GameScene::Update() {



}

//�`��
void GameScene::Draw() {

	Raki_DX12B::Get()->StartDraw();

	Raki_DX12B::Get()->EndDraw();
}