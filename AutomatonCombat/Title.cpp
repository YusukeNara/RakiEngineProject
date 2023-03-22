#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"
#include "WaveDirections.h"

#include <AI_BehaviorBaseNode.h>

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {

    titleSprite.Create(TexManager::LoadTexture("Resources/TitleFont.png"));

    overSprite.Create(TexManager::LoadTexture("Resources/gameover.png"));

    clearSprite.Create(TexManager::LoadTexture("Resources/ClearSprite.png"));

    ////プレイヤー
    pl      = std::make_shared<Player>();
    astar   = std::make_shared<NavMeshAstar>();

    nData.LoadNavMesh("Resources/NavMeshTestData.txt");
    astar->SetNavMeshData(nData.navMeshData);

    diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);

    gobject.Init();

    pl->Init();

    gobject.SetPlayer(pl.get());

    emanager.Init(pl, astar);

    build[0].Load();

    for (int i = 0; i < 5; i++) {
        sky[i].Init();
    }
    sky[0].SetAffinParam(RVector3(90.0f,0.0f,180.0f), RVector3(0,500,500));
    sky[1].SetAffinParam(RVector3(90.f,0.f,0.f), RVector3(0,500,-500));
    sky[2].SetAffinParam(RVector3(0.f,0.f,90.f), RVector3(500,500,0));
    sky[3].SetAffinParam(RVector3(0.f,0.0f,-90.f), RVector3(-500,500,0));
    sky[4].SetAffinParam(RVector3(180.f, 0.f, 0.f), RVector3(0, 500, 0));

    for (int i = 0; i < build.size(); i++) {
        build[i].Init();
        if (i < 5) {
            build[i].SetBuildingPos(RVector3(-400.0f + 100.0f * float(i), 0.0f, 400.0f));
        }
        else {
            build[i].SetBuildingPos(RVector3(400.0f, 0.0f, 400.0f - 100.0f * float(i)));
        }

    }

    pl->Update();

    emanager.Update();

    stage.Init();

    WaveDirections::Get()->Init();

    t_frame = 0;
    std::vector<NavMesh> result;
    //astar.NavMeshSearchAstar(&nData.navMeshData[0], &nData.navMeshData[3], result);
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
        t_frame = 0;
        if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A)) { 

            pl->Reset();
            emanager.Reset();
            NowSceneState = game; 
            WaveDirections::Get()->PlayNextWaveDir();
        }
        break;
    case game:

        if (t_frame < 60) {
            t_frame++;
        }

        gobject.Update();

        pl->Update();
        emanager.Update();
        if (pl->hitpoint <= 0) {
            NowSceneState = over;
        }
        if (emanager.gameCleared) {
            NowSceneState = clear;
        }

        break;
    case over:

        if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A)) { 
            pl->Reset();
            emanager.Reset();
            NowSceneState = game; 
            t_frame = 0;
        }
        else if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_B)) { 
            NowSceneState = title; 
            t_frame = 0;
        }
        break;
    case clear:
        //クリア演出と入力処理
        if (Input::isXpadButtonPushed(XPAD_BUTTON_B)) {
            NowSceneState = title;
            t_frame = 0;
        }

        break;
    default:
        break;
    }

    gobject.Update();

    WaveDirections::Get()->Update();
}

//描画
void Title::Draw() {

    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

    for (int i = 0; i < 5; i++) {
        sky[i].Draw();
    }

    emanager.Draw();
    pl->Draw();


    //ゲーム内制作モデルデータ
    gobject.Draw();

    if(isBuildDraw){ stage.Draw(); }


    //ロードモデルデータ

    NY_Object3DManager::Get()->CloseDrawObject3D();

    diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer, &NY_Object3DManager::Get()->m_shadomMap);

    pl->ParticleDraw();

    emanager.ParticleDraw();

    gobject.EffectDraw();

    SpriteManager::Get()->SetCommonBeginDraw();

    float rate = (float)t_frame / 60;
    t_pos = Rv3Ease::InQuad(t_center,t_up,rate);

    if (NowSceneState == title) { 
        titleSprite.DrawExtendSprite(t_pos.x - 300.f, t_pos.y - 100.f, t_pos.x + 300.f, t_pos.y + 100.f);
        titleSprite.Draw();
    }
    if (NowSceneState == over) {
        overSprite.DrawExtendSprite(1280.0f / 4, 720.0f * 0.4, 1280.0f * 0.75, 720.0f * 0.6);
        overSprite.Draw();
    }
    if (NowSceneState == game) {
        if(t_frame < 60){
            titleSprite.DrawExtendSprite(t_pos.x - 300.f, t_pos.y - 100.f, t_pos.x + 300.f, t_pos.y + 100.f);
            titleSprite.Draw();
        }
        pl->UiDraw();
        ImguiMgr::Get()->StartDrawImgui("State", 100, 100);

        ImGui::Text("Wave : %d  Kill : %d  Next : %d",
            emanager.waveCount, emanager.killCount, emanager.waveKillCount - emanager.killCount);

        ImGui::Text("F1 : Edit AI");

        ImguiMgr::Get()->EndDrawImgui();
    }
    if (NowSceneState == clear) {
        clearSprite.DrawExtendSprite(1280.0f / 4, 720.0f / 2, 1280.0f * 0.75, 720.0f * 0.8);
        clearSprite.Draw();
    }

    ImguiMgr::Get()->StartDrawImgui("Build", 100, 100);

    ImGui::Checkbox("isDraw", &isBuildDraw);

    ImguiMgr::Get()->EndDrawImgui();

    emanager.UIDraw();

    WaveDirections::Get()->Draw();

    emanager.DebugDraw();

    pl->DebugDraw();

}
