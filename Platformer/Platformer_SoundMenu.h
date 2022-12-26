#ifndef DEF_SOUND_MENU
#define DEF_SOUND_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Zix_PGE_Controller.h"

class OneLoneCoder_Platformer;

class cSoundMenu
{
public:
	cSoundMenu(olc::PixelGameEngine* gfx, olc::Sprite* soundMenuNoSound, olc::Sprite* soundMenuLow, olc::Sprite* soundMenuMid, olc::Sprite* soundMenuHigh);

	static OneLoneCoder_Platformer* engine;

	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller);
	int GetSoundVolume();

private:
	int nSoundVolume = 10;

	olc::Sprite* sprSoundMenuNoSound;
	olc::Sprite* sprSoundMenuLow;
	olc::Sprite* sprSoundMenuMid;
	olc::Sprite* sprSoundMenuHigh;
};

#endif // !DEF_SOUND_MENU
