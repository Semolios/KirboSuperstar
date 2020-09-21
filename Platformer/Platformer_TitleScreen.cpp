#include "Platformer_TitleScreen.h"

cTitleScreen::cTitleScreen(olc::Sprite* background)
{
	sprBackGround = background;
}

bool cTitleScreen::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	return false;
}
