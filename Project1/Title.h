#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"

#include <DifferrdRenderingMgr.h>
//#include <FbxLoader.h>

#include "Sprite.h"

#include "Raki_Input.h"
#include "Audio.h"
#include <ParticleManager.h>

#include "testParticle.h"
#include "Player.h"
#include "Enemy.h"
#include <BehaviorTree.h>
#include <BehaviorEditor.h>

enum NowState {
    title,
    game,
    over,
};


class Title : public BaseScene {

public:
    Title(ISceneChanger *changer);
    void Initialize() override;    //�������������I�[�o�[���C�h�B
    void Finalize() override ;   //�I���������I�[�o�[���C�h�B
    void Update() override;        //�X�V�������I�[�o�[���C�h�B
    void Draw() override;          //�`�揈�����I�[�o�[���C�h�B

    //�V�[���Ŏg���C���X�^���X
    XMFLOAT3 eye = { 0,20,-75 };
    XMFLOAT3 target = { 0,0,0 };
    XMFLOAT3 up = { 0,1,0 };

    XMFLOAT3 eye2 = { 0,25,-300 };
    XMFLOAT3 target2 = { 0,0,0 };
    XMFLOAT3 up2 = { 0,1,0 };

    //�I�u�W�F�N�g
    RVector3 scale = { 5,5,5 };
    Object3d *rtDrawer;
    //�V�^Object3d
    Object3d* ship2;
    Object3d* ship3;
    Object3d* saru;
    //��p�ϊ����
    RVector3 scale1, rot1, pos1 = RVector3(-50, 250, 0);
    RVector3 scale2, rot2, pos2;
    RVector3 scale3, rot3, pos3;
    //���x�i�[
    RVector3 vel = RVector3(0, 0, 0);
    RVector3 vel2 = RVector3(0, 0, 0);
    RVector3 vel3 = RVector3(0, 0, 0);
    RVector3 acc = RVector3(0, 0, 0);
    RVector3 acc2 = RVector3(0, 0, 0);
    RVector3 acc3 = RVector3(0, 0, 0);

    //����
    int time = 0;

    Sprite testInstance;
    float x1, x2, y1, y2;

    //��
    Object3d *tileObject;
    UINT tiletex;

    //�}�E�X�m�F�ϐ�
    bool clicking = false;
    bool clickTrigger = false;
    bool clicked = false;
    XMFLOAT2 mousePos;
    XMFLOAT2 mouseVel;

    //�����_�����O���ʕ`��
    bool isadd = false;
    //�p�[�e�B�N���o���Ԋu
    int pSpawnDelay = 5;
    int PSPAWN_DELAY_TIME = 15;
    //�p�[�e�B�N���o����
    int pSpawnNum = 1;
    //�p�[�e�B�N�����\�[�X
    int adrate = 0;
    int isControlColor1 = 0;

    //Player pl;
    //Enemy enemy;
    //AI_BehaviorTree enemyBehaviorTree;
    //BehaviorBaseNode* firstNode;
    //BehaviorBaseNode* approachNode;
    //ApproachingMoveAct* approachObject;
    //BehaviorBaseNode* retreatNode;
    //RetreatMoveAct* retreatObject;
    //BehaviorBaseNode* waitNode;
    //WaitAct* waitObject;
    //BehaviorEditor editor;

    //GUI�e�X�g
    bool swapDraw = false;

    DiferredRenderingMgr diffMgr;
};

