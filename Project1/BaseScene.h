#pragma once
#include "Task.h"
#include "ISceneChanger.h"
#include "Raki_DX12B.h"
#include "Raki_WinAPI.h"
#include <wrl.h>

using namespace Microsoft::WRL;

/// <summary>
/// 各シーンの基底クラス、各シーンはこれを継承することで実装
/// <para>基底クラスはメンバ変数として、RakiDX12、NY_Object3DManager、SpriteManagerを持つ</para>
/// <para>これにより、派生先でObject3D、Spriteのインスタンスを使用したり、Managerの機能を使用可能</para>
/// </summary>
class BaseScene : public Task {

    //template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:
    
    ISceneChanger *mSceneChanger;    //クラス所有元にシーン切り替えを伝えるインターフェイス
    //static Raki_DX12B *dx12b;        //Raki_DX12Bクラスのポインタ、派生先でも基本1つしか使わないのでstaticに

public:
    BaseScene(ISceneChanger *changer);
    BaseScene() {};
    virtual ~BaseScene() {

    }



    virtual void Initialize() override {}    //初期化処理をオーバーライド。
    virtual void Finalize() override {}        //終了処理をオーバーライド。
    virtual void Update() override {}        //更新処理をオーバーライド。
    virtual void Draw() override{}            //描画処理をオーバーライド。

};

