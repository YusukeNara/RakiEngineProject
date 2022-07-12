#include "Player.h"

Player::Player()
{
	Init();
}

Player::~Player()
{
	Finalize();
}

void Player::Init()
{
	//キューブ読み込み
	object = LoadModel_FBXFile("cube");

	//パラメータ初期化
	pos.zero();
	pos.y = 5.0f;
	rot.zero();
	speedVec.zero();

	speedScale = RVector3(1, 1, 1);

	campos.zero();
}

void Player::Update()
{


	//入力に応じてパラメーター更新
	speedVec.zero();
	if (Input::isKey(DIK_W)) { speedVec.z = 1.0f; }
	if (Input::isKey(DIK_S)) { speedVec.z = -1.0f; }
	if (Input::isKey(DIK_A)) { speedVec.x = -1.0f; }
	if (Input::isKey(DIK_D)) { speedVec.x = 1.0f; }

	pos += speedVec;

	angleVec.zero();
	if (Input::isKey(DIK_RIGHT)) { angleVec.y = 1.0f; }
	if (Input::isKey(DIK_LEFT)) { angleVec.y = -1.0f; }

	rot += angleVec;

	if (rot.y > 360.0f) { rot.y -= 360.0f; }
	if (rot.y < 0.0f) { rot.y += 360.0f; }
	if (rot.x > 360.0f) { rot.x -= 360.0f; }
	if (rot.x < 0.0f) { rot.x += 360.0f; }

	//アフィン変換情報更新
	object->SetAffineParam(RVector3(0.2, 0.2, 0.2),
		rot,
		pos);
}

void Player::Draw()
{
	object->DrawObject();
}

void Player::Finalize()
{
	if (object != nullptr) { DeleteObject(object); }
}
