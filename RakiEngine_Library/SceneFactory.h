#include "AbstructSceneFactory.h"

#pragma once

//FactoryMethod�̗p�V�[���Ǘ���
//�V�[���쐬�H��̋�̕���
class SceneFactory : public AbstructSceneFactory
{
public:
	/// <summary>
	/// �V�[�������֐��i�������z�֐��j
	/// </summary>
	/// <returns>���������V�[���̃|�C���^</returns>
	SceneBase *CreateScene() override;


};

