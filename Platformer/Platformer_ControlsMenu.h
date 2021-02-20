#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class cControlsMenu
{
public:
	cControlsMenu(olc::PixelGameEngine* gfx, olc::Sprite* background);

private:
	olc::Sprite* sprBackGround;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};
