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
	RVector3 camOffsetVec = { 0,20,100 };
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
	RVector3 farPos = RV3Colider::CalcScreen2World(DirectX::XMFLOAT2{ 1280.0f / 2,720.0f / 2 }, 1.0f);
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
	//���͂ɉ����ăp�����[�^�[�X�V
	//speedVec.zero();
	//if (Input::isKey(DIK_W)) { speedVec.z = -1.0f; }
	//if (Input::isKey(DIK_S)) { speedVec.z = 1.0f; }
	//if (Input::isKey(DIK_A)) { speedVec.x = 1.0f; }
	//if (Input::isKey(DIK_D)) { speedVec.x = -1.0f; }

	//��b���x
	float speedScale = 0.0f;
	RVector3 speedtempF(0,0,0);//�O�㑬�x
	RVector3 speedtempT(0,0,0);//���E���x
	//���͂ɉ����Čv�Z��������x�N�g�����قȂ�
	if (Input::isKey(DIK_W) || Input::isKey(DIK_S)) {
		if (Input::isKey(DIK_W)) { speedScale = -1.0f; }
		if (Input::isKey(DIK_S)) { speedScale = 1.0f; }
		speedtempF += (lVec * speedScale);
		speedtempF.y = 0;
	}
	if (Input::isKey(DIK_A) || Input::isKey(DIK_D)) {
		RVector3 rotateVec;
		speedScale = 0.0f;
		if (Input::isKey(DIK_A)) { 
			speedScale = -1.0f; 
			rotateVec = { -lVec.z,lVec.y,lVec.x };
		}
		if (Input::isKey(DIK_D)) { 
			speedScale = -1.0f;
			rotateVec = { lVec.z,lVec.y,-lVec.x };
		}
		speedtempT += (rotateVec * speedScale);
	}
	pos += (speedtempF + speedtempT);

	angleVec.zero();
	if (Input::isKey(DIK_RIGHT)) { angleVec.y = 1.0f; }
	if (Input::isKey(DIK_LEFT)) { angleVec.y = -1.0f; }
	if (Input::isKey(DIK_UP)) { angleVec.x = 1.0f; }
	if (Input::isKey(DIK_DOWN)) { angleVec.x = -1.0f; }

	rot += angleVec;
	if (rot.y > 360.0f) { rot.y -= 360.0f; }
	if (rot.y < 0.0f) { rot.y += 360.0f; }
	if (rot.x > 80.0f) { rot.x = 80.0f; }
	if (rot.x < -80.0f) { rot.x = -70.0f; }

	//�A�t�B���ϊ����X�V
	object->SetAffineParam(RVector3(0.07, 0.07, 0.07),
		rot,
		pos);
}

void Player::Init()
{
	//�I�u�W�F�N�g�ǂݍ���
	object = LoadModel_FBXFile("cube");
	object->SetAffineParamScale(RVector3(0.3, 0.5, 0.3));

	//�p�����[�^������
	pos.zero();
	pos.y = 5.0f;
	rot.zero();
	speedVec.zero();

	speedScale = RVector3(1, 1, 1);
}

void Player::Update()
{
	PlayerMove();

	CameraMove();

}

void Player::Draw()
{
	object->DrawObject();
}

void Player::Finalize()
{
	if (object != nullptr) { DeleteObject(object); }
}
