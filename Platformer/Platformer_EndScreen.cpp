#include "Platformer_EndScreen.h"

cEndScreen::cEndScreen(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;
}

bool cEndScreen::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	return false;
}
