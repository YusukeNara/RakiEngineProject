#pragma once
#include "NY_Object3D.h"

struct ParticleBase {
	Object3d *obj; //���I�u�W�F�N�g
	XMFLOAT3 vec;      //�����x�N�g��
	int startFlame;  //�J�n�t���[��
	int endFlame;    //�I���t���[��
	bool  isAlive;     //�`��t���O
};
