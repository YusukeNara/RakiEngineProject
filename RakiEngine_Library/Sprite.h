#pragma once
#include "SpriteManager.h"
#include "NY_Camera.h"
#include "TexManager.h"

class Sprite
{
public:
	//スプライト一枚の情報
	SpriteData spdata;
	//アニメーションデータ
	uvAnimData *animData;

public:
	//スプライト生成（実体生成時に起動でいい？）(デバイス、スプライトサイズ、リソース番号、アンカーポイント、スプライトマネージャーポインタ)
	Sprite(XMFLOAT2 size,UINT resourceID, bool adjustResourceFlag = false,XMFLOAT2 anchor = { 0.0f,0.0f })
	{
		//スプライト生成
		CreateSprite(size, anchor, resourceID, adjustResourceFlag, nullptr);
	};
	//引数なし（別で初期化）
	Sprite() {};

	//スプライト初期化(デバイス、スプライトサイズ、アンカーポイント、使用するリソース番号、リソース調整フラグ)
	void CreateSprite(XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag, uvAnimData *animData = nullptr);

	/// <summary>
	/// 描画のもとになるスプライトデータ生成
	/// </summary>
	/// <param name="resourceID">テクスチャハンドル</param>
	/// <param name="sizeX">描画サイズ</param>
	/// <param name="sizeY">描画サイズ</param>
	/// <param name="reserveDrawCount">描画する数</param>
	void Create(UINT resourceID, float sizeX, float sizeY);

	//サイズ変更
	void ResizeSprite(XMFLOAT2 newsize);

	//スプライト更新（エンジンで勝手にやる）
	void UpdateSprite();

	//頂点インスタンスデータを更新
	void InstanceUpdate();

	//スプライト描画
	void Draw();

	void DrawSprite(float posX, float posY);

	void DrawExtendSprite(float x1, float y1, float x2, float y2);

	//マルチパス結果描画
	void DrawMPRender();

private:

	UINT sizeInsVB;

	UINT instanceDrawCount;

	bool isVertexBufferNeedResize();

	//頂点バッファのサイズ変更（インスタンシング用バッファ）
	void ResizeVertexInstanceBuffer(UINT newWidthSize);

};

///新、スプライト生成の仕方
///
/// テクスチャ番号を指定して作成
/// ↓
/// Updateはしない
/// ↓
/// Draw()以外の描画関数を呼ぶ
/// ↓
/// これ以上描画しないタイミングでDraw()を実行
/// 
/// 描画関数
/// DrawSprite(x,y); 左上基準、通常描画
/// 


