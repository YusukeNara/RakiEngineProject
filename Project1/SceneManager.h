#pragma once
#include "ISceneChanger.h"
#include "BaseScene.h"
#include "Raki_DX12B.h"
#include "Raki_WinAPI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

/// <summary>
/// シーン管理クラス　仕様
/// <para>詳しくは dixq.net/g/sp_06.html を参照すること</para>
/// <para>・各シーンのクラスのインスタンスを生成し、現在のシーンの処理のみ実行する</para>
/// <para>・各シーンのクラスのオーバーライド関数は、引数にRaki_DX12Bクラスを取得する</para>
/// <para>・使用時は、このクラスのインスタンスをmainで生成、Init、Update、Draw、(必要に応じてFinalize)を実行すればOK</para>
/// <para>・シーン変更実行時は、各クラスの更新処理でChangeScene(引数：enumで定義したシーン番号)関数を実行することで、</para>
/// <para>　前シーンの終了処理を実行、次シーンの処理を開始する</para>
/// </summary>
class SceneManager : public ISceneChanger ,Task
{
private:
    //各シーンインスタンス
	BaseScene *titleScene;
    BaseScene *gameScene;

    BaseScene *nowScene;

	eScene mNextScene;    //次のシーン管理変数

public:
    //コンスタラクタ
    SceneManager();
    ~SceneManager() {
        delete titleScene;
        titleScene = nullptr;
        delete gameScene;
        gameScene = nullptr;
    }
    void Initialize() override;//初期化
    void Finalize() override;//終了処理
    void Update() override;//更新
    void Draw() override;//描画

    // 引数 nextScene にシーンを変更する
    void ChangeScene(eScene NextScene) override;

};

