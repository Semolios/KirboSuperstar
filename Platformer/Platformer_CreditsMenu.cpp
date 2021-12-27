#include "Platformer_CreditsMenu.h"

cCreditsMenu::cCreditsMenu(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;
}

bool cCreditsMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	return true;
}
