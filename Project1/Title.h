#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"

#include "Sprite.h"

#include "Raki_Input.h"
#include "Audio.h"
#include <ParticleManager.h>

#include "testParticle.h"

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
    XMFLOAT3 eye = { 0,30,-150 };
    XMFLOAT3 target = { 0,0,0 };
    XMFLOAT3 up = { 0,1,0 };

    XMFLOAT3 eye2 = { 0,50,0 };
    XMFLOAT3 target2 = { 0,0,0 };
    XMFLOAT3 up2 = { 0,1,0 };

    int animationTime;
    const int ANIM_TIME = 60;

    //�I�u�W�F�N�g
    Object3d *obj1;
    Object3d *obj2;
    Object3d *gmodel;
    RVector3 scale = { 5,5,5 };

    //�V�^Object3d
    Object3d *newObjectSystem;
    //��p�ϊ����
    RVector3 scale1, rot1, pos1;
    RVector3 scale2, rot2, pos2;

    Sprite testInstance;

    //��
    Object3d *tileObject;
    UINT tiletex;

    Object3d *animTestModel;

    //�}�E�X�m�F�ϐ�
    bool clicking = false;
    bool clickTrigger = false;
    bool clicked = false;
    XMFLOAT2 mousePos;
    XMFLOAT2 mouseVel;

    //�����_�����O���ʕ`��

    bool isadd = false;

    //�p�[�e�B�N��
    ParticleManager *particle1;
    //�p�[�e�B�N���v���g�^�C�v
    BomParticle *bomProto;
    //�p�[�e�B�N���o���Ԋu
    int pSpawnDelay = 5;
    int PSPAWN_DELAY_TIME = 15;
    //�p�[�e�B�N���o����
    int pSpawnNum = 1;
    //�p�[�e�B�N�����\�[�X

    UINT ptex;

    int adrate = 0;

    int isControlColor1 = 0;
};

