#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {

    
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    newObjectSystem = LoadModel_ObjFile("player");
    ship2 = LoadModel_ObjFile("player");
    ship3 = LoadModel_ObjFile("player");

    scale1 = { 10.0,10.0,10.0 };
    rot1 = { 0,0,0 };
    pos1 = { -50,0,0 };

    scale2 = { 10.0,10.0,10.0 };
    rot2 = { 0,0,0 };
    pos2 = { 0,0,0 };

    scale3 = { 10.0,10.0,10.0 };
    rot3 = { 0,0,0 };
    pos3 = { 50,0,0 };

    newObjectSystem->SetAffineParam(scale1, rot1, pos1);
    ship2->SetAffineParam(scale2, rot2, pos2);
    ship3->SetAffineParam(scale3, rot3, pos3);

    //画像の読み込み
    tiletex = TexManager::LoadTexture("Resources/grif.png");

    tileObject = NY_Object3DManager::Get()->CreateModel_Tile(500, 500, 50, 50, tiletex);
    tileObject->color = DirectX::XMFLOAT4(1, 1, 1, 0.5);
    tileObject->SetAffineParam(RVector3(1, 1, 1), RVector3(90, 0, 0), RVector3(0, 0, 0));

    //スプライトに画像を割り当てる（画像のハンドル、縦幅、横幅）
    testInstance.Create(tiletex);
    x1 = 100.0f;
    x2 = 150.0f;
    y1 = 100.0f;
    y2 = 150.0f;

    //パーティクル
    particle1 = ParticleManager::Create();
    bomProto = new BomParticle(RVector3(0, 0, 0));
    //プロトタイプをセット
    particle1->Prototype_Set(bomProto);
    //パーティクルのテクスチャ
    ptex = TexManager::LoadTexture("Resources/effect1.png");

}

//初期化
void Title::Initialize() {
    /// <summary>
    /// フィールド管理部初期化
    /// </summary>

    //StageMoveParticle::Get()->Init(&cam);
}

void Title::Finalize()
{
    //DeleteObject3d(obj1);
    //DeleteObject3d(obj2);
    //DeleteObject3d(gmodel);
    //DeleteObject3d(animTestModel);

    DeleteObject(newObjectSystem);
    DeleteObject(tileObject);
}

//更新
void Title::Update() {
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    NY_Object3DManager::Get()->UpdateAllObjects();

    if(Input::isKey(DIK_SPACE)){ particle1->Prototype_Add(3, RVector3(0, 10, 0)); }

    x2 -= 0.1f;

    //Object3dのテスト

    //速度計算


    //重力落下
    time++;
    pos1.y = CalcGravity(200, 0, 1, time, vel.y);
    

    if (Input::isKey(DIK_LEFT)) {
        vel.x += CalcAccelToFramePerVel(-3.0f);
        //等速直線運動
        vel2.x = -1.0f;
        //等加速度直線運動（押しっぱなしで速度上昇）
        //引数は1secの移動量、これをフレームレートで割って1Fの移動量を算出
        vel3.x += CalcAccelToFramePerVel(-3.0f);
    }
    if (Input::isKey(DIK_RIGHT)) {
        vel.x += CalcAccelToFramePerVel(3.0f);
        //等速直線運動
        vel2.x = 1.0f;
        //等加速度直線運動
        vel3.x += CalcAccelToFramePerVel(3.0f);
    }


    if (Input::isKeyTrigger(DIK_ESCAPE)) {
        pos1 = RVector3(-50, 200, 0);
        pos2 = RVector3(0, 0, 0);
        pos3 = RVector3(50, 0, 0);
        vel = RVector3(0, 0, 0);
        vel2 = RVector3(0, 0, 0);
        vel3 = RVector3(0, 0, 0);
        acc = RVector3(0, 0, 0);
        acc2 = RVector3(0, 0, 0);
        acc3 = RVector3(0, 0, 0);

        time = 0;
    }

    ////等速直線運動 コントローラー入力強さ * 速度定数
    ////vel2.x = Input::GetXpadLStickTilt().x_rate * 1.0f;
    ////vel2.z = Input::GetXpadLStickTilt().y_rate * 1.0f;
    ////等加速度運動  (1secでの移動量 * コントローラー入力強さ) / フレームレート
    //vel3.x += CalcAccelToFramePerVel(60.0f * Input::GetXpadLStickTilt().x_rate);

    //速度加算
    //pos1 += vel;
    pos2 += vel2;
    pos3 += vel3;

    //床にあたったら
    if (pos1.y < 0.0f) {
        vel.zero();
        pos1.y = 0.0f;
    }

    newObjectSystem->SetAffineParamTranslate(pos1);
    ship2->SetAffineParamTranslate(pos2);
    ship3->SetAffineParamTranslate(pos3);

    if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_L)) {
        particle1->Prototype_Add(5, pos1);
    }
    if (Input::isXpadButtonPushed(XPAD_BUTTON_OPTION_R)) {
        particle1->Prototype_Add(20, { 0,0,0 });
    }

    particle1->Prototype_Update();

    //if (mpRenderSp.spdata.color.w > 1.0f) {
    //    isadd = false;
    //    mpRenderSp.spdata.color.w = 1.0f;
    //}
    //else if(mpRenderSp.spdata.color.w < 0.0f){
    //    isadd = true;
    //    mpRenderSp.spdata.color.w = 0.0f;
    //}

    //if (isadd) {
    //    mpRenderSp.spdata.color.w += 0.001;
    //}
    //else {
    //    mpRenderSp.spdata.color.w -= 0.001;
    //}

    //mpRenderSp.UpdateSprite();

}

//描画
void Title::Draw() {

    //背景に常にいる
    Raki_DX12B::Get()->StartDrawRenderTarget();

    Raki_DX12B::Get()->StartDrawBackbuffer();

    //パーティクル描画
    particle1->Prototype_Draw(ptex);

    //描画1回目
    //testFbx->DrawObject();
    newObjectSystem->DrawObject();
    ship2->DrawObject();
    ship3->DrawObject();
    tileObject->DrawObject();

    SpriteManager::Get()->SetCommonBeginDraw();

    testInstance.DrawExtendSprite(x1, y1, x2, y2);
    testInstance.DrawExtendSprite(x1, y1 + 50, x2, y2 + 50);

    //本描画（実際に描画される）
    testInstance.Draw();

    ImguiMgr::Get()->StartDrawImgui("pos", 100, 300);

    ImGui::Text("Left Ship");
    ImGui::Text("pos : x.%f y.%f", pos1.x, pos1.y);
    ImGui::Text("vel : x.%f y.%f", vel.x, vel.y);
    ImGui::Text("acc : x.%f y.%f", acc.x, acc.y);
    ImGui::Text("Center Ship");
    ImGui::Text("pos : x.%f z.%f", pos2.x, pos2.z);
    ImGui::Text("vel : x.%f z.%f", vel2.x, vel2.z);
    ImGui::Text("acc : x.%f z.%f", acc2.x, acc2.z);
    ImGui::Text("Right Ship");
    ImGui::Text("pos : x.%f z.%f", pos3.x, pos3.z);
    ImGui::Text("vel : x.%f z.%f", vel3.x, vel3.z);
    ImGui::Text("acc : x.%f z.%f", acc3.x, acc3.z);

    ImguiMgr::Get()->EndDrawImgui();

    //描画終了
    Raki_DX12B::Get()->CloseDraw();

}
