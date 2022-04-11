#pragma once
#include "SpriteManager.h"
#include "NY_Camera.h"
#include "TexManager.h"

class Sprite
{
public:
	//�X�v���C�g�ꖇ�̏��
	SpriteData spdata;
	//�A�j���[�V�����f�[�^
	uvAnimData *animData;

public:
	//�X�v���C�g�����i���̐������ɋN���ł����H�j(�f�o�C�X�A�X�v���C�g�T�C�Y�A���\�[�X�ԍ��A�A���J�[�|�C���g�A�X�v���C�g�}�l�[�W���[�|�C���^)
	Sprite(XMFLOAT2 size,UINT resourceID, bool adjustResourceFlag = false,XMFLOAT2 anchor = { 0.0f,0.0f })
	{
		//�X�v���C�g����
		CreateSprite(size, anchor, resourceID, adjustResourceFlag, nullptr);
	};
	//�����Ȃ��i�ʂŏ������j
	Sprite() {};

	//�X�v���C�g������(�f�o�C�X�A�X�v���C�g�T�C�Y�A�A���J�[�|�C���g�A�g�p���郊�\�[�X�ԍ��A���\�[�X�����t���O)
	void CreateSprite(XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag, uvAnimData *animData = nullptr);

	/// <summary>
	/// �`��̂��ƂɂȂ�X�v���C�g�f�[�^����
	/// </summary>
	/// <param name="resourceID">�e�N�X�`���n���h��</param>
	/// <param name="sizeX">�`��T�C�Y</param>
	/// <param name="sizeY">�`��T�C�Y</param>
	/// <param name="reserveDrawCount">�`�悷�鐔</param>
	void Create(UINT resourceID, float sizeX, float sizeY);

	//�T�C�Y�ύX
	void ResizeSprite(XMFLOAT2 newsize);

	//�X�v���C�g�X�V�i�G���W���ŏ���ɂ��j
	void UpdateSprite();

	//���_�C���X�^���X�f�[�^���X�V
	void InstanceUpdate();

	//�X�v���C�g�`��
	void Draw();

	void DrawSprite(float posX, float posY);

	void DrawExtendSprite(float x1, float y1, float x2, float y2);

	//�}���`�p�X���ʕ`��
	void DrawMPRender();

private:

	UINT sizeInsVB;

	UINT instanceDrawCount;

	bool isVertexBufferNeedResize();

	//���_�o�b�t�@�̃T�C�Y�ύX�i�C���X�^���V���O�p�o�b�t�@�j
	void ResizeVertexInstanceBuffer(UINT newWidthSize);

};

///�V�A�X�v���C�g�����̎d��
///
/// �e�N�X�`���ԍ����w�肵�č쐬
/// ��
/// Update�͂��Ȃ�
/// ��
/// Draw()�ȊO�̕`��֐����Ă�
/// ��
/// ����ȏ�`�悵�Ȃ��^�C�~���O��Draw()�����s
/// 
/// �`��֐�
/// DrawSprite(x,y); �����A�ʏ�`��
/// 


