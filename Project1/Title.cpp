#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

#include <AI_BehaviorBaseNode.h>

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {

    titleSprite.Create(TexManager::LoadTexture("Resources/TitleFont.png"));

    

    ////プレイヤー

    diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);

    gobject.Init();

    pl.Init();

    emanager.Init(&pl);

    build[0].Load();

    for (int i = 0; i < build.size(); i++) {
        build[i].Init();
        if (i < 5) {
            build[i].SetBuildingPos(RVector3(100 * i + 250, 0, 0));
        }
        else {
            build[i].SetBuildingPos(RVector3(0, 0, -100 * i - 5 - 250));
        }

    }

    pl.Update();

    emanager.Update();
}

Title::~Title()
{

}

//初期化
void Title::Initialize() {

}

void Title::Finalize()
{

}

//更新
void Title::Update() {

    switch (NowSceneState)
    {
    case title:
        if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A)) { 
            pl.Reset();
            emanager.Reset();
            NowSceneState = game; 
        }
        break;
    case game:

        pl.Update();
        emanager.Update();
        if (pl.hitpoint <= 0) {
            NowSceneState = over;
        }
        break;
    case over:

        if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A)) { 
            pl.Reset();
            emanager.Reset();
            NowSceneState = game; 
        }
        else if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_B)) { 
            NowSceneState = title; 
        }
        break;
    case clear:
        break;
    default:
        break;
    }

    gobject.Update();
}

//描画
void Title::Draw() {

    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

    emanager.Draw();
    pl.Draw();

    //ゲーム内制作モデルデータ
    gobject.Draw();
    for (auto& b : build) {
        b.Draw();
    }

    //ロードモデルデータ

    NY_Object3DManager::Get()->CloseDrawObject3D();

    diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer);

    SpriteManager::Get()->SetCommonBeginDraw();

    if (NowSceneState == title) { 
        titleSprite.DrawExtendSprite(1280.0f / 4, 720.0f / 4, 1280.0f * 0.75, 720.0f * 0.75);
        titleSprite.Draw();
    }
    if (NowSceneState == over) {
        overSprite.DrawExtendSprite(1280.0f / 4, 720.0f * 0.4, 1280.0f * 0.75, 720.0f * 0.6);
        overSprite.Draw();
    }
    if (NowSceneState == game) {
        pl.UiDraw();
        ImguiMgr::Get()->StartDrawImgui("State", 100, 100);

        ImGui::Text("Wave : %d  Kill : %d  Next : %d",
            emanager.waveCount, emanager.killCount, emanager.waveKillCount - emanager.killCount);

        ImGui::Text("F1 : Edit AI");

        ImguiMgr::Get()->EndDrawImgui();
    }

    emanager.UIDraw();

    emanager.DebugDraw();

}
