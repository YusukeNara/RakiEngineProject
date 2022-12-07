#pragma once
#include <math.h>

#include <Raki_Input.h>
#include <Sprite.h>

#include "Vec2.h"

#define	SX_SCREEN	1280
#define	SY_SCREEN	720

#define	R_OBJ		 16		//���̂̔��a
#define	LENGTH_LINK	 50		//���̂̃����N�̒���
#define	K_HOOK		(0.01f)	//�t�b�N�̖@���̌W��K
#define	K_V_RES		(0.1f)	//�S����R�W��

#define	ACC_G		(0.2f)	//�d�͉����x

#define	NUM_HIST	1000



class Obj
{
public:
	int	stat;	//0=���݂��Ȃ��A1=���݂��Ċ������A2=���݂��ĐÎ~
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
	Obj* link0;	//�S���łȂ����Ă���OBJ
	Obj* link1;	//�S���łȂ����Ă���OBJ
	int	grabbed;	//�}�E�X�Œ͂܂�Ă���

	Obj();
	void init();
	void update();
	void disp();

};

// �R���X�g���N�^ �I�[�o�[���[�h
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

// �����o�ϐ��̕\��
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

	// 0:���݂��ĂȂ��@1:���݂��Ċ������@2:���݂��ĂĊ������ĂȂ�
	if (stat == 2) {
		return;
	}


	{
		// acc:�����x
		acc.x = 0.0f;
		acc.y = ACC_G;	//�d�͉����x�i�l�͓K���ŗǂ��j

		if (link0)	//�eOBJ������?
		{
			// ������1���obj�ւ̃x�N�g��
			Vec2	_dist = link0->pos - pos;
			// ���̃x�N�g���̑傫��
			double	_len = _dist.length();

			// �eobj�Ƃ̋������W�������iLENGTH_LINK�j���傫�����
			// �����x�����Z
			if (_len > LENGTH_LINK)
			{
				// �x�N�g��(_dist)���W����������̑���
				_dist = _dist * (_len - LENGTH_LINK);

				_dist /= LENGTH_LINK;

				// K_HOOK:�΂˒萔�i�L�тɂ����j
				// ����(m)���傫���ق�_dist���|���������x(acc)�̑����͌���
				Vec2 add_acc = _dist * K_HOOK / m;
				acc += add_acc;
			}
		}
		if (link1)	//�qOBJ������?
		{
			// �����̂P����obj�ւ̃x�N�g��
			Vec2	_dist = link1->pos - pos;
			// ���̃x�N�g���̑傫��
			double	_len = _dist.length();

			if (_len > LENGTH_LINK)
			{
				_dist = _dist * (_len - LENGTH_LINK) / LENGTH_LINK;
				acc += _dist * K_HOOK / m;
			}
		}

		vel += acc;			//���x+=�����x

		vel -= vel * K_V_RES;	//�S����R
								// (���̂��ߖT�̗��̂��������邱�Ƃɂ���Ď󂯂锽��p)

		pos += vel;			//�ʒu+=���x
	}

}

inline void Obj::disp()
{
	double	x = pos.x + SX_SCREEN / 2;
	double	y = pos.y + SY_SCREEN / 2;

	sphere.DrawExtendSprite(x - r, y - r, x + r, y + r);
	sphere.Draw();

	//link�̕\��
	if (link0)
	{
		double	xl = link0->pos.x + SX_SCREEN / 2;
		double	yl = link0->pos.y + SY_SCREEN / 2;
		line.DrawLine(x, y, xl, yl);//����`��
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

