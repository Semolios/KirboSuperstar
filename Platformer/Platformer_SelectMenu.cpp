#include "Platformer_SelectMenu.h"

cSelectMenu::cSelectMenu(olc::Sprite* background, olc::Sprite* cursor)
{
	sprBackGround = background;
	sprCursor = cursor;
}

bool cSelectMenu::Update(OneLoneCoder_Platformer* engine, float fElapsedTime, ControllerManager* controller)
{
	engine->DrawSprite(0, 0, sprBackGround);

	// Handle input
	if (engine->GetKey(olc::DOWN).bReleased || controller->GetButton(DOWN).bPressed)
	{
		engine->PlaySample("menuBip");
		nPlayerChoice++;
		if (nPlayerChoice > 3) nPlayerChoice = 0;
	}

	if (engine->GetKey(olc::UP).bReleased || controller->GetButton(UP).bPressed)
	{
		engine->PlaySample("menuBip");
		nPlayerChoice--;
		if (nPlayerChoice < 0) nPlayerChoice = 3;
	}

	// Draw choices and cursor
	engine->SetPixelMode(olc::Pixel::ALPHA);

	if (!bInOptionsSubmenu)
	{
		engine->DrawKirboString(160, 30,  "Play",	  2);
		engine->DrawKirboString(160, 70,  "Settings", 2);
		engine->DrawKirboString(160, 110, "Credits",  2);
		engine->DrawKirboString(160, 150, "Quit",	  2);
	}
	else
	{
		engine->DrawKirboString(160, 30,  "Controls", 2);
		engine->DrawKirboString(160, 70,  "Sounds",	  2);
		engine->DrawKirboString(160, 110, "Screen",   2);
		engine->DrawKirboString(160, 150, "Back",	  2);
	}

	int ncursorY;
	if (nPlayerChoice == 0) ncursorY = 32;
	if (nPlayerChoice == 1) ncursorY = 72;
	if (nPlayerChoice == 2) ncursorY = 112;
	if (nPlayerChoice == 3) ncursorY = 152;

	engine->DrawSprite(120, ncursorY, sprCursor);
	engine->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

int cSelectMenu::GetPlayerChoice()
{
	return nPlayerChoice;
}

bool cSelectMenu::IsInOptionSubmenu()
{
	return bInOptionsSubmenu;
}

void cSelectMenu::SetInOptionSubmenu(bool submenu)
{
	bInOptionsSubmenu = submenu;
	nPlayerChoice = 0;
}
