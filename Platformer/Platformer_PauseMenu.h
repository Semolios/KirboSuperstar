#ifndef DEF_PAUSE_MENU
#define DEF_PAUSE_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Zix_PGE_Controller.h"

class cPauseMenu
{
public:
	cPauseMenu(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* cursor);

private:
	int nPlayerChoice = 0;

	olc::Sprite* sprBackGround;
	olc::Sprite* sprCursor;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller);
	int GetPlayerChoice();
	void SetPlayerChoice(int choice);
};

#endif // !DEF_PAUSE_MENU