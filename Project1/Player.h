#pragma once
#include <NY_Object3DMgr.h>
#include <Sprite.h>
#include <NY_Camera.h>
#include <vector>
#include <array>
#include <memory>

#include "Bullet.h"

class Player
{
public:
	//�v���C���[�̕\���I�u�W�F�N�g
	Object3d* object;

	//UI���\�[�X
	Sprite s_hpFont;
	Sprite s_hpBar;
	Sprite s_CtrlImage;
	Sprite s_sight;

	//�e�֌W
	Model3D bulletModel;
	//std::vector<Bullet> bullets;
	std::array<Bullet, 25> bullets;

	//�e��p�����[�^
	RVector3 pos;	//���W
	RVector3 rot;		//��]
	RVector3 angleVec;	//��]���x
	RVector3 speedVec;		//���x�x�N�g��
	RVector3 speedScale;	//���x�X�P�[�����O
	RVector3 playerLookVec;	//�����x�N�g��
	RVector3 bVec;			//�e�����x�N�g��
	RVector3 lVec;			//�����x�N�g��
	RV3Colider::Ray ray;

	RV3Colider::Rv3AABB boxAABB;

	//�����蔻��
	RV3Colider::Sphere bodyColider;

	int hitpoint = 10;

	Player();
	~Player();

	void CameraMove();

	void PlayerMove();

	void Shot();

	void BulletManagement();
	//�e������
	
	
public:
	//������
	void Init();
	void Reset();
	//�X�V
	void Update();
	//�`��
	void Draw();
	void UiDraw();
	void DebugDraw();
	//�I��
	void Finalize();

};

