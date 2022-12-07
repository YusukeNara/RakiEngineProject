#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Sprite.h"

#pragma comment(lib, "d3d12.lib")

#define IKD_EPSIRON 0.00001f//誤差

using namespace DirectX;

//スクリーン座標をワールド座標に変換（対象のスクリーン座標X,Y,射影空間Z位置,ビューポート範囲？,射影変換行列）
XMFLOAT3 Proto_CalcScreenToWorldPos(float scrX, float scrY, float fz, int window_w, int window_h, XMMATRIX prj)
{
	XMVECTOR pos;
	//射影変換行列とビューポート行列の逆行列を格納する変数
	XMMATRIX InvPrj, InvVP;
	//各行列の逆行列を出す
	InvPrj = XMMatrixInverse(nullptr, prj);
	//ビューポート行列はもとはない？のでここで定義して逆行列を出す
	InvVP = XMMatrixIdentity();
	InvVP.r[0].m128_f32[0] = window_w / 2.0f;
	InvVP.r[1].m128_f32[1] = -window_h / 2.0f;
	InvVP.r[3].m128_f32[0] = window_w / 2.0f;
	InvVP.r[3].m128_f32[1] = window_h / 2.0f;
	InvVP = XMMatrixInverse(nullptr, InvVP);

	XMMATRIX inverce = InvVP * InvPrj;
	XMVECTOR scr = { scrX,scrY,fz};

	pos = XMVector3TransformCoord(scr, inverce);

	XMFLOAT3 returnpos = { pos.m128_f32[0],pos.m128_f32[1],pos.m128_f32[2] };

	return returnpos;
}


XMVECTOR Proto_CalcScreenToWorldPosVector(float scrX, float scrY, float fz, int window_w, int window_h, XMMATRIX prj,XMMATRIX view)
{
	XMVECTOR pos;
	//射影変換行列とビューポート行列の逆行列を格納する変数
	XMMATRIX InvPrj, InvVP, InvCam;
	//各行列の逆行列を出す

	//射影変換の逆行列
	InvPrj = XMMatrixInverse(nullptr, prj);
	//ビューポート行列はもとはない？のでここで定義して逆行列を出す
	InvVP = XMMatrixIdentity();
	InvVP.r[0].m128_f32[0] = (float) window_w / 2.0f;
	InvVP.r[1].m128_f32[1] = (float)-window_h / 2.0f;
	InvVP.r[2].m128_f32[2] = (float)1.0f - 0.0f;
	InvVP.r[3].m128_f32[0] = (float) window_w / 2.0f;
	InvVP.r[3].m128_f32[1] = (float) window_h / 2.0f;
	InvVP.r[3].m128_f32[2] = (float)0.0f;
	InvVP = XMMatrixInverse(nullptr, InvVP);

	InvCam = XMMatrixInverse(nullptr, InvCam);
	//各逆行列を乗算
	XMMATRIX inverce = InvVP * InvPrj * InvCam;
	//スクリーン座標に射影zを加えたものをXMVECTORで定義
	XMVECTOR scr = { scrX,scrY,fz };
	//スクリーンに射影zを加えたやつと合成逆行列を乗算
	pos = XMVector3TransformCoord(scr, inverce);
	//レイの方向ベクトルを返却
	return pos;
}

/*
	上述関数を使用してnearとfarを求めたとき、zが射影空間のzとして計算されない（差が1になる）

	原因：引数に取った射影行列が、スプライトの射影行列だった。
	射影は3D空間上に描画される物体を直方体空間内に収めるための行列。
	しかしspriteは2D空間なので、これの逆行列を使用して変換しても2Dのまま。

	3Dオブジェクトの変換に使用している射影行列を使用しなければならない！！！
*/

//レイと球の衝突点を計算し返却(対象の半径、対象の中心点、レイの開始点、レイの方向ベクトル、（出力）衝突点、（出力）距離)
bool CalcSphereRayColision(float rad, XMVECTOR center, XMVECTOR rayStart, XMVECTOR rayVec, XMVECTOR *out_ColiP, float *pOut_t)
{
	XMVECTOR u = rayStart - center;

	float a = XMVectorGetX(XMVector3Dot(rayVec, rayVec));
	float b = XMVector3Dot(rayVec, u).m128_f32[0];
	float c = XMVector3Dot(u, u).m128_f32[0] - rad * rad;

	if (a - IKD_EPSIRON <= 0.0f) {
		//誤差
		return false;
	}

	float isColli = b * b - a * c;
	if (isColli < 0.0f) {
		//衝突しない
		return false;
	}

	float t = (-b - sqrt(b * b - a * c)) / a;

	if (pOut_t) {
		*pOut_t = t;
	}

	if (out_ColiP) {
		*out_ColiP = rayStart + rayVec * t;
	}

	return true;
}


XMVECTOR GetFollowingBulletPos(XMVECTOR targetPos, XMVECTOR bulletPos, XMVECTOR velocity, float period)
{
	XMVECTOR accel = { 0,0,0 };
	XMVECTOR diff = targetPos - bulletPos;

	accel += (diff - velocity * period) * 2.0f / (period * period);

	XMVECTOR vel = velocity;
	vel += accel;

	XMVECTOR returnPos = bulletPos;
	returnPos += vel;
	
	return returnPos;
}