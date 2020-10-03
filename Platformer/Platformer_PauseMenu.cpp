#include "Platformer_PauseMenu.h"

cPauseMenu::cPauseMenu(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;
}

bool cPauseMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	return true;
}
