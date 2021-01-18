#include "Platformer_SelectMenu.h"

cSelectMenu::cSelectMenu(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* cursor)
{
	sprBackGround = background;
	sprCursor = cursor;
}

bool cSelectMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	// Handle input
	if (gfx->GetKey(olc::DOWN).bReleased)
	{
		nPlayerChoice++;
		if (nPlayerChoice > 2) nPlayerChoice = 0;
	}

	if (gfx->GetKey(olc::UP).bReleased)
	{
		nPlayerChoice--;
		if (nPlayerChoice < 0) nPlayerChoice = 2;
	}

	// Draw choices and cursor
	gfx->DrawString(160, 30, "Play", olc::WHITE, 5);
	gfx->DrawString(160, 70, "Controls", olc::WHITE, 5);
	gfx->DrawString(160, 110, "Quit", olc::WHITE, 5);

	int ncursorY;
	if (nPlayerChoice == 0) ncursorY = 32;
	if (nPlayerChoice == 1) ncursorY = 72;
	if (nPlayerChoice == 2) ncursorY = 112;

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawSprite(120, ncursorY, sprCursor);
	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

int cSelectMenu::GetPlayerChoice()
{
	return nPlayerChoice;
}
