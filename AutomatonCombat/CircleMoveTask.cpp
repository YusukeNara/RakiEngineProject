#include "CircleMoveTask.h"

void CircleMoveTask::Init()
{
	for (int i = 0; i < 3; i++) {
		circles[i].zero();
		r[i] = 50 + 50 * i;
		radSpeed[i] = 0.2 + 0.2 * i;
	}
	frame = 0;
	t = 0.0f;


	circle.Create(TexManager::LoadTexture("Resources/effect1.png"));
}

void CircleMoveTask::Update()
{
	frame++;
	if (frame % 6 == 0) {
		t += 0.1f;
	}

	for (int i = 0; i < 3; i++) {
		circles[i] = CircleMove(r[i], radSpeed[i], t, initr, circles[i]);
	}


}

void CircleMoveTask::Draw()
{
	SpriteManager::Get()->SetCommonBeginDraw();

	for (int i = 0; i < 3; i++) {
		circle.DrawExtendSprite(OFFSET_X + circles[i].x - SIZE, OFFSET_Y + circles[i].y - SIZE, OFFSET_X + circles[i].x + SIZE, OFFSET_Y + circles[i].y + SIZE);
	}
	circle.DrawExtendSprite(OFFSET_X - SIZE, OFFSET_Y - SIZE, OFFSET_X + SIZE, OFFSET_Y + SIZE);

	circle.Draw();
}

RVector3 CircleMoveTask::CircleMove(float r, float rs, float t, float initRad, RVector3& v)
{
	RVector3 result = v;

	result.x = r * cosf(rs * t + initRad);
	result.y = r * sinf(rs * t + initRad);

	return result;
}
