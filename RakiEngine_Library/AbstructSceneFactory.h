#pragma once

#include "SceneBase.h"

//FactoryMethod�̗p�V�[���Ǘ���

//�V�[�����쐬���钊�ۃN���X�i�V�[���쐬�H��̊T�O�j
class AbstructSceneFactory 
{
public:
	/// <summary>
	/// �V�[�������֐��i�������z�֐��j
	/// </summary>
	/// <returns>���������V�[���̃|�C���^</returns>
	virtual SceneBase *CreateScene() = 0;


};

