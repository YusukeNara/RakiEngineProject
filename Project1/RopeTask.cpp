#include "RopeTask.h"

void RopeTask::Init()
{
	// �L�����N�^�[�̏����ʒu���Z�b�g
	for (int i = 0; i < NUM_OBJ; i++)
	{
		a[i].init();
	}

	a[0].stat = 2;	//�ŏ��̂P�ڂ͐Î~
	a[0].pos.y = -400;

	_idx_obj = 0;
}

void RopeTask::Update()
{
	// ���Z�b�g?
	if (Input::isKey(DIK_R))
	{
		for (int i = 0; i < NUM_OBJ; i++)
		{
			a[i].stat = 0;
			a[i].link0 = NULL;
			a[i].link1 = NULL;
			a[i].grabbed = 0;
		}
		a[0].stat = 2;	//�ŏ��̂P�͐Î~
		_idx_obj = 0;
	}

	// ���̒ǉ�?
	if (Input::isKeyTrigger(DIK_UP))
	{
		if (_idx_obj < NUM_OBJ)
		{
			_idx_obj++;
			a[_idx_obj].stat = 1;
			a[_idx_obj].vel.clear(0);
			a[_idx_obj].pos = a[_idx_obj - 1].pos;
			// �����̐eobj���P���obj��
			a[_idx_obj].link0 = &a[_idx_obj - 1];
			// �P���obj�̎qobj��������
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
