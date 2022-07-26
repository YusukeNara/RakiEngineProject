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

    //�摜�̓ǂݍ���
    tiletex = TexManager::LoadTexture("Resources/white.png");
    tileObject = NY_Object3DManager::Get()->CreateModel_Tile(500, 500, 50, 50, tiletex);
    tileObject->color = DirectX::XMFLOAT4(1, 1, 1, 1);
    tileObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));

    //�X�v���C�g�ɉ摜�����蓖�Ă�i�摜�̃n���h���A�c���A�����j
    testInstance.Create(tiletex);
    x1 = 100.0f;
    x2 = 150.0f;
    y1 = 100.0f;
    y2 = 150.0f;

    //�v���C���[
    pl.Init();
    //�G
    enemy.Init(ship2);
    //�e��s���I�u�W�F�N�g����
    //�ړ��̔���m�[�h
    firstNode = new BehaviorBaseNode;
    //����X�N���v�g�t���I�u�W�F�N�g
    MoveJudgeObject* moveJudgeObject = new MoveJudgeObject(&enemy, &pl.pos);
    //�s���I�u�W�F�N�g
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
    //�m�[�h����
    firstNode->CreateJudgeNode("firstNode", BehaviorBaseNode::RULE_RANDOM, moveJudgeObject);
    firstNode->AddjudgeNodeChild(approachNode);
    firstNode->AddjudgeNodeChild(retreatNode);
    firstNode->AddjudgeNodeChild(waitNode);
    //�r�w�C�r�A������
    enemyBehaviorTree.Init(firstNode);
    editor.Init(&enemyBehaviorTree);
    editor.AddEditData_Node(approachNode);
    editor.AddEditData_Node(retreatNode);
    editor.AddEditData_Node(waitNode);
    editor.AddEditData_ActScript(approachObject);
    editor.AddEditData_ActScript(retreatObject);
    editor.AddEditData_ActScript(waitObject);

    //�}���`�p�X�`��o��
    rtDrawer->SetAffineParam(scale, RVector3(0, 0, 0), RVector3(0, 0, 0));

    diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);
}

//������
void Title::Initialize() {

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
}

//�X�V
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

    rot3.y += 1.0f;
    saru->SetAffineParam(scale3, rot3, pos3);

    camera->SetViewStatusEyeTargetUp(eye, target, up);

    //enemyBehaviorTree.Run();
}

//�`��
void Title::Draw() {

    NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);

    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

    tileObject->DrawObject();
    saru->DrawObject();
    ship3->DrawObject();
    pl.Draw();
    enemy.Draw();

    NY_Object3DManager::Get()->CloseDrawObject3D();

    diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer);

    ImguiMgr::Get()->StartDrawImgui("window", 100, 100);



    ImguiMgr::Get()->EndDrawImgui();

    //editor.EditorDraw();
    //editor.ObjectDataDraw();
    //editor.NodeDataDraw();

}
