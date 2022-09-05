#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

#include <AI_BehaviorBaseNode.h>

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {
    ship2 = LoadModel_ObjFile("player");
    ship3 = LoadModel_ObjFile("Sphere");
    rtDrawer = NY_Object3DManager::Get()->CreateModel_Tile(32, 18, 1, 1, tiletex);

    scale1 = { 10.0,10.0,10.0 };
    rot1 = { 0,0,0 };
    pos1 = { -25,20,0 };
    scale2 = { 10.0,10.0,10.0 };
    rot2 = { 0,0,0 };
    pos2 = { 25,20,-10 };
    scale3 = { 10.0,10.0,10.0 };
    rot3 = { 0,0,0 };
    pos3 = { 0,20,0 };

    ship2->SetAffineParam(scale2, rot2, pos2);
    ship3->SetAffineParam(scale1, rot1, pos1);
    saru = LoadModel_ObjFile("saru");
    saru->SetAffineParam(scale3, rot3, pos3);

    //画像の読み込み
    tiletex = TexManager::LoadTexture("Resources/blackParticleTex.png");
    tileObject = NY_Object3DManager::Get()->CreateModel_Tile(500, 500, 20, 20, tiletex);
    tileObject->color = DirectX::XMFLOAT4(1, 1, 1, 1);
    tileObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));

    //スプライトに画像を割り当てる（画像のハンドル、縦幅、横幅）
    testInstance.Create(tiletex);
    x1 = 100.0f;
    x2 = 150.0f;
    y1 = 100.0f;
    y2 = 150.0f;

    //プレイヤー
    pl.Init();
    //敵
    enemy.Init(ship2);

    //マルチパス描画出力
    rtDrawer->SetAffineParam(scale, RVector3(0, 0, 0), RVector3(0, 0, 0));

    emanager.Init(&pl);

    diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);

    NowSceneState = title;
}

//初期化
void Title::Initialize() {

}

void Title::Finalize()
{
    delete ship2;
    delete ship3;
    delete saru;
    delete tileObject;

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


}

//描画
void Title::Draw() {

    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

    tileObject->DrawObject();
    pl.Draw();
    enemy.Draw();
    emanager.Draw();

    NY_Object3DManager::Get()->CloseDrawObject3D();

    diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer);

    SpriteManager::Get()->SetCommonBeginDraw();

    pl.UiDraw();

    emanager.DebugDraw();
}
