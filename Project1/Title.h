#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"
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
    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override ;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。

    //シーンで使うインスタンス
    XMFLOAT3 eye = { 0,150,-200 };
    XMFLOAT3 target = { 0,0,0 };
    XMFLOAT3 up = { 0,1,0 };

    const float PI              = 3.14f;
    float ROPE_LENGHT     = 200.0f;
    const float CIRCLE_LENGHT   = (ROPE_LENGHT * 2 * PI);
    const float MASS            = 0.3f;
    const float G               = 9.81f;

    const float SIZE = 20.0f;

    float x;
    float speed;
    float angle;
    float axis_x = 320.0f;
    float axis_y = 100.0f;
    float nx, ny;

    //円スプライト
    Sprite circleSprite;
    
};

