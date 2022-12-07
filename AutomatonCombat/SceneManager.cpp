#include "SceneManager.h"
#include "GameScene.h"
#include "Title.h"

//Raki_DX12B         *SceneManager::dx12b  = nullptr;
//NY_Object3DManager *SceneManager::objmgr = nullptr;
//SpriteManager      *SceneManager::spmgr  = nullptr;

SceneManager::SceneManager() :mNextScene(eScene_None) {

    //�e�V�[���̃C���X�^���X����
    titleScene = (BaseScene *) new Title(this);

    nowScene = titleScene;
    Initialize();
}

void SceneManager::Initialize()
{
    //�V�[���̏�����
	nowScene->Initialize();
}

void SceneManager::Finalize()
{
    nowScene->Finalize();
}

void SceneManager::Update()
{
    if (mNextScene != eScene_None) {    //���̃V�[�����Z�b�g����Ă�����
        nowScene->Finalize();//���݂̃V�[���̏I�����������s
        
        switch (mNextScene) {       //�V�[���ɂ���ď����𕪊�
        case eScene_Title:        //���̉�ʂ����j���[�Ȃ�
            nowScene = titleScene;
            break;//�ȉ���
        case eScene_Game:
            break;
        }
        mNextScene = eScene_None;    //���̃V�[�������N���A
        nowScene->Initialize();    //�V�[����������
    }

    nowScene->Update(); //�V�[���̍X�V
}

void SceneManager::Draw()
{
    //���݃V�[���̕`�揈�������s
    nowScene->Draw();
}

void SceneManager::ChangeScene(eScene NextScene)
{
    //���̃V�[���ԍ����Z�b�g
    mNextScene = NextScene;
}
