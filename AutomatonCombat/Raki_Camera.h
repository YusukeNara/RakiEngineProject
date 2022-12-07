#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// <para> リメイク版カメラ、ビュー行列から作っている</para>
/// <para> カメラに追従などの機能をもたせる場合は、継承を用いて機能を実装すること</para>
/// </summary>
class Raki_BaseCamera
{
private:
	// ---------- プライベートメンバ変数 ----------//

	//ビュー行列
	XMMATRIX matView;
	//回転行列
	XMMATRIX matViewRot;
	//回転方向ベクトル
	XMVECTOR camAxisX, camAxisY, camAxisZ;

public:
	// ---------- コンストラクタ、デストラクタ ---------- //
	Raki_BaseCamera() {
		//ビュー構成要素初期化
		XMFLOAT3 ie{ 0,0,0 }, it{ 0,0,camRange }, iu{ 0,1,0 };
		eyeVec    = XMLoadFloat3(&ie);
		targetVec = XMLoadFloat3(&it);
		upVec     = XMLoadFloat3(&iu);
		//回転角初期化
		XMFLOAT3 rad{ 0,0,0 };
		camRad = XMLoadFloat3(&rad);
		//ビュー行列初期化
		matView    = XMMatrixIdentity();
		matViewRot = XMMatrixIdentity();
		//回転方向ベクトル初期化
		camAxisX = XMLoadFloat3(&ie);
		camAxisY = XMLoadFloat3(&ie);
		camAxisZ = XMLoadFloat3(&ie);

		UpdateViewMatrix();
	};
	~Raki_BaseCamera() {};

	// ---------- メンバ変数 ---------- //

	///ビュー構成要素
	XMVECTOR eyeVec;    //カメラ位置
	XMVECTOR targetVec; //視線方向ベクトル
	XMVECTOR upVec;     //上方向ベクトル

	//カメラ回転角要素
	XMVECTOR    camRad;           //カメラ回転角
	const float camRange = 5.0f;  //カメラ位置と視線方向ベクトルとの距離

	// ---------- メンバ関数 ---------- //

	/// <summary>
	/// カメラ初期化
	/// </summary>
	void Init();

	/// <summary>
	/// ビュー行列更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// 仮想関数
	/// <para> 基底クラスではUpdateViewMatrixの呼び出しのみ実行</para>
	/// <para> 派生クラスでは対応する機能に応じた更新処理を記述して実行すること</para>
	/// <para> ただし派生先では必ず、関数内でUpdateViewMatrixを実行すること</para>
	/// <para>（派生先での固有の変数がある場合はまた別だが、そんなときは更新処理は別で記述するべきかも）</para>
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	XMMATRIX GetView();

};

