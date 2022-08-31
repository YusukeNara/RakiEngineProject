#pragma once
#include <math.h>

#include <Raki_Input.h>
#include <Sprite.h>

#include "Vec2.h"

#define	SX_SCREEN	1280
#define	SY_SCREEN	720

#define	R_OBJ		 16		//物体の半径
#define	LENGTH_LINK	 50		//物体のリンクの長さ
#define	K_HOOK		(0.01f)	//フックの法則の係数K
#define	K_V_RES		(0.1f)	//粘性抵抗係数

#define	ACC_G		(0.2f)	//重力加速度

#define	NUM_HIST	1000



class Obj
{
public:
	int	stat;	//0=存在しない、1=存在して活動中、2=存在して静止
	int	graph;

	Sprite sphere;
	Sprite line;

	Vec2	pos;
	Vec2	size;
	Vec2	vel;
	Vec2	acc;
	double	m;
	double	r;
	double	dist;
	Vec2	hp[NUM_HIST];
	int		idx_hist;
	//	int	hit;
	int	idx;
	//	int hit_cnt;
	Obj* link0;	//ゴムでつながっているOBJ
	Obj* link1;	//ゴムでつながっているOBJ
	int	grabbed;	//マウスで掴まれている

	Obj();
	void init();
	void update();
	void disp();

};

// コンストラクタ オーバーロード
inline Obj::Obj()
{
	sphere.Create(TexManager::LoadTexture("Resources/Sphere.png"));
	line.Create(TexManager::LoadTexture("Resources/white.png"));

	pos.clear(0);
	r = R_OBJ;
	dist = 0;
	vel.clear(0);
	acc.clear(0);
	m = 1.0f;
	stat = 0;
	idx_hist = 0;
	//	hit = 0;
	//	hit_cnt = 0;
	for (int i = 0; i < NUM_HIST; i++)
	{
		hp[i].clear(0);
	}
	link0 = NULL;
	link1 = NULL;
	grabbed = 0;
}

//double g = 0.0f;

inline void Obj::init()
{
	sphere.Create(TexManager::LoadTexture("Resources/Sphere.png"));
	line.Create(TexManager::LoadTexture("Resources/white.png"));

	pos.clear(0);
	r = R_OBJ;
	dist = 0;
	vel.clear(0);
	acc.clear(0);
	m = 1.0f;
	stat = 0;
	idx_hist = 0;
	//	hit = 0;
	//	hit_cnt = 0;
	for (int i = 0; i < NUM_HIST; i++)
	{
		hp[i].clear(0);
	}
	link0 = NULL;
	link1 = NULL;
	grabbed = 0;
}

// メンバ変数の表示
inline void Obj::update()
{
	hp[idx_hist % NUM_HIST].x = pos.x;
	hp[idx_hist % NUM_HIST].y = pos.y;
	idx_hist++;

	if (grabbed)
	{
		DirectX::XMFLOAT2 m = Input::getMousePos();
		pos.x = m.x;
		pos.y = m.y;
		if (!Input::isMouseClickTrigger(MOUSE_L))
			grabbed = 0;
	}
	else
	{
		DirectX::XMFLOAT2 m = Input::getMousePos();
		Vec2 MousePos = Vec2(m.x, m.y);
		double _dst = pos.distance(MousePos);
		if (Input::isMouseClickTrigger(MOUSE_L))
		{
			if (_dst < r)
				grabbed = 1;
		}

	}

	// 0:存在してない　1:存在して活動中　2:存在してて活動してない
	if (stat == 2) {
		return;
	}


	{
		// acc:加速度
		acc.x = 0.0f;
		acc.y = ACC_G;	//重力加速度（値は適当で良い）

		if (link0)	//親OBJがある?
		{
			// 自分の1つ上のobjへのベクトル
			Vec2	_dist = link0->pos - pos;
			// ↑のベクトルの大きさ
			double	_len = _dist.length();

			// 親objとの距離が標準距離（LENGTH_LINK）より大きければ
			// 加速度を加算
			if (_len > LENGTH_LINK)
			{
				// ベクトル(_dist)＊標準距離からの増分
				_dist = _dist * (_len - LENGTH_LINK);

				_dist /= LENGTH_LINK;

				// K_HOOK:ばね定数（伸びにくさ）
				// 質量(m)が大きいほど_distを掛けた加速度(acc)の増分は減る
				Vec2 add_acc = _dist * K_HOOK / m;
				acc += add_acc;
			}
		}
		if (link1)	//子OBJがある?
		{
			// 自分の１つ下のobjへのベクトル
			Vec2	_dist = link1->pos - pos;
			// ↑のベクトルの大きさ
			double	_len = _dist.length();

			if (_len > LENGTH_LINK)
			{
				_dist = _dist * (_len - LENGTH_LINK) / LENGTH_LINK;
				acc += _dist * K_HOOK / m;
			}
		}

		vel += acc;			//速度+=加速度

		vel -= vel * K_V_RES;	//粘性抵抗
								// (物体が近傍の流体を引きずることによって受ける反作用)

		pos += vel;			//位置+=速度
	}

}

inline void Obj::disp()
{
	double	x = pos.x + SX_SCREEN / 2;
	double	y = pos.y + SY_SCREEN / 2;

	sphere.DrawExtendSprite(x - r, y - r, x + r, y + r);
	sphere.Draw();

	//linkの表示
	if (link0)
	{
		double	xl = link0->pos.x + SX_SCREEN / 2;
		double	yl = link0->pos.y + SY_SCREEN / 2;
		line.DrawLine(x, y, xl, yl);//線を描画
		line.Draw();
	}
}

#define	NUM_OBJ	10





class RopeTask
{
public:
	Obj	a[NUM_OBJ];

	int	_idx_obj;

	int			MouseInput;
	bool		MouseInputLeft;
	Vec2		MousePosInScreen;
	Vec2		MousePos;

	void Init();

	void Update();

	void Draw();

	inline void	updateMouse(void)
	{
		MouseInputLeft = Input::isMouseClickTrigger(MOUSE_L);
		int	_x, _y;
		DirectX::XMFLOAT2 m;
		m = Input::getMousePos();
		_x = m.x;
		_y = m.y;
		MousePosInScreen.x = _x;
		MousePosInScreen.y = _y;
		MousePos.x = _x - SX_SCREEN / 2;
		MousePos.y = _y - SY_SCREEN / 2;
	}

};

