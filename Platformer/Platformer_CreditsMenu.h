#ifndef DEF_CREDITS_MENU
#define DEF_CREDITS_MENU

#pragma once

#include "Platformer_Engine.h"

class cCreditsMenu
{
public:
    cCreditsMenu(olc::Sprite* background);

    bool Update(OneLoneCoder_Platformer* engine, float fElapsedTime);
    void ResetTextPosTimer();
    bool IsCreditOver();

private:
    const float cfCreditsDuration = 128.0f;
    const float cfTextScrollingSpeed = 20.0f;
    const int   cnKirboCharHeight = 16;
    
    olc::Sprite* sprBackGround;
    std::vector<std::string> credits;
    float fTextPosTimer;
};

#endif // !DEF_CREDITS_MENU
