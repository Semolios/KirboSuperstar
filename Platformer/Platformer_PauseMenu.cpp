#include "Platformer_PauseMenu.h"

cPauseMenu::cPauseMenu(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* cursor)
{
	sprBackGround = background;
	sprCursor = cursor;
}

bool cPauseMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	// Handle input
	if (gfx->GetKey(olc::DOWN).bReleased)
	{
		nPlayerChoice++;
		if (nPlayerChoice >= 2) nPlayerChoice = 0;
	}

	if (gfx->GetKey(olc::UP).bReleased)
	{
		nPlayerChoice--;
		if (nPlayerChoice < 0) nPlayerChoice = 1;
	}

	// Draw choices and cursor
	gfx->DrawString(160, 370, "Continue", olc::WHITE, 5);
	gfx->DrawString(160, 410, "Return to map", olc::WHITE, 5);

	gfx->SetPixelMode(olc::Pixel::ALPHA);
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
