#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"
#include <DifferrdRenderingMgr.h>
#include "Sprite.h"
#include "Raki_Input.h"
#include "Audio.h"
#include <ParticleManager.h>
#include "testParticle.h"
#include "Player.h"
#include "Enemy.h"
#include <BehaviorTree.h>
#include <BehaviorEditor.h>
#include "SwordEnemy.h"
#include "EnemyManager.h"

#include <Quaternion.h>

enum NowState {
    title,
    game,
    clear,
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
    XMFLOAT3 eye = { 0,10,75 };
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
    Object3d* box;
    RV3Colider::Rv3AABB boxAABB;
    //��p�ϊ����
    RVector3 scale1, rot1, pos1 = RVector3(-50, 250, 0);
    RVector3 scale2, rot2, pos2;
    RVector3 scale3, rot3, pos3;

    RVector3 v1;
    RVector3 v2;

    //����
    int time = 0;

    Sprite testInstance;
    float x1, x2, y1, y2;

    //��
    Object3d *tileObject;
    std::array<Object3d*, 4> wallObject;

    UINT tiletex;

    DiferredRenderingMgr diffMgr;

    Player pl;
    Enemy enemy;

    //�G�Ǘ���
    EnemyManager emanager;
    //GUI�e�X�g
    bool swapDraw = false;

    //�^�C�g���A�I�[�o�[
    Sprite titleSprite, overSprite;

    RVector3 hitPos;

    NowState NowSceneState = game;
};

