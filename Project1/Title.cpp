#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

#include <AI_BehaviorBaseNode.h>

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {
    
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    circleSprite.Create(TexManager::LoadTexture("Resources/blockParticleTex.png"));
}

//‰Šú‰»
void Title::Initialize() {
    speed = 0.0f;
    x = CIRCLE_LENGHT / 8.0f;
}

void Title::Finalize()
{

}

//XV
void Title::Update() {

    speed += -MASS * G * sinf(x / ROPE_LENGHT);
    x += speed;

    angle = (x / ROPE_LENGHT) + (PI / 2.0f);

    nx = cosf(angle) * ROPE_LENGHT;
    ny = sinf(angle) * ROPE_LENGHT;

}

//•`‰æ
void Title::Draw() {

    //•`‰æ’†SÀ•W
    float posX = nx + axis_x;
    float posY = ny + axis_y;

    SpriteManager::Get()->SetCommonBeginDraw();

    circleSprite.DrawExtendSprite(posX - SIZE, posY - SIZE, posX + SIZE, posY + SIZE);
    circleSprite.Draw();

    ImguiMgr::Get()->StartDrawImgui("furiko_param", 100, 100);

    ImGui::Text("pos : %0.2f %0.2f", posX, posY);
    ImGui::SliderFloat("ROPE_LENGHT", &ROPE_LENGHT, 60.0f, 300.0f);

    ImguiMgr::Get()->EndDrawImgui();

}
