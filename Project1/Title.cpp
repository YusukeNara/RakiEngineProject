#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

#include <AI_BehaviorBaseNode.h>

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {
    
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    newObjectSystem = LoadModel_ObjFile("player");
    ship2 = LoadModel_ObjFile("player");
    ship3 = LoadModel_ObjFile("player");
    rtDrawer = NY_Object3DManager::Get()->CreateModel_Tile(32, 18, 1, 1, tiletex);

    scale1 = { 15.0,15.0,15.0 };
    rot1 = { 0,0,0 };
    pos1 = { -100,5,0 };

    scale2 = { 30.0,30.0,30.0 };
    rot2 = { 0,0,0 };
    pos2 = { 0,5,100 };

    scale3 = { 30.0,30.0,30.0 };
    rot3 = { 0,0,0 };
    pos3 = { 100,5,0 };

    newObjectSystem->SetAffineParam(scale1, rot1, pos1);
    ship2->SetAffineParam(scale2, rot2, pos2);
    ship3->SetAffineParam(scale3, rot3, pos3);

    //画像の読み込み
    tiletex = TexManager::LoadTexture("Resources/white.png");
    tileObject = NY_Object3DManager::Get()->CreateModel_Tile(500, 500, 50, 50, tiletex);
    tileObject->color = DirectX::XMFLOAT4(1, 1, 1, 1);
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

    //プレイヤー
    pl.Init();
    //敵
    enemy.Init(newObjectSystem);
    //各種行動オブジェクト生成

    //移動の判定ノード
    firstNode = new BehaviorBaseNode;
    //判定スクリプト付きオブジェクト
    MoveJudgeObject* moveJudgeObject = new MoveJudgeObject(&enemy, &pl.pos);

    //行動オブジェクト
    approachNode      = new BehaviorBaseNode;
    approachObject    = new ApproachingMoveAct(&enemy, &pl.pos);
    approachObject->actScriptName = std::string("approach");
    approachNode->CreateActionNode("approachAction", approachObject, moveJudgeObject);
    retreatNode       = new BehaviorBaseNode;
    retreatObject     = new RetreatMoveAct(&enemy, &pl.pos);
    retreatObject->actScriptName = std::string("retreat");
    retreatNode->CreateActionNode("retreatAction", retreatObject, moveJudgeObject);
    waitNode          = new BehaviorBaseNode;
    waitObject        = new WaitAct(&enemy, &pl.pos);
    waitObject->actScriptName = std::string("wait");
    waitNode->CreateActionNode("WaitAction", waitObject, moveJudgeObject);

    //ノード生成
    firstNode->CreateJudgeNode("firstNode", BehaviorBaseNode::RULE_RANDOM, moveJudgeObject);
    firstNode->AddjudgeNodeChild(approachNode);
    firstNode->AddjudgeNodeChild(retreatNode);
    firstNode->AddjudgeNodeChild(waitNode);

    //ビヘイビア初期化
    enemyBehaviorTree.Init(firstNode);

    editor.Init(&enemyBehaviorTree);
    editor.AddEditData_Node(approachNode);
    editor.AddEditData_Node(retreatNode);
    editor.AddEditData_Node(waitNode);

    editor.AddEditData_ActScript(approachObject);
    editor.AddEditData_ActScript(retreatObject);
    editor.AddEditData_ActScript(waitObject);

    //マルチパス描画出力
    rtDrawer->SetAffineParam(scale, RVector3(0, 0, 0), RVector3(0, 0, 0));
}

//初期化
void Title::Initialize() {

}

void Title::Finalize()
{
    DeleteObject(ship2);
    DeleteObject(ship3);
    DeleteObject(newObjectSystem);
    DeleteObject(tileObject);

    pl.Finalize();

    delete firstNode;
    delete approachNode;
    delete approachObject;
    delete retreatNode;
    delete retreatObject;
    delete waitObject;
    delete waitNode;
}

//更新
void Title::Update() {
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    if(Input::isKey(DIK_SPACE)){ particle1->Prototype_Add(3, RVector3(0, 10, 0)); }

    //newObjectSystem->SetAffineParamTranslate(pos1);
    ship2->SetAffineParamTranslate(pos2);
    ship3->SetAffineParamTranslate(pos3);

    if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_L)) {
        particle1->Prototype_Add(5, pos1);
    }
    if (Input::isXpadButtonPushed(XPAD_BUTTON_OPTION_R)) {
        particle1->Prototype_Add(20, { 0,0,0 });
    }

    particle1->Prototype_Update();

    pl.Update();

    enemyBehaviorTree.Run();
}

//描画
void Title::Draw() {

    //パーティクル描画
    //particle1->Prototype_Draw(ptex);

    //描画1回目
    //newObjectSystem->DrawObject();

    NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);

    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

    ship3->DrawObject();
    tileObject->DrawObject();
    //pl.Draw();
    //enemy.Draw();

    NY_Object3DManager::Get()->CloseDrawObject3D();

    NY_Camera::Get()->SetViewStatusEyeTargetUp(eye2, target2, up2);

    rtDrawer->DrawRTexObject(&NY_Object3DManager::Get()->m_gBuffer);
    //ship2->DrawObject();

    //SpriteManager::Get()->SetCommonBeginDraw();
    //testInstance.DrawExtendSprite(x1, y1, x2, y2);
    //testInstance.DrawExtendSprite(x1, y1 + 50, x2, y2 + 50);
    ////本描画（実際に描画される）
    //testInstance.Draw();

    editor.EditorDraw();
    editor.ObjectDataDraw();
    editor.NodeDataDraw();

}
