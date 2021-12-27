#include "Platformer_SelectMenu.h"

cSelectMenu::cSelectMenu(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* cursor)
{
	sprBackGround = background;
	sprCursor = cursor;
}

bool cSelectMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	// Handle input
	if (gfx->GetKey(olc::DOWN).bReleased || controller->GetButton(DOWN).bPressed)
	{
		nPlayerChoice++;
		if (nPlayerChoice > 3) nPlayerChoice = 0;
	}

	if (gfx->GetKey(olc::UP).bReleased || controller->GetButton(UP).bPressed)
	{
		nPlayerChoice--;
		if (nPlayerChoice < 0) nPlayerChoice = 3;
	}

	// Draw choices and cursor
	gfx->DrawString(160, 30, "Play", olc::WHITE, 5);
	gfx->DrawString(160, 70, "Controls", olc::WHITE, 5);
	gfx->DrawString(160, 110, "Credits", olc::WHITE, 5);
	gfx->DrawString(160, 150, "Quit", olc::WHITE, 5);

	int ncursorY;
	if (nPlayerChoice == 0) ncursorY = 32;
	if (nPlayerChoice == 1) ncursorY = 72;
	if (nPlayerChoice == 2) ncursorY = 112;
	if (nPlayerChoice == 3) ncursorY = 152;

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawSprite(120, ncursorY, sprCursor);
	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

int cSelectMenu::GetPlayerChoice()
{
	return nPlayerChoice;
}
