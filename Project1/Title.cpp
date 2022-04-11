#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {

    
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    newObjectSystem = LoadModel_ObjFile("player");

    scale1 = { 5.0,5.0,5.0 };
    rot1 = { 0,0,0 };
    pos1 = { -50,0,0 };

    scale2 = { 10.0,10.0,10.0 };
    rot2 = { 0,90,0 };
    pos2 = { 50,0,0 };

    //画像の読み込み
    tiletex = TexManager::LoadTexture("Resources/grif.png");


    tileObject = NY_Object3DManager::Get()->CreateModel_Tile(100, 100, 10, 10, tiletex);
    tileObject->color = DirectX::XMFLOAT4(1, 1, 1, 0.5);
    tileObject->SetAffineParam(RVector3(1, 1, 1), RVector3(90, 0, 0), RVector3(0, 0, 0));

    //スプライトに画像を割り当てる（画像のハンドル、縦幅、横幅）
    testInstance.Create(tiletex, 32, 32);


    //NY_Object3DManager::Get()->SetCamera(&cam);
    //model1.LoadObjModel("player");
    //model2.CreateBoxModel(2.5, 1, 1, TexManager::LoadTexture("Resources/warning.png"));
    //grid.CreatePlaneModelXY(50, 50, 10, 10, TexManager::LoadTexture("Resources/grif.png"),nullptr);
    //UINT animtex = TexManager::LoadDivTextureTest(&testData, "Resources/animation_test.png", 5, 32);
    //animModel.CreatePlaneModelXY(10, 10, 1, 1, animtex, &testData);
    //obj1 = CreateObject3d(&model1, RVector3(0, 0, 0));
    //obj2 = CreateObject3d(&model2, RVector3(10, 0, 0));
    //gmodel = CreateObject3d(&grid, RVector3(0, 0, 10));
    //obj1->scale = scale;
    //animTestModel = CreateObject3d(&animModel, RVector3(-7.5f, 0, -2.0f));
    //animationTime = 0;

    particle1 = ParticleManager::Create();
    bomProto = new BomParticle(RVector3(0, 0, 0));
    //プロトタイプをセット
    particle1->Prototype_Set(bomProto);

    //パーティクルのテクスチャ
    ptex = TexManager::LoadTexture("Resources/effect1.png");
    //スポーン座標
    //p1state.spawnPos = RVector3(50, 40, 0);
    //p1state.isRandomSpawn = true;
    //p1state.spawnRange1 = RVector3(15, 15, 15);
    //p1state.spawnRange2 = RVector3(-15, -15, -15);
    ////速度
    //p1state.vel = RVector3(0, 0, 0);
    //p1state.isRandomVelocity = true;
    //p1state.velRange1 = RVector3(0.5, 0.3, 0.5);
    //p1state.velRange2 = RVector3(-0.5, 0.1, -0.5);
    ////加速度
    //p1state.acc = RVector3(0, -0.01, 0);
    //p1state.isRandomAccel = false;
    ////色
    //p1state.color = DirectX::XMFLOAT4(1, 1, 1, 1);
    //p1state.endColor = DirectX::XMFLOAT4(1, 1, 1, 0.1);
    //p1state.isRandomColor = false;
    ////スケーリング
    //p1state.scale = 5.0f;
    //p1state.endScale = 1.0f;
    //p1state.isRandomScale = false;
    ////回転
    //p1state.rot = 0.0f;
    //p1state.startRotate = 0.0f;
    //p1state.endRotate = 0.0f;
    ////寿命
    //p1state.activeTime = 90;

    //mpRenderSp.CreateSprite({ 1280,720 }, 0, false, nullptr);
    //mpRenderSp.spdata.position = { 0,0,0 };
    //mpRenderSp.UpdateSprite();

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

    delete particle1;
    delete bomProto;
}

//更新
void Title::Update() {
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    if (Input::isKey(DIK_W)) { eye.y += 0.3; }
    if (Input::isKey(DIK_S)) { eye.y -= 0.3; }
    if (Input::isKey(DIK_D)) { eye.x += 0.3; }
    if (Input::isKey(DIK_A)) { eye.x -= 0.3; }

    NY_Object3DManager::Get()->UpdateAllObjects();

    if(Input::isKey(DIK_SPACE)){ particle1->Prototype_Add(3, RVector3(0, 10, 0)); }
    

    //パーティクル生成
    //pSpawnDelay++;
    //if (pSpawnDelay > PSPAWN_DELAY_TIME) {
    //    pSpawnDelay = 5;
    //    for (int i = 0; i < pSpawnNum; i++) {
    //        //パーティクル生成
    //        particle1->Add(p1state);
    //    }
    //}
    particle1->Prototype_Add(1, RVector3(0, 0, 0));

    particle1->Prototype_Update();

    //animationTime++;
    //if (animationTime > ANIM_TIME) {
    //    animationTime = 0;
    //    testData.usingNo++;
    //    cursorAnim.usingNo++;
    //    if (testData.usingNo > 4) {
    //        testData.usingNo = 0;
    //        cursorAnim.usingNo = 0;
    //    }
    //}


    //Object3dのテスト
    pos1.x -= 0.1;
    newObjectSystem->SetAffineParam(scale1, rot1, pos1);


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

    //描画1回目
    newObjectSystem->DrawObject();
    tileObject->DrawObject();
    //パーティクル描画
    particle1->Prototype_Draw(ptex);

    SpriteManager::Get()->SetCommonBeginDraw();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            testInstance.DrawSprite(i * 32, j * 32);
        }
    }
    //本描画（実際に描画される）
    testInstance.Draw();

    //描画終了
    Raki_DX12B::Get()->CloseDraw();

}
