#pragma once

#include <string>
#include <memory>

#include "NY_Object3D.h"
#include "Colider.h"


class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	//�A�v���P�[�V�����g�p�������z�֐�
	virtual void Init()		= 0;
	virtual void Update()	= 0;
	virtual void Draw()		= 0;
	virtual void Finalize()	= 0;
	//�ǂݍ��ݐ�p
	virtual void Load() {};

	/// ���z�֐�
	//�Փˎ��R�[���o�b�N
	virtual void OnCollision(ColliderInfo* info){}


protected:
	//���ׂẴI�u�W�F�N�g�ɕK�v�Ȃ��̂̂�
	//���̑��̃I�u�W�F�N�g�͔h���N���X�Œ�`���邱��



public:
	//�`��I�u�W�F�N�g
	std::shared_ptr<Object3d>		object3d;

	//�Փ˔���
	std::shared_ptr<BaseCollider>	collider;

	//�ϊ����
	RVector3 pos;

private:




};