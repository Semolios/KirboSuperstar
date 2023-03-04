#ifndef DEF_PAUSE_MENU
#define DEF_PAUSE_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Zix_PGE_Controller.h"
#include "Platformer_Engine.h"

class cPauseMenu
{
public:
	cPauseMenu(olc::Sprite* background, olc::Sprite* cursor);

	bool Update(OneLoneCoder_Platformer* engine, float fElapsedTime, ControllerManager* controller);
	int GetPlayerChoice();
	void SetPlayerChoice(int choice);

private:
	int nPlayerChoice = 0;

	olc::Sprite* sprBackGround;
	olc::Sprite* sprCursor;
};

#endif // !DEF_PAUSE_MENU
