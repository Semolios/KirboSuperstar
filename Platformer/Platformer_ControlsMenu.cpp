#include "Platformer_ControlsMenu.h"

cControlsMenu::cControlsMenu(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;
}

bool cControlsMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	return true;
}
