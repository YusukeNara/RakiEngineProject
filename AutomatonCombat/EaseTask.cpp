#include "EaseTask.h"

#include <Raki_imguiMgr.h>

void EaseTask::Init()
{
	handle = TexManager::LoadTexture("Resources/grif.png");

	easeWindow.Create(handle);

	for (int i = 0; i < 3; i++) {
		ease[i].zero();
		start[i] = RVector3(-100, 100 + 110 * i, 0);
		target[i] = RVector3(100, 100 + 110 * i, 0);
	}
	frame = 0;
}

void EaseTask::Update()
{
	if (frame < EASE_FRAME) {
		frame++;
		float rate = float(frame) / float(EASE_FRAME);
		for (int i = 0; i < 3; i++) {
			ease[i] = Rv3Ease::OutQuad(start[i], target[i], rate);
		}
	}
}

void EaseTask::Draw()
{
	SpriteManager::Get()->SetCommonBeginDraw();

	for (int i = 0; i < 3; i++) {
		easeWindow.DrawExtendSprite(ease[i].x - width, ease[i].y - height, ease[i].x + width, ease[i].y + height);
	}

	easeWindow.Draw();

	ImguiMgr::Get()->StartDrawImgui("EaseControl", 100, 100);

	if (ImGui::Button("Out")) {
		for (int i = 0; i < 3; i++) {
			ease[i].zero();
			start[i] = RVector3(  100, 100 + 110 * i, 0);
			target[i] = RVector3(-100, 100 + 110 * i, 0);
		}
		frame = 0;
	}

	if (ImGui::Button("In")) {
		for (int i = 0; i < 3; i++) {
			ease[i].zero();
			start[i] = RVector3(-100, 100 + 110 * i, 0);
			target[i] = RVector3(100, 100 + 110 * i, 0);
		}
		frame = 0;
	}

	ImguiMgr::Get()->EndDrawImgui();
	
}
