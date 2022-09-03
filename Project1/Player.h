#pragma once
#include <NY_Object3DMgr.h>
#include <NY_Camera.h>

class Player
{
public:
	//�v���C���[�̕\���I�u�W�F�N�g
	Object3d* object;

	//�e��p�����[�^
	RVector3 pos;	//���W
	RVector3 rot;		//��]
	RVector3 angleVec;	//��]���x
	RVector3 speedVec;		//���x�x�N�g��
	RVector3 speedScale;	//���x�X�P�[�����O
	RVector3 playerLookVec;	//�����x�N�g��
	RVector3 bVec;			//�e�����x�N�g��
	RVector3 lVec;			//�����x�N�g��


	Player();
	~Player();

	void CameraMove();

	void PlayerMove();
	
public:
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�I��
	void Finalize();


};

