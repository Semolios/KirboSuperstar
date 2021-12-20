#ifndef DEF_SELECT_MENU
#define DEF_SELECT_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Zix_PGE_Controller.h"

class cSelectMenu
{
public:
	cSelectMenu(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* cursor);

private:
	int nPlayerChoice = 0;

	olc::Sprite* sprBackGround;
	olc::Sprite* sprCursor;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller);
	int GetPlayerChoice();
};

#endif // !DEF_SELECT_MENU
