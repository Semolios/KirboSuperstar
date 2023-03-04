#ifndef DEF_CREDITS_MENU
#define DEF_CREDITS_MENU

#pragma once

#include "olcPixelGameEngine.h"

class cCreditsMenu
{
public:
	cCreditsMenu(olc::PixelGameEngine* gfx, olc::Sprite* background);

	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);

private:
	olc::Sprite* sprBackGround;
};

#endif // !DEF_CREDITS_MENU
