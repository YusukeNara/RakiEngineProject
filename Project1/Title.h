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
#include "GroundObject.h"
#include "BuildingObject.h"

enum NowState {
    title,
    game,
    clear,
    over,
};


class Title : public BaseScene {

public:
    Title(ISceneChanger *changer);
    ~Title() override;

    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override ;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。

    //シーンで使うインスタンス
    XMFLOAT3 eye = { 0,10,75 };
    XMFLOAT3 target = { 0,0,0 };
    XMFLOAT3 up = { 0,1,0 };

    XMFLOAT3 eye2 = { 0,25,-300 };
    XMFLOAT3 target2 = { 0,0,0 };
    XMFLOAT3 up2 = { 0,1,0 };

    DiferredRenderingMgr diffMgr;

    Player pl;

    ////敵管理部
    EnemyManager emanager;

    //タイトル、オーバー
    Sprite titleSprite, overSprite;

    RVector3 hitPos;

    NowState NowSceneState = title;

    GroundObject gobject;

    std::array<BuildingObject,10> build;
};

