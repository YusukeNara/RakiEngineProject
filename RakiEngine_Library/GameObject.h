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
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Finalize() {};
	//�ǂݍ��ݐ�p
	virtual void Load() {};

	/// ���z�֐�
	//�Փˎ��R�[���o�b�N
	virtual void OnCollision(const ColliderInfo *info){}


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
	RVector3 rot;
	RVector3 scale;

private:




};