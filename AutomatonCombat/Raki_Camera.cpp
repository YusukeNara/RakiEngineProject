#include "Raki_Camera.h"

void Raki_BaseCamera::Init()
{
}

void Raki_BaseCamera::UpdateViewMatrix()
{

	/// カメラz軸ベクトルを求める
	camAxisZ = XMVectorSubtract(targetVec, eyeVec);

	//Z軸ベクトルが0だと計算できないので除外
	assert(!XMVector3Equal(camAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(camAxisZ));
	//上方向ベクトルが0だと計算できないので除外
	assert(!XMVector3Equal(upVec, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVec));

	//z軸ベクトルを正規化
	camAxisZ = XMVector3Normalize(camAxisZ);

	/// カメラX軸ベクトルを求める
	camAxisX = XMVector3Cross(upVec, camAxisZ);
	//X軸ベクトル正規化
	camAxisX = XMVector3Normalize(camAxisX);

	/// カメラY軸ベクトルを求める
	camAxisY = XMVector3Cross(camAxisZ, camAxisX);
	//y軸ベクトル正規化
	camAxisY = XMVector3Normalize(camAxisY);

	/// 回転行列を求める
	matViewRot.r[0] = camAxisX;
	matViewRot.r[1] = camAxisY;
	matViewRot.r[2] = camAxisZ;
	matViewRot.r[3] = XMVectorSet(0, 0, 0, 1);

	/// 転置により回転行列の逆行列を求め、ビューに反映
	matView = XMMatrixTranspose(matViewRot);

	/// 平行移動の逆行列を求める

	//カメラ位置の逆ベクトル(* -1)
	XMVECTOR revEye = XMVectorNegate(eyeVec);
	//カメラ位置からワールド原点のベクトル
	XMVECTOR tx = XMVector3Dot(camAxisX, revEye);//x
	XMVECTOR ty = XMVector3Dot(camAxisY, revEye);//y
	XMVECTOR tz = XMVector3Dot(camAxisZ, revEye);//z
	//まとめる
	XMVECTOR t  = XMVectorSet(tx.m128_f32[0], ty.m128_f32[1], tz.m128_f32[2], 1.0f);

	/// ビューに反映
	matView.r[3] = t;

}

void Raki_BaseCamera::Update()
{

	//ビュー行列更新
	UpdateViewMatrix();
}

XMMATRIX Raki_BaseCamera::GetView()
{
	return matView;
}
