#include "RopeTask.h"

void RopeTask::Init()
{
	// キャラクターの初期位置をセット
	for (int i = 0; i < NUM_OBJ; i++)
	{
		a[i].init();
	}

	a[0].stat = 2;	//最初の１個目は静止
	a[0].pos.y = -400;

	_idx_obj = 0;
}

void RopeTask::Update()
{
	// リセット?
	if (Input::isKey(DIK_R))
	{
		for (int i = 0; i < NUM_OBJ; i++)
		{
			a[i].stat = 0;
			a[i].link0 = NULL;
			a[i].link1 = NULL;
			a[i].grabbed = 0;
		}
		a[0].stat = 2;	//最初の１個は静止
		_idx_obj = 0;
	}

	// 物体追加?
	if (Input::isKeyTrigger(DIK_UP))
	{
		if (_idx_obj < NUM_OBJ)
		{
			_idx_obj++;
			a[_idx_obj].stat = 1;
			a[_idx_obj].vel.clear(0);
			a[_idx_obj].pos = a[_idx_obj - 1].pos;
			// 自分の親objを１つ上のobjに
			a[_idx_obj].link0 = &a[_idx_obj - 1];
			// １つ上にobjの子objを自分に
			a[_idx_obj - 1].link1 = &a[_idx_obj];
		}
	}


}

void RopeTask::Draw()
{
	for (int i = 0; i < NUM_OBJ; i++)
	{
		if (a[i].stat == 0)continue;

		a[i].update();
		a[i].disp();
	}
}
