#pragma once
#include <Sprite.h>
#include <RVector.h>

class WaveDirections
{
public:
	static WaveDirections* Get()
	{
		static WaveDirections ins;
		return &ins;
	}

	//�S���o������
	void Init();

	//���o�X�V
	void Update();

	void Draw();

	void PlayNextWaveDir();

private:

	//����
	Sprite NumSprite;
	//���E�F�[�u�J�ڗp
	Sprite goSprite;
	//
	bool wm_isActive = false;
	//�t���[���Ǘ�
	int wm_frame = 0;

	RVector3 wn_center = { 640.f,360.f,0 };
	RVector3 wn_up = { 640.f,-180.f,0 };
	RVector3 wn_down = { 640.f,+900.f,0 };

	RVector3 wm_pos3;
	RVector3 wm_pos2;
	RVector3 wm_pos1;
	RVector3 wm_posGo;

	float wn_size;
	float wn_MAX_SIZE = 64;

	void PlayNextWaveDirectionUpdate();

};

