#include "Platformer_PauseMenu.h"

cPauseMenu::cPauseMenu(olc::Sprite* background, olc::Sprite* cursor)
{
	sprBackGround = background;
	sprCursor = cursor;
}

bool cPauseMenu::Update(OneLoneCoder_Platformer* engine, float fElapsedTime, ControllerManager* controller)
{
	engine->DrawSprite(0, 0, sprBackGround);

	// Handle input
	if (engine->GetKey(olc::DOWN).bReleased || controller->GetButton(DOWN).bPressed)
	{
		engine->PlaySample("menuBip");
		nPlayerChoice++;
		if (nPlayerChoice >= 2) nPlayerChoice = 0;
	}

	if (engine->GetKey(olc::UP).bReleased || controller->GetButton(UP).bPressed)
	{
		engine->PlaySample("menuBip");
		nPlayerChoice--;
		if (nPlayerChoice < 0) nPlayerChoice = 1;
	}

	// Draw choices and cursor
	engine->SetPixelMode(olc::Pixel::ALPHA);

	engine->DrawKirboString(160, 370, "Continue",	   2);
	engine->DrawKirboString(160, 410, "Return to map", 2);

	engine->DrawSprite(120, nPlayerChoice == 0 ? 372 : 412, sprCursor);
	engine->SetPixelMode(olc::Pixel::NORMAL);

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
