#ifndef DEF_SCREEN_RESOLUTION_MENU
#define DEF_SCREEN_RESOLUTION_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Zix_PGE_Controller.h"

class OneLoneCoder_Platformer;

class cScreenModeMenu
{
public:
	cScreenModeMenu(olc::PixelGameEngine* gfx, olc::Sprite* screenModeMenu, olc::Sprite* rightArrow, olc::Sprite* leftArrow);

	static OneLoneCoder_Platformer* engine;

	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller);
	void UpdateScreenMode();

private:
	const int cnScreenModeTextX = 150;
	const int cnScreenModeTextY = 150;
	const int cnTextX = 520;
	const int cnTextY1 = 250;
	const int cnTextY2 = 270;
	const int cnTextY3 = 290;
	const int cnLeftArrowX = 450;
	const int cnRightArrowX = 750;

	bool bWindowed;

	olc::Sprite* sprScreenModeMenu;
	olc::Sprite* sprRightArrow;
	olc::Sprite* sprLeftArrow;
};

#endif // !DEF_SCREEN_RESOLUTION_MENU
