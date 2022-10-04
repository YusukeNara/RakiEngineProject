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
    box = NY_Object3DManager::Get()->CreateModel_Box(20, 1, 1, tiletex);
    box->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(10, 20, 10));

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

    titleSprite.Create(TexManager::LoadTexture("Resources/TitleFont.png"));
    overSprite.Create(TexManager::LoadTexture("Resources/gameover.png"));
    
    NowSceneState = game;
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

    //ゲーム内制作モデルデータ
    box->DrawObject();
    tileObject->DrawObject();
    //ロードモデルデータ
    pl.Draw();
    emanager.Draw();


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

    pl.UiDraw();

    emanager.DebugDraw();
}
