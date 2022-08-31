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

    //敵初期化
    swordEnemy = new SwordEnemy(&pl);

    diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);
}

//初期化
void Title::Initialize() {
    swordEnemy->Init();
}

void Title::Finalize()
{
    DeleteObject(ship2);
    DeleteObject(ship3);
    DeleteObject(tileObject);

    pl.Finalize();
    delete firstNode;
    delete approachNode;
    delete approachObject;
    delete retreatNode;
    delete retreatObject;
    delete waitObject;
    delete waitNode;
    delete swordEnemy;
}

//更新
void Title::Update() {
    pl.Update();
    ship2->SetAffineParamTranslate(pos2);
    ship3->SetAffineParamRotate(rot3);
    ship3->SetAffineParamTranslate(pos1);
    saru->SetAffineParamTranslate(pos3);

    if (Input::isKey(DIK_A)) {
        eye.x -= 1;
        target.x -= 1;
    }
    if (Input::isKey(DIK_D)) {
        eye.x += 1;
        target.x += 1;
    }
    if (Input::isKey(DIK_W)) {
        eye.z += 1;
        target.z += 1;
    }
    if (Input::isKey(DIK_S)) {
        eye.z -= 1;
        target.z -= 1;
    }
    saru->SetAffineParam(scale3, rot3, pos3);

    swordEnemy->Update();


    //カメラパラメーター計算
    camRightAngle = 0.0f;
    camUpAngle = 0.0f;
    if (Input::isKey(DIK_RIGHT)) { camRightAngle += 0.01f; }
    if (Input::isKey(DIK_LEFT)) { camRightAngle -= 0.01f; }
    if (Input::isKey(DIK_UP)) { camUpAngle += 0.01f; }
    if (Input::isKey(DIK_DOWN)) { camUpAngle -= 0.01f; }

    RVector3 camUp = RVector3(up.x, up.y, up.z);
    RVector3 camTarget = RVector3(target.x, target.y, target.z);
    RVector3 camEye = RVector3(eye.x, eye.y, eye.z);
    RQuaternion qCamRight = quaternion(RVector3(up.x, up.y, up.z), camRightAngle);
    RVector3 camSide = cross(camUp, camTarget - camEye).norm();
    RQuaternion qCamUp = quaternion(camSide, camUpAngle);
    RQuaternion q = qCamRight * qCamUp;
    RQuaternion qPos = quaternion(eye.x, eye.y, eye.z, 0);
    RQuaternion qq = conjugate(q);
    qPos = q * qPos * qq;
    camEye = { qPos.x,qPos.y,qPos.z };
    RQuaternion qup = RQuaternion(camUp.x, camUp.y, camUp.z, 0);
    qup = q * qup * qq;
    camUp = getAxis(qup);

    eye = { camEye.x ,camEye.y,camEye.z };
    RVector3 eyepos = { camEye.x - pl.pos.x ,camEye.y - pl.pos.y,camEye.z - pl.pos.z };
    target;
    up = { camUp.x,camUp.y,camUp.z };
    camera->SetViewStatusEyeTargetUp(eyepos, target, up);

}

//描画
void Title::Draw() {

    NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);

    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

    tileObject->DrawObject();
    //saru->DrawObject();
    //ship3->DrawObject();
    pl.Draw();
    enemy.Draw();
    swordEnemy->Draw();

    NY_Object3DManager::Get()->CloseDrawObject3D();

    diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer);

    ImguiMgr::Get()->StartDrawImgui("window", 100, 100);

    ImGui::Text("enemyPos %f %f %f", swordEnemy->s_object.pos.x, swordEnemy->s_object.pos.y, swordEnemy->s_object.pos.z);

    ImguiMgr::Get()->EndDrawImgui();

    swordEnemy->DebugDraw();

}
