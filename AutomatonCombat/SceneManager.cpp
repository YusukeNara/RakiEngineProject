#include "SceneManager.h"
#include "GameScene.h"
#include "Title.h"

//Raki_DX12B         *SceneManager::dx12b  = nullptr;
//NY_Object3DManager *SceneManager::objmgr = nullptr;
//SpriteManager      *SceneManager::spmgr  = nullptr;

SceneManager::SceneManager() :mNextScene(eScene_None) {

    //各シーンのインスタンス生成
    titleScene = (BaseScene *) new Title(this);

    nowScene = titleScene;
    Initialize();
}

void SceneManager::Initialize()
{
    //シーンの初期化
	nowScene->Initialize();
}

void SceneManager::Finalize()
{
    nowScene->Finalize();
}

void SceneManager::Update()
{
    if (mNextScene != eScene_None) {    //次のシーンがセットされていたら
        nowScene->Finalize();//現在のシーンの終了処理を実行
        
        switch (mNextScene) {       //シーンによって処理を分岐
        case eScene_Title:        //次の画面がメニューなら
            nowScene = titleScene;
            break;//以下略
        case eScene_Game:
            break;
        }
        mNextScene = eScene_None;    //次のシーン情報をクリア
        nowScene->Initialize();    //シーンを初期化
    }

    nowScene->Update(); //シーンの更新
}

void SceneManager::Draw()
{
    //現在シーンの描画処理を実行
    nowScene->Draw();
}

void SceneManager::ChangeScene(eScene NextScene)
{
    //次のシーン番号をセット
    mNextScene = NextScene;
}
