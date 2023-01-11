#include "WaveDirections.h"

void WaveDirections::Init()
{
	NumSprite.CreateAndSetDivisionUVOffsets(10, 5, 2, 64, 64, TexManager::LoadTexture("Resources/zenNum.png"));
	goSprite.Create(TexManager::LoadTexture("Resources/GO.png"));
	wm_pos3 = wn_down;
	wm_pos2 = wn_down;
	wm_pos1 = wn_down;
	wm_posGo = wn_down;
}

void WaveDirections::Update()
{

	PlayNextWaveDirectionUpdate();
}

void WaveDirections::Draw()
{

	if (wm_isActive) {
		NumSprite.uvOffsetHandle = 3;
		NumSprite.DrawExtendSprite(wm_pos3.x - 32.f, wm_pos3.y - 32.f, wm_pos3.x + 32.f, wm_pos3.y + 32.f);
		NumSprite.uvOffsetHandle = 2;
		NumSprite.DrawExtendSprite(wm_pos2.x - 32.f, wm_pos2.y - 32.f, wm_pos2.x + 32.f, wm_pos2.y + 32.f);
		NumSprite.uvOffsetHandle = 1;
		NumSprite.DrawExtendSprite(wm_pos1.x - 32.f, wm_pos1.y - 32.f, wm_pos1.x + 32.f, wm_pos1.y + 32.f);
		goSprite.DrawExtendSprite(wm_posGo.x - 64.f, wm_posGo.y - 32.f, wm_posGo.x + 64.f, wm_posGo.y + 32.f);
	}

	NumSprite.Draw();
	goSprite.Draw();
}

void WaveDirections::PlayNextWaveDir()
{
	wm_isActive = true;
}

void WaveDirections::PlayNextWaveDirectionUpdate()
{
	if (wm_isActive == false) {
		return;
	}

	wm_frame++;
	
	float rate = (float)wm_frame / 60.0f;


	
	
	
	if (wm_frame <= 60) {
		wm_pos3 = Rv3Ease::InQuad(wn_down, wn_center, rate);
	}
	if (wm_frame >= 60) {
		wm_pos3 = Rv3Ease::InQuad(wn_center, wn_up, rate-1.f);
		wm_pos2 = Rv3Ease::InQuad(wn_down, wn_center, rate - 1.f);
	}
	if (wm_frame >= 120) {
		wm_pos2 = Rv3Ease::InQuad(wn_center, wn_up, rate-2.f);
		wm_pos1 = Rv3Ease::InQuad(wn_down, wn_center, rate - 2.f);
	}
	if (wm_frame >= 180) {
		wm_pos1 = Rv3Ease::InQuad(wn_center, wn_up, rate-3.f);
		wm_posGo = Rv3Ease::InQuad(wn_down, wn_center, rate - 3.f);
	}
	if (wm_frame >= 240) {
		wm_posGo = Rv3Ease::InQuad(wn_center, wn_up, rate-4.f);
	}
	if (wm_frame >= 300) {
		wm_pos3 = wn_down;
		wm_pos2 = wn_down;
		wm_pos1 = wn_down;
		wm_posGo = wn_down;
		wm_frame = 0;
		wm_isActive = false;
	}

}
