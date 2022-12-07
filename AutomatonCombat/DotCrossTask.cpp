#include "DotCrossTask.h"

#include <Raki_imguiMgr.h>

void DotCrossTask::Init()
{
	a.zero();
	b.zero();

	sphere.Create(TexManager::LoadTexture("Resources/Sphere.png"));
	line.Create(TexManager::LoadTexture("Resources/white.png"));

	center = RVector3(1280 / 2, 720 / 2, 0);

	r = NY_random::floatrand_sl(200, 50);
}

void DotCrossTask::Update()
{

}

void DotCrossTask::Draw()
{
	//描画
	if (isHitLineCircle(a, b, center, r)) {
		Sprite::SetSpriteColorParam(1, 0.2, 0.2, 1);
	}
	else {
		Sprite::SetSpriteColorParam(1, 1, 1, 1);
	}
	sphere.DrawExtendSprite(center.x - r, center.y - r, center.x + r, center.y + r);
	sphere.Draw();

	Sprite::SetSpriteColorParam(1, 1, 1, 1);

	line.DrawLine(a.x, a.y, b.x, b.y);
	line.Draw();

	ImguiMgr::Get()->StartDrawImgui("Ctrl", 100, 100);


	ImGui::SliderFloat("line a x", &a.x, 0, 1280);
	ImGui::SliderFloat("line a y", &a.y, 0, 720);
	ImGui::SliderFloat("line b x", &b.x, 0, 1280);
	ImGui::SliderFloat("line b y", &b.y, 0, 720);
	ImGui::SliderFloat("circle r", &r, 10, 360);

	ImguiMgr::Get()->EndDrawImgui();

}

bool DotCrossTask::isHitLineCircle(RVector3 a, RVector3 b, RVector3 center, float r)
{
	//各ベクトル生成
	RVector3 sc = a - center;
	RVector3 ec = b - center;
	RVector3 se = a - b;

	//abベクトル単位化し、他ベクトルと外積計算
	RVector3 n_se = se.norm();
	
	float proj = (sc.x * n_se.y) - (n_se.x * sc.y);

	if (fabs(proj) < r) {
		float d1 = (sc.x * se.x) + (sc.y * se.y);
		float d2 = (ec.x * se.x) + (ec.y * se.y);

		if (d1 * d2 <= 0.0f) {
			return true;
		}
	}
	else if(sc.length() < r || ec.length() < r) {
		return true;
	}

	return false;
}
