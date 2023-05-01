#include "Platformer_CreditsMenu.h"

cCreditsMenu::cCreditsMenu(olc::Sprite* background)
{
    sprBackGround = background;
    credits.push_back("musics");
    credits.push_back("");
    credits.push_back("siivagunner");
    credits.push_back("");
    credits.push_back("forest area (ost version) kirby adventure");
    credits.push_back("boss battle kirby super star");
    credits.push_back("fountain of dreams kirby nightmare in dreamland");
    credits.push_back("lose life kirby adventure");
    credits.push_back("green greens kirby super star");
    credits.push_back("vegetable valley kirby adventure");
    credits.push_back("grape garden (ost version) kirby adventure");
    credits.push_back("mt. dedede kirby dream land");
    credits.push_back("invincibility theme kirby adventure");
    credits.push_back("float islands kirby super star");
    credits.push_back("green greens (alternate mix) kirby super star");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("neo midgar");
    credits.push_back("");
    credits.push_back("kirby adventure boss theme sonic the hedgehog version");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("thelegendofrenegade");
    credits.push_back("");
    credits.push_back("kirby super star ultra meta knight theme (sega genesis remix)");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("superrayman001");
    credits.push_back("");
    credits.push_back("battleship halberd (on deck)");
    credits.push_back("kirby super star ultra ost extended");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("giivasunner");
    credits.push_back("");
    credits.push_back("candy mountain kirby super star");
    credits.push_back("dynablade overworld kirby super star");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("jemmysponz");
    credits.push_back("");
    credits.push_back("christmas coconut mall remix");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("sprites");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("level design");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("level art");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("game design");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("programming");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("animations");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("effects design");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("ui design");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("play tests");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("bug tests");
    credits.push_back("");
    credits.push_back("semolios");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("kirbo and the something series");
    credits.push_back("are the properties of terminal montage");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("");
    credits.push_back("thank you for playing");
}

bool cCreditsMenu::Update(OneLoneCoder_Platformer* engine, float fElapsedTime)
{
    engine->PlaySample("creditsMenu", false, true);

    engine->DrawSprite(0, 0, sprBackGround);

    fTextPosTimer += fElapsedTime;

    engine->SetPixelMode(olc::Pixel::ALPHA);

    for (int i = 0; i < credits.size(); i++)
    {
        engine->DrawKirboString(engine->ScreenWidth() / 2, engine->ScreenHeight() - fTextPosTimer * cfTextScrollingSpeed + i * cnKirboCharHeight, credits.at(i), 1, true);
    }

    engine->SetPixelMode(olc::Pixel::NORMAL);

    return true;
}

void cCreditsMenu::ResetTextPosTimer()
{
    fTextPosTimer = 0.0f;
}

bool cCreditsMenu::IsCreditOver()
{
    return fTextPosTimer >= cfCreditsDuration;
}
