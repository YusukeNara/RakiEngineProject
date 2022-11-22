#include "Player.h"

Player::Player()
{
	Init();
	boxAABB = RV3Colider::Rv3AABB(RVector3(-20, -20, -20), RVector3(20, 20, 20), RVector3(25, 20, 25));
}

Player::~Player()
{
	Finalize();
}

void Player::CameraMove()
{
	RVector3 camOffsetVec = { 0,10,50 };
	//��������̂̓v���C���[�Ȃ̂Ńv���C���[�̍��W�������_
	//��]�p�̓v���C���[�I�u�W�F�N�g��rot���g�p(x���ɂ��Ă�80 ~ -80�̊�)
	//XY���̉�]�s������߁A�I�t�Z�b�g����]������
	
	DirectX::XMMATRIX rotX, rotY, rotM;
	rotX = XMMatrixIdentity();
	rotY = XMMatrixIdentity();
	rotM = XMMatrixIdentity();
	rotX = XMMatrixRotationX(XMConvertToRadians(rot.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(rot.y));
	rotM *= rotX;
	rotM *= rotY;
	DirectX::XMVECTOR v0 = { camOffsetVec.x,camOffsetVec.y,camOffsetVec.z,0 };
	DirectX::XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	RVector3 eyeVec = { v.m128_f32[0],v.m128_f32[1],v.m128_f32[2] };

	//�J�������W��ݒ�
	RVector3 eye = pos + eyeVec;
	RVector3 target = pos;
	RVector3 up = { 0,1,0 };
	NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);
	
	//�x�N�g�����v�Z
	float d = 0;
	RVector3 hitPos;
	if (RV3Colider::ColisionRayToAABB(ray, boxAABB, &d, &hitPos)) {
		bVec = (pos - hitPos).norm();
	}
	else {
		RVector3 farPos = RV3Colider::CalcScreen2World(DirectX::XMFLOAT2{ 1280.0f / 2,720.0f / 4 }, 1.0f);
		bVec = (pos - farPos).norm();
	}

	//�e�x�N�g���̓v���C���[�̍��W�ƃJ�����ŉ��_��P�ʃx�N�g���ɂ��ċ��߂�

	//�����x�N�g����XZ���ʂ̂ݍl�����邽�߁A�J������]�s���Y���𗘗p����
	//�����̌�����Y����]�s��������āA�����x�N�g�����Z�o
	DirectX::XMVECTOR localLookV = { 0,0,10,0 };
	DirectX::XMVECTOR lookV = XMVector3TransformNormal(localLookV, rotY);
	lVec = { lookV.m128_f32[0],lookV.m128_f32[1],lookV.m128_f32[2] };
	lVec = lVec.norm();

	ray.dir = bVec;
	ray.start = eye;

}

void Player::PlayerMove()
{
	//��b���x
	float speedScale = 0.0f;
	RVector3 speedtempF(0,0,0);//�O�㑬�x
	RVector3 speedtempT(0,0,0);//���E���x
	//�X�v�����g�{��
	float sprintScale = 1.0f;
	if (Input::isXpadButtonPushing(XPAD_BUTTON_LSTICK) || Input::isKey(DIK_LSHIFT)) { sprintScale = 1.7f; }

	//���͂ɉ����Čv�Z��������x�N�g�����قȂ�
	if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_UP) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_DOWN) || Input::isKey(DIK_W) || Input::isKey(DIK_S)) {


		if (Input::isKey(DIK_W)) {
			speedScale = -1.0f;
		}
		else if (Input::isKey(DIK_S)) {
			speedScale = 1.0f;
		}
		else {
			speedScale = -Input::GetXpadLStickTilt().y_rate;
		}

		speedtempF += (lVec * speedScale) * sprintScale;
		speedtempF.y = 0;
	}
	if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_LEFT) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_RIGHT) || Input::isKey(DIK_A) || Input::isKey(DIK_D)) {
		RVector3 rotateVec;
		speedScale = 0.0f;
		if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_LEFT)) {
			speedScale = Input::GetXpadLStickTilt().x_rate;
			rotateVec = { -lVec.z,lVec.y,lVec.x };
		}
		else if (Input::isKey(DIK_A)) {
			speedScale = 1.0f;
			rotateVec = { -lVec.z,lVec.y,lVec.x };
		}
		else if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_RIGHT)) {
			speedScale = -Input::GetXpadLStickTilt().x_rate;
			rotateVec = { lVec.z,lVec.y,-lVec.x };
		}
		else if(DIK_D) {
			speedScale = -1.0f;
			rotateVec = { lVec.z,lVec.y,-lVec.x };
		}
		speedtempT += (rotateVec * speedScale);
	}

	pos += (speedtempF + speedtempT);

	angleVec.zero();
	
	angleVec.y = Input::GetXpadRStickTilt().x_rate;
	angleVec.x = Input::GetXpadRStickTilt().y_rate;

	//angleVec.y = Input::getMouseVelocity().x * 0.2;
	//angleVec.x = Input::getMouseVelocity().y * 0.2;

	rot += angleVec;
	if (rot.y > 360.0f) { rot.y -= 360.0f; }
	if (rot.y < 0.0f) { rot.y += 360.0f; }
	if (rot.x > 80.0f) { rot.x = 80.0f; }
	if (rot.x < -80.0f) { rot.x = -70.0f; }

	//�͈͓��ɔ[�߂�
	if (pos.x > 400.0f) { pos.x = 400.0f; }
	if (pos.x < -400.0f) { pos.x = -400.0f; }
	if (pos.z > 400.0f) { pos.z = 400.0f; }
	if (pos.z < -400.0f) { pos.z = -400.0f; }

	//�A�t�B���ϊ����X�V
	object3d->SetAffineParam(RVector3(0.07f, 0.07f, 0.07f),
		rot,
		pos);

	bodyColider.center = pos;
}

void Player::Shot()
{
	if (Input::isXpadButtonPushTrigger(XPAD_TRIGGER_RT) || Input::isMouseClickTrigger(MOUSE_L)) {
		for (int i = 0; i < bullets.size(); i++) {
			if (!bullets[i].isAlive) {
				bullets[i].Fire(pos, -bVec, 6.0f, 10.0f, bulletModel);
				break;
			}
		}
	}
}

void Player::BulletManagement()
{
	if (bullets.size() <= 0) { return; }

	for (auto b = bullets.begin(); b != bullets.end();) {
		if (b->CheckDistance()) {
			b->Finalize();
			b->isAlive = false;
		}
		else {
			b->Update();
		}
		b++;
	}

	//��苗���ŏ���
	

}



void Player::Init()
{
	//�I�u�W�F�N�g�ǂݍ���
	object3d.reset(LoadModel_FBXFile("cube"));
	object3d->SetAffineParamScale(RVector3(0.3f, 0.5f, 0.3f));
	
	bulletModel = std::make_shared<Model3D>();
	bulletModel->LoadObjModel("Sphere");

	//�X�v���C�g����
	s_hpBar.Create(TexManager::LoadTexture("Resources/healthBar.png"));
	s_hpFont.Create(TexManager::LoadTexture("Resources/healthText.png"));
	s_CtrlImage.Create(TexManager::LoadTexture("Resources/Stick.png"));
	s_sight.Create(TexManager::LoadTexture("Resources/Sight.png"));



	//�e������
	for (auto &b : bullets) {
		b.Init();
	}

	//�p�����[�^������
	pos.zero();
	pos.y = 5.0f;
	rot.zero();
	speedVec.zero();
	bodyColider.rad = 5.0f;

	speedScale = RVector3(1, 1, 1);

	hitpoint = 10;
}

void Player::Reset()
{
	//�p�����[�^������
	pos.zero();
	pos.y = 5.0f;
	rot.zero();
	speedVec.zero();
	bodyColider.rad = 5.0f;

	speedScale = RVector3(1, 1, 1);

	hitpoint = 10;
}

void Player::Update()
{
	if (hitpoint <= 0) {
		return;
	}

	PlayerMove();

	CameraMove();

	Shot();

	BulletManagement();
}

void Player::Draw()
{
	object3d->DrawObject();

	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].isAlive) {
			bullets[i].Draw();
		}
	}

	for (auto& b : bullets) {
		if (b.isAlive) {
			b.Draw();
		}
	}
}

void Player::UiDraw()
{
	//2dUI�̕`��

	s_hpFont.DrawSprite(0, 0);
	for (int i = 0; i < hitpoint; i++) {
		s_hpBar.DrawSprite(120.0f + float(48 * i), 0);
	}
	s_CtrlImage.DrawSprite(1056.0f, 558.0f);
	float cx = 1280.0f / 2.0f;
	float cy = 720.0f / 4.0f;
	s_sight.DrawExtendSprite(cx - 48.0f, cy - 48.0f, cx + 48.0f, cy + 48.0f);

	s_hpFont.Draw();
	s_hpBar.Draw();
	s_CtrlImage.Draw();
	s_sight.Draw();

}

void Player::Finalize()
{
	for (auto& b : bullets) {
		b.Finalize();
	}
}

void Player::Load()
{
}

void Player::OnCollision(ColliderInfo* info)
{
}
