#ifndef DEF_CREDITS_MENU
#define DEF_CREDITS_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class cCreditsMenu
{
public:
	cCreditsMenu(olc::PixelGameEngine* gfx, olc::Sprite* background);

private:
	olc::Sprite* sprBackGround;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};

#endif // !DEF_CREDITS_MENU
