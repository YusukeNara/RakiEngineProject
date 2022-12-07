#include "ConflictAndRepulsionTask.h"

#include <Raki_imguiMgr.h>


void ConflictAndRepulsionTask::Init()
{

	circle.Create(TexManager::LoadTexture("Resources/effect1.png"));

	b.center = RVector3(300, 400, 0);

	a.r = 50;
	b.r = 50;
}

void ConflictAndRepulsionTask::Update()
{
	if (Input::isKey(DIK_SPACE)) {
		a.v0.x =  5.0f;
		b.v0.x =  -4.0f;
		//‰‘¬“x
		a.v = a.v0;
		b.v = b.v0;

		a.m = 0.5f;
		b.m = 0.5f;

		a.center = RVector3(100, 300, 0);
		b.center = RVector3(800, 300, 0);

	}

	a.center += a.v;
	b.center += b.v;


	if (isHitCircle(a, b)) {
		//‰^“®—Ê•Û‘¶‚Ì–@‘¥‚ÌŒöŽ®‚©‚çv2'‚ðÁ‹Ž‚µAa‚Ì‘¬“x‚ð‹‚ß‚é
		float av1 = ((a.m - b.m) * a.v0.x + (2 * b.m * b.v0.x)) / (a.m + b.m);
		//‹t‚à‹‚ß‚é
		float bv1 = ((b.m - a.m) * b.v0.x + (2 * a.m * a.v0.x)) / (a.m + b.m);
		//’µ‚Ë•Ô‚èŒW”‚ð“ü‚ê‚Ä‘¬“x‚ð‘ã“ü
		a.v.x = av1 * (e * e);
		b.v.x = bv1 * (e * e);
	}

}

void ConflictAndRepulsionTask::Draw()
{
	SpriteManager::Get()->SetCommonBeginDraw();

	circle.DrawExtendSprite(a.center.x - a.r, a.center.y - a.r, a.center.x + a.r, a.center.y + a.r);
	circle.DrawExtendSprite(b.center.x - b.r, b.center.y - b.r, b.center.x + b.r, b.center.y + b.r);

	circle.Draw();

	ImguiMgr::Get()->StartDrawImgui("status", 100, 100);

	ImGui::Text("circle1 m : %.3f  v : %.3f v0 : %.3f", a.m, a.v.x, a.v0.x);
	ImGui::Text("circle2 m : %.3f  v : %.3f v0 : %.3f", b.m, b.v.x, b.v0.x);
	ImGui::Text("V0 all : %.3f", a.v0.x + b.v0.x);
	ImGui::Text("v all : %.3f", a.v.x + b.v.x);
	ImGui::SliderFloat("rebound coefficient (e)", &e, 0, 1);

	ImguiMgr::Get()->EndDrawImgui();

}

bool ConflictAndRepulsionTask::isHitPointCircle(RVector3 center, float r, RVector3 ppos, RVector3& coliPos)
{
	return false;
}

bool ConflictAndRepulsionTask::isHitCircle(Circle a, Circle b)
{
	float d = distance(a.center, b.center);
	return d < a.r + b.r;
}
