#ifndef DEF_MANUAL
#define DEF_MANUAL

#pragma once

#include "Platformer_Engine.h"

class cManual
{
public:
    cManual(OneLoneCoder_Platformer* engine,
            olc::Sprite* vacuum,
            olc::Sprite* launchJesusCross,
            olc::Sprite* jesusCross,
            olc::Sprite* slapAttack,
            olc::Sprite* jump,
            olc::Sprite* fly,
            olc::Sprite* cameraDown);

    bool Update(OneLoneCoder_Platformer* engine, float fElapsedTime);
    void SetPage(int page);
    bool IsLastPage();
    void NextPage();
    void PrintPage0(OneLoneCoder_Platformer* engine);
    void PrintPage1(OneLoneCoder_Platformer* engine);
    void PrintPage2(OneLoneCoder_Platformer* engine);
    void PrintPage3(OneLoneCoder_Platformer* engine);
    void PrintPage4(OneLoneCoder_Platformer* engine);
    void PrintPage5(OneLoneCoder_Platformer* engine);

private:
    olc::Sprite* sprVacuum;
    olc::Sprite* sprLaunchJesusCross;
    olc::Sprite* sprJesusCross;
    olc::Sprite* sprSlapAttack;
    olc::Sprite* sprJump;
    olc::Sprite* sprFly;
    olc::Sprite* sprCameraDown;

    int nPage;
};

#endif // !DEF_MANUAL
