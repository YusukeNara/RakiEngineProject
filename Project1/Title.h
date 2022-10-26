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

    //オブジェクト
    RVector3 scale = { 5,5,5 };
    Object3d *rtDrawer;
    //新型Object3d
    Object3d* ship2;
    Object3d* ship3;
    Object3d* saru;
    Object3d* box;
    RV3Colider::Rv3AABB boxAABB;
    //上用変換情報
    RVector3 scale1, rot1, pos1 = RVector3(-50, 250, 0);
    RVector3 scale2, rot2, pos2;
    RVector3 scale3, rot3, pos3;

    RVector3 v1;
    RVector3 v2;

    //時間
    int time = 0;

    Sprite testInstance;
    float x1, x2, y1, y2;

    //床
    Object3d *tileObject;
    std::array<Object3d*, 4> wallObject;

    UINT tiletex;

    DiferredRenderingMgr diffMgr;

    Player pl;
    Enemy enemy;

    //敵管理部
    EnemyManager emanager;
    //GUIテスト
    bool swapDraw = false;

    //タイトル、オーバー
    Sprite titleSprite, overSprite;

    RVector3 hitPos;

    NowState NowSceneState = game;
};

