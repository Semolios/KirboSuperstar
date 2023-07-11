#include "Platformer_Manual.h"

cManual::cManual(OneLoneCoder_Platformer* engine, olc::Sprite* vacuum, olc::Sprite* launchJesusCross, olc::Sprite* jesusCross, olc::Sprite* slapAttack, olc::Sprite* jump, olc::Sprite* fly, olc::Sprite* cameraDown)
{
    sprVacuum = vacuum;
    sprLaunchJesusCross = launchJesusCross;
    sprJesusCross = jesusCross;
    sprSlapAttack = slapAttack;
    sprJump = jump;
    sprFly = fly;
    sprCameraDown = cameraDown;
}

bool cManual::Update(OneLoneCoder_Platformer* engine, float fElapsedTime)
{
    engine->SetPixelMode(olc::Pixel::ALPHA);

    engine->DrawSprite(0, 0, engine->GetLoadedSprite("ControlsMenuBckGrd"));

    switch (nPage)
    {
        case 0: PrintPage0(engine); break;
        case 1: PrintPage1(engine); break;
        case 2: PrintPage2(engine); break;
    }

    engine->DrawKirboString(400, 440, "press space or   to continue");
    engine->DrawSprite(593, 438, engine->GetLoadedSprite("controllerA"));

    engine->SetPixelMode(olc::Pixel::NORMAL);
    return true;
}

void cManual::SetPage(int page)
{
    nPage = page;
}

bool cManual::IsLastPage()
{
    return nPage == 2;
}

void cManual::NextPage()
{
    nPage++;
}

void cManual::PrintPage0(OneLoneCoder_Platformer* engine)
{
    // Vacuum Attack
    engine->DrawSprite(70, 150, sprVacuum);
    engine->DrawKirboString(50, 320, "press 1 or ");
    engine->DrawKirboString(50, 340, "to use the vacuum attack");
    engine->DrawSprite(195, 320, engine->GetLoadedSprite("controllerX"));

    // Jesus Cross Attack
    engine->DrawSprite(400, 150, sprLaunchJesusCross);
    engine->DrawSprite(630, 130, sprJesusCross);
    engine->DrawKirboString(400, 320, "press 2 or ");
    engine->DrawKirboString(400, 340, "to launch a jesus cross");
    engine->DrawSprite(545, 320, engine->GetLoadedSprite("controllerY"));
}

void cManual::PrintPage1(OneLoneCoder_Platformer* engine)
{
    // Slap Attack
    engine->DrawSprite(70, 150, sprSlapAttack);
    engine->DrawKirboString(50, 320, "press 3 or ");
    engine->DrawKirboString(50, 340, "to use the slap attack");
    engine->DrawSprite(195, 320, engine->GetLoadedSprite("controllerB"));

    // Jump
    engine->DrawSprite(400, 150, sprJump);
    engine->DrawKirboString(400, 320, "press space or ");
    engine->DrawKirboString(400, 340, "to jump and double jump");
    engine->DrawSprite(597, 320, engine->GetLoadedSprite("controllerA"));
}

void cManual::PrintPage2(OneLoneCoder_Platformer* engine)
{
    // Fly
    engine->DrawSprite(70, 150, sprFly);
    engine->DrawKirboString(50, 320, "press up to fly");

    // Lower the camera
    engine->DrawSprite(450, 200, sprCameraDown);
    engine->DrawKirboString(400, 320, "press down");
    engine->DrawKirboString(400, 340, "to lower the camera height");
}
