#include "Platformer_PauseMenu.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cPauseMenu::engine = nullptr;

cPauseMenu::cPauseMenu(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* cursor)
{
	sprBackGround = background;
	sprCursor = cursor;
}

bool cPauseMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	// Handle input
	if (gfx->GetKey(olc::DOWN).bReleased || controller->GetButton(DOWN).bPressed)
	{
		engine->PlaySample("menuBip");
		nPlayerChoice++;
		if (nPlayerChoice >= 2) nPlayerChoice = 0;
	}

	if (gfx->GetKey(olc::UP).bReleased || controller->GetButton(UP).bPressed)
	{
		engine->PlaySample("menuBip");
		nPlayerChoice--;
		if (nPlayerChoice < 0) nPlayerChoice = 1;
	}

	// Draw choices and cursor
	gfx->SetPixelMode(olc::Pixel::ALPHA);

	engine->DrawKirboString(160, 370, "Continue",	   2);
	engine->DrawKirboString(160, 410, "Return to map", 2);

	gfx->DrawSprite(120, nPlayerChoice == 0 ? 372 : 412, sprCursor);
	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

int cPauseMenu::GetPlayerChoice()
{
	return nPlayerChoice;
}

void cPauseMenu::SetPlayerChoice(int choice)
{
	nPlayerChoice = choice;
}
