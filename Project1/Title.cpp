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

    //�摜�̓ǂݍ���
    tiletex = TexManager::LoadTexture("Resources/grif.png");

    tileObject = NY_Object3DManager::Get()->CreateModel_Tile(500, 500, 50, 50, tiletex);
    tileObject->color = DirectX::XMFLOAT4(1, 1, 1, 0.5);
    tileObject->SetAffineParam(RVector3(1, 1, 1), RVector3(90, 0, 0), RVector3(0, 0, 0));

    //�X�v���C�g�ɉ摜�����蓖�Ă�i�摜�̃n���h���A�c���A�����j
    testInstance.Create(tiletex);
    x1 = 100.0f;
    x2 = 150.0f;
    y1 = 100.0f;
    y2 = 150.0f;

    //�p�[�e�B�N��
    particle1 = ParticleManager::Create();
    bomProto = new BomParticle(RVector3(0, 0, 0));
    //�v���g�^�C�v���Z�b�g
    particle1->Prototype_Set(bomProto);
    //�p�[�e�B�N���̃e�N�X�`��
    ptex = TexManager::LoadTexture("Resources/effect1.png");

}

//������
void Title::Initialize() {
    /// <summary>
    /// �t�B�[���h�Ǘ���������
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

//�X�V
void Title::Update() {
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    NY_Object3DManager::Get()->UpdateAllObjects();

    if(Input::isKey(DIK_SPACE)){ particle1->Prototype_Add(3, RVector3(0, 10, 0)); }

    x2 -= 0.1f;

    //Object3d�̃e�X�g

    //���x�v�Z


    //�d�͗���
    time++;
    pos1.y = CalcGravity(200, 0, 1, time, vel.y);
    

    if (Input::isKey(DIK_LEFT)) {
        vel.x += CalcAccelToFramePerVel(-3.0f);
        //���������^��
        vel2.x = -1.0f;
        //�������x�����^���i�������ςȂ��ő��x�㏸�j
        //������1sec�̈ړ��ʁA������t���[�����[�g�Ŋ�����1F�̈ړ��ʂ��Z�o
        vel3.x += CalcAccelToFramePerVel(-3.0f);
    }
    if (Input::isKey(DIK_RIGHT)) {
        vel.x += CalcAccelToFramePerVel(3.0f);
        //���������^��
        vel2.x = 1.0f;
        //�������x�����^��
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

    ////���������^�� �R���g���[���[���͋��� * ���x�萔
    ////vel2.x = Input::GetXpadLStickTilt().x_rate * 1.0f;
    ////vel2.z = Input::GetXpadLStickTilt().y_rate * 1.0f;
    ////�������x�^��  (1sec�ł̈ړ��� * �R���g���[���[���͋���) / �t���[�����[�g
    //vel3.x += CalcAccelToFramePerVel(60.0f * Input::GetXpadLStickTilt().x_rate);

    //���x���Z
    //pos1 += vel;
    pos2 += vel2;
    pos3 += vel3;

    //���ɂ���������
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

//�`��
void Title::Draw() {

    //�w�i�ɏ�ɂ���
    Raki_DX12B::Get()->StartDrawRenderTarget();

    Raki_DX12B::Get()->StartDrawBackbuffer();

    //�p�[�e�B�N���`��
    particle1->Prototype_Draw(ptex);

    //�`��1���
    //testFbx->DrawObject();
    newObjectSystem->DrawObject();
    ship2->DrawObject();
    ship3->DrawObject();
    tileObject->DrawObject();

    SpriteManager::Get()->SetCommonBeginDraw();

    testInstance.DrawExtendSprite(x1, y1, x2, y2);
    testInstance.DrawExtendSprite(x1, y1 + 50, x2, y2 + 50);

    //�{�`��i���ۂɕ`�悳���j
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

    //�`��I��
    Raki_DX12B::Get()->CloseDraw();

}
