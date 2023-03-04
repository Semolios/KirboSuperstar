#ifndef DEF_SELECT_MENU
#define DEF_SELECT_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Zix_PGE_Controller.h"
#include "Platformer_Engine.h"

class cSelectMenu
{
public:
	cSelectMenu(olc::Sprite* background, olc::Sprite* cursor);

	bool Update(OneLoneCoder_Platformer* engine, float fElapsedTime, ControllerManager* controller);
	int GetPlayerChoice();
	bool IsInOptionSubmenu();
	void SetInOptionSubmenu(bool submenu);

private:
	bool bInOptionsSubmenu;
	int nPlayerChoice = 0;

	olc::Sprite* sprBackGround;
	olc::Sprite* sprCursor;
};

#endif // !DEF_SELECT_MENU
