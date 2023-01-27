#include "Player.h"

Player::Player()
{
	boxAABB = RV3Colider::Rv3AABB(RVector3(-20, -20, -20), RVector3(20, 20, 20), RVector3(25, 20, 25));
}

Player::~Player()
{
	Finalize();
}

void Player::CameraMove()
{
	RVector3 camOffsetVec = { 0,60,60 };
	//注視するのはプレイヤーなのでプレイヤーの座標が注視点
	//回転角はプレイヤーオブジェクトのrotを使用(x軸については80 ~ -80の間)
	//XY軸の回転行列を求め、オフセットを回転させる
	
	DirectX::XMMATRIX rotX, rotY, rotM;
	rotX = XMMatrixIdentity();
	rotY = XMMatrixIdentity();
	rotM = XMMatrixIdentity();
	rotX = XMMatrixRotationX(XMConvertToRadians(l_rot.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(l_rot.y));
	rotM *= rotX;
	rotM *= rotY;
	DirectX::XMVECTOR v0 = { camOffsetVec.x,camOffsetVec.y,camOffsetVec.z,0 };
	DirectX::XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	RVector3 eyeVec = { v.m128_f32[0],v.m128_f32[1],v.m128_f32[2] };

	//カメラ座標を設定
	RVector3 eye = pos + eyeVec;
	RVector3 target = pos;
	RVector3 up = { 0,1,0 };
	NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);
	
	//ベクトルを計算
	float d = 0;
	RVector3 hitPos;
	if (RV3Colider::ColisionRayToAABB(ray, boxAABB, &d, &hitPos)) {
		bVec = (pos - hitPos).norm();
	}
	else {
		RVector3 farPos = RV3Colider::CalcScreen2World(DirectX::XMFLOAT2{ 1280.0f / 2,720.0f / 4 }, 1.0f);
		bVec = (pos - farPos).norm();
	}

	//弾ベクトルはプレイヤーの座標とカメラ最遠点を単位ベクトルにして求める

	//向きベクトルはXZ平面のみ考慮するため、カメラ回転行列のY軸を利用する
	//初期の向きにY軸回転行列をかけて、向きベクトルを算出
	DirectX::XMVECTOR localLookV = { 0,0,10,0 };
	DirectX::XMVECTOR lookV = XMVector3TransformNormal(localLookV, rotY);
	lVec = { lookV.m128_f32[0],lookV.m128_f32[1],lookV.m128_f32[2] };
	lVec = lVec.norm();

	ray.dir = bVec;
	ray.start = eye;

}

void Player::PlayerMove()
{
	//基礎速度
	float speedScale = 0.0f;
	RVector3 speedtempF(0,0,0);//前後速度
	RVector3 speedtempT(0,0,0);//左右速度
	//スプリント倍率
	float sprintScale = 1.0f;
	if (Input::isXpadButtonPushing(XPAD_BUTTON_LSTICK) || Input::isKey(DIK_LSHIFT)) { sprintScale = 1.7f; }

	//入力に応じて計算する方向ベクトルが異なる
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

	l_rot += angleVec;
	if (l_rot.y > 360.0f) { l_rot.y -= 360.0f; }
	if (l_rot.y < 0.0f) { l_rot.y += 360.0f; }
	if (l_rot.x > 80.0f) { l_rot.x = 80.0f; }
	if (l_rot.x < -80.0f) { l_rot.x = -70.0f; }

	//範囲内に納める
	if (pos.x > 500.0f) { pos.x = 500.0f; }
	if (pos.x < -500.0f) { pos.x = -500.0f; }
	if (pos.z > 500.0f) { pos.z = 500.0f; }
	if (pos.z < -500.0f) { pos.z = -500.0f; }

	rot.y = l_rot.y - 180.0f;

	//アフィン変換情報更新
	object3d->SetAffineParam(RVector3(10.0f, 10.0f, 10.0f),
		rot,
		pos);

	bodyColider.center = pos;
}

void Player::Shot()
{
	if (Input::isXpadButtonPushTrigger(XPAD_TRIGGER_RT) || Input::isMouseClickTrigger(MOUSE_L)) {
		if (nowBullet > 0) {
			for (int i = 0; i < bullets.size(); i++) {
				if (!bullets[i].isAlive) {
					bullets[i].Fire(pos, -bVec, 20.0f, 5.0f, bulletModel);
					nowBullet--;
					break;
				}
			}

		}
	}
}

void Player::Reload()
{
	nowBullet = MAX_BULLET;
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

	//一定距離で消失
	

}

void Player::HealthManagement()
{
	//赤アルファ
	damagedAlpha = 1.0f - float(hitpoint / float(HITPOINT_MAX));

	//ダメージ受けたときの回復時間
	if (healCoolTime > 0) {
		healCoolTime--;
	}
	else {
		//ダメージ受けてたら
		if (hitpoint < HITPOINT_MAX) {
			//回復
			hitpoint++;
		}
	}


}



void Player::Init()
{
	//オブジェクト読み込み
	object3d.reset(LoadModel_FBXFile("FiringRifle"));
	//object3d.reset(LoadModel_FBXFile("cube"));
	
	bulletModel = std::make_shared<Model3D>();
	bulletModel->LoadObjModel("Sphere");

	//スプライト生成
	s_hpBar.Create(TexManager::LoadTexture("Resources/healthBar.png"));
	s_hpFont.Create(TexManager::LoadTexture("Resources/healthText.png"));
	s_CtrlImage.Create(TexManager::LoadTexture("Resources/Stick.png"));
	s_sight.Create(TexManager::LoadTexture("Resources/Sight.png"));
	s_numFont.CreateAndSetDivisionUVOffsets(10, 5, 2, 64, 64, TexManager::LoadTexture("Resources/zenNum.png"));
	s_noammo.Create(TexManager::LoadTexture("Resources/noammo.png"));

	warningSprite.Create(TexManager::LoadTexture("Resources/warningEffect.png"));
	damagedAlpha = 0.0f;

	//弾初期化
	for (auto &b : bullets) {
		b.Init();
	}

	//パラメータ初期化
	pos.zero();
	pos.z = 400.0f;
	pos.y = 4.0f;
	rot.zero();
	speedVec.zero();
	bodyColider.rad = 5.0f;

	speedScale = RVector3(1, 1, 1);

	hitpoint = HITPOINT_MAX;

	nowBullet = MAX_BULLET;

	l_rot = rot;

	object3d->PlayAnimation();
}

void Player::Reset()
{
	//パラメータ初期化
	pos.zero();
	pos.y = 5.0f;
	rot.zero();
	speedVec.zero();
	bodyColider.rad = 5.0f;

	speedScale = RVector3(1, 1, 1);

	hitpoint = HITPOINT_MAX;
}

void Player::Update()
{
	if (hitpoint <= 0) {
		return;
	}

	//if (damagedAlpha > 0.0f) {
	//	damagedAlpha -= 0.02f;
	//}

	PlayerMove();

	CameraMove();

	Shot();

	BulletManagement();

	HealthManagement();
}

void Player::Draw()
{
	object3d->DrawObject();

	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].isAlive) {
			bullets[i].Draw();
		}
	}


}

void Player::UiDraw()
{
	//2dUIの描画

	s_hpFont.DrawSprite(0, 0);
	s_CtrlImage.DrawSprite(32.0f, 558.0f);
	float cx = 1280.0f / 2.0f;
	float cy = 720.0f / 4.0f;

	if (nowBullet > 0) {
		s_sight.DrawExtendSprite(cx - 48.0f, cy - 48.0f, cx + 48.0f, cy + 48.0f);
		s_sight.Draw();

	}
	else {
		s_noammo.DrawExtendSprite(cx - 128.0f, cy - 32.0f, cx + 128.0f, cy + 32.0f);
		s_noammo.Draw();
	}


	s_hpFont.Draw();
	s_CtrlImage.Draw();
	
	s_numFont.uvOffsetHandle = (nowBullet % (int)pow(10, 1 + 1)) / (int)pow(10, 1);
	s_numFont.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.f - 64.f, 720.f);
	s_numFont.uvOffsetHandle = (nowBullet % (int)pow(10, 0 + 1)) / (int)pow(10, 0);
	s_numFont.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.f, 720.f);
	s_numFont.Draw();

	warningSprite.SetSpriteColorParam(0.8f, 0.2f, 0.2f, damagedAlpha);
	warningSprite.DrawExtendSprite(0.0f, -48.0f * redEffectScale, 1280.0f, 48.0f * redEffectScale);
	warningSprite.DrawExtendSprite(0.0f, 720.0f - 48.0f * redEffectScale, 1280.0f, 720.0f + 48.0f * redEffectScale);
	warningSprite.Draw();

	warningSprite.SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
}

void Player::ParticleDraw()
{
	for (auto& b : bullets) {
		if (b.isAlive) {
			b.Draw();
		}
	}
}

void Player::DebugDraw()
{
	ImguiMgr::Get()->StartDrawImgui("PlayerStatus", 100, 100);

	ImGui::Text("hitPoint : %d ammo : %d", hitpoint, nowBullet);

	ImguiMgr::Get()->EndDrawImgui();
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

void Player::OnCollision(const ColliderInfo* info)
{
}

void Player::PlayerDamaged(int damage)
{
	//体力減少
	hitpoint -= damage;
	//体力回復待機時間
	healCoolTime = HEAL_COOLTIME;
	//赤表示スケーリング更新
	redEffectScale = float(HITPOINT_MAX / float(hitpoint) - 0.99f);
	//赤アルファ
	damagedAlpha = 1.0f - float(hitpoint / float(HITPOINT_MAX));
	
}
