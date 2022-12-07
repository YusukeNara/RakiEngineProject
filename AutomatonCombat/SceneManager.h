#pragma once
#include "ISceneChanger.h"
#include "BaseScene.h"
#include "Raki_DX12B.h"
#include "Raki_WinAPI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

/// <summary>
/// �V�[���Ǘ��N���X�@�d�l
/// <para>�ڂ����� dixq.net/g/sp_06.html ���Q�Ƃ��邱��</para>
/// <para>�E�e�V�[���̃N���X�̃C���X�^���X�𐶐����A���݂̃V�[���̏����̂ݎ��s����</para>
/// <para>�E�e�V�[���̃N���X�̃I�[�o�[���C�h�֐��́A������Raki_DX12B�N���X���擾����</para>
/// <para>�E�g�p���́A���̃N���X�̃C���X�^���X��main�Ő����AInit�AUpdate�ADraw�A(�K�v�ɉ�����Finalize)�����s�����OK</para>
/// <para>�E�V�[���ύX���s���́A�e�N���X�̍X�V������ChangeScene(�����Fenum�Œ�`�����V�[���ԍ�)�֐������s���邱�ƂŁA</para>
/// <para>�@�O�V�[���̏I�����������s�A���V�[���̏������J�n����</para>
/// </summary>
class SceneManager : public ISceneChanger ,Task
{
private:
    //�e�V�[���C���X�^���X
	BaseScene *titleScene;
    BaseScene *gameScene;

    BaseScene *nowScene;

	eScene mNextScene;    //���̃V�[���Ǘ��ϐ�

public:
    //�R���X�^���N�^
    SceneManager();
    ~SceneManager() {
        delete titleScene;
        titleScene = nullptr;
        delete gameScene;
        gameScene = nullptr;
    }
    void Initialize() override;//������
    void Finalize() override;//�I������
    void Update() override;//�X�V
    void Draw() override;//�`��

    // ���� nextScene �ɃV�[����ύX����
    void ChangeScene(eScene NextScene) override;

};

