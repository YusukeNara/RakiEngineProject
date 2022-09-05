#include "Player.h"

Player::Player()
{
	Init();
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

	//�e�x�N�g���̓v���C���[�̍��W�ƃJ�����ŉ��_��P�ʃx�N�g���ɂ��ċ��߂�
	RVector3 farPos = RV3Colider::CalcScreen2World(DirectX::XMFLOAT2{ 1280.0f / 2,720.0f / 4 }, 1.0f);
	bVec = (pos - farPos).norm();
	//�����x�N�g����XZ���ʂ̂ݍl�����邽�߁A�J������]�s���Y���𗘗p����
	//�����̌�����Y����]�s��������āA�����x�N�g�����Z�o
	DirectX::XMVECTOR localLookV = { 0,0,10,0 };
	DirectX::XMVECTOR lookV = XMVector3TransformNormal(localLookV, rotY);
	lVec = { lookV.m128_f32[0],lookV.m128_f32[1],lookV.m128_f32[2] };
	lVec = lVec.norm();


}

void Player::PlayerMove()
{
	//��b���x
	float speedScale = 0.0f;
	RVector3 speedtempF(0,0,0);//�O�㑬�x
	RVector3 speedtempT(0,0,0);//���E���x
	//�X�v�����g�{��
	float sprintScale = 1.0f;
	if (Input::isXpadButtonPushing(XPAD_BUTTON_LSTICK)) { sprintScale = 1.7f; }

	//���͂ɉ����Čv�Z��������x�N�g�����قȂ�
	if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_UP) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_DOWN)) {

		speedScale = -Input::GetXpadLStickTilt().y_rate;
		speedtempF += (lVec * speedScale) * sprintScale;
		speedtempF.y = 0;
	}
	if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_LEFT) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_RIGHT)) {
		RVector3 rotateVec;
		speedScale = 0.0f;
		if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_LEFT)) {
			speedScale = Input::GetXpadLStickTilt().x_rate;
			rotateVec = { -lVec.z,lVec.y,lVec.x };
		}
		if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_RIGHT)) {
			speedScale = -Input::GetXpadLStickTilt().x_rate;
			rotateVec = { lVec.z,lVec.y,-lVec.x };
		}
		speedtempT += (rotateVec * speedScale);
	}

	pos += (speedtempF + speedtempT);

	angleVec.zero();
	
	angleVec.y = Input::GetXpadRStickTilt().x_rate;
	angleVec.x = Input::GetXpadRStickTilt().y_rate;

	rot += angleVec;
	if (rot.y > 360.0f) { rot.y -= 360.0f; }
	if (rot.y < 0.0f) { rot.y += 360.0f; }
	if (rot.x > 80.0f) { rot.x = 80.0f; }
	if (rot.x < -80.0f) { rot.x = -70.0f; }

	//�͈͓��ɔ[�߂�
	if (pos.x > 250.0f) { pos.x = 250.0f; }
	if (pos.x < -250.0f) { pos.x = -250.0f; }
	if (pos.z > 250.0f) { pos.z = 250.0f; }
	if (pos.z < -250.0f) { pos.z = -250.0f; }

	//�A�t�B���ϊ����X�V
	object->SetAffineParam(RVector3(0.07, 0.07, 0.07),
		rot,
		pos);

	bodyColider.center = pos;
}

void Player::Shot()
{
	if (Input::isXpadButtonPushTrigger(XPAD_TRIGGER_RT)) {
		for (int i = 0; i < bullets.size(); i++) {
			if (!bullets[i].isAlive) {
				bullets[i].Fire(pos, -bVec, 6.0f, 10.0f);
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
	object = LoadModel_FBXFile("cube");
	object->SetAffineParamScale(RVector3(0.3, 0.5, 0.3));

	//�X�v���C�g����
	s_hpBar.Create(TexManager::LoadTexture("Resources/healthBar.png"));
	s_hpFont.Create(TexManager::LoadTexture("Resources/healthText.png"));
	s_CtrlImage.Create(TexManager::LoadTexture("Resources/Stick.png"));
	s_sight.Create(TexManager::LoadTexture("Resources/Sight.png"));

	//�e������
	for (auto &b : bullets) {
		b.Init(pos, pos, bVec, 6.0f, 10.0f);
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
	object->DrawObject();

	for (auto& b : bullets) {
		b.Draw();
	}
}

void Player::UiDraw()
{
	//2dUI�̕`��

	s_hpFont.DrawSprite(0, 0);
	for (int i = 0; i < hitpoint; i++) {
		s_hpBar.DrawSprite(120 + (48 * i), 0);
	}
	s_CtrlImage.DrawSprite(1056, 558);
	float cx = 1280.0f / 2;
	float cy = 720.0f / 4;
	s_sight.DrawExtendSprite(cx - 48, cy - 48, cx + 48, cy + 48);

	s_hpFont.Draw();
	s_hpBar.Draw();
	s_CtrlImage.Draw();
	s_sight.Draw();

}

void Player::Finalize()
{
	if (object != nullptr) { delete object; }
	for (auto& b : bullets) {
		b.Finalize();
	}
}
