#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"
#include "Raki_Input.h"

class GameScene : public BaseScene {
public:
    GameScene(ISceneChanger *changer);
    void Initialize() override;    //�������������I�[�o�[���C�h�B
    void Finalize() override;        //�I���������I�[�o�[���C�h�B
    void Update() override;        //�X�V�������I�[�o�[���C�h�B
    void Draw() override;            //�`�揈�����I�[�o�[���C�h�B

    ////���f��
    //Model3D boxModel;
    ////�I�u�W�F�N�g
    //Object3d boxObject;

};

