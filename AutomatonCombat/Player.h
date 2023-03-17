#pragma once
#include <NY_Object3DMgr.h>
#include <Sprite.h>
#include <NY_Camera.h>
#include <GameObject.h>
#include <vector>
#include <array>
#include <memory>
#include <ParticleManager.h>

#include "Bullet.h"

class Player : public GameObject
{
public:
	//UI���\�[�X
	Sprite s_hpFont;
	Sprite s_hpBar;
	Sprite s_CtrlImage;
	Sprite s_sight;
	Sprite s_numFont;
	Sprite s_noammo;

	//�e�֌W
	std::shared_ptr<Model3D> bulletModel;
	//std::vector<Bullet> bullets;
	std::array<Bullet, 25> bullets;
	//�c�e���ƍő�e��
	const int MAX_BULLET = 25;
	int nowBullet = 25;

	//�e��p�����[�^
	RVector3 angleVec;		//��]���x
	RVector3 speedVec;		//���x�x�N�g��
	RVector3 speedScale;	//���x�X�P�[�����O
	RVector3 playerLookVec;	//�����x�N�g��
	RVector3 bVec;			//�e�����x�N�g��
	RVector3 lVec;			//�����x�N�g��
	RV3Colider::Ray ray;

	RV3Colider::Rv3AABB boxAABB;

	//�����蔻��
	RV3Colider::Sphere bodyColider;

	Sprite warningSprite;

	float damagedAlpha;

	//�̗͊Ǘ���
	int hitpoint = 100;
	const int HITPOINT_MAX = 100;
	int healCoolTime = 0;
	const int HEAL_COOLTIME = 180;
	float redEffectScale = 0.0f;

	RVector3 l_rot;

	//�J�����⊮�p
	struct CameraMoveParam
	{
		float camMovePower;
		const int camMovePowerMaxFrame = 15;
		int camMovePowerFrame;

		CameraMoveParam() = default;
		~CameraMoveParam() = default;
	};
	CameraMoveParam camMoveParam;


	Player();
	~Player();

	void CameraMove();

	void PlayerMove();

	//�ˌ�
	void Shot();

	//�đ��U
	void Reload();

	void BulletManagement();
	//�e������
	
	void HealthManagement();
	
public:
	//������
	void Init()		override;
	void Reset();
	void Update()	override;
	void Draw()		override;
	void UiDraw();
	void ParticleDraw();
	void DebugDraw();
	//�I��
	void Finalize() override;

	void Load() override;
	void OnCollision(const ColliderInfo* info) override;

	//�_���[�W����
	void PlayerDamaged(int damage);

};

