#ifndef DEF_SOUND_MENU
#define DEF_SOUND_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "Zix_PGE_Controller.h"
#include "Platformer_Engine.h"

class cSoundMenu
{
public:
	cSoundMenu(olc::Sprite* soundMenuNoSound,
			   olc::Sprite* soundMenuLow,
			   olc::Sprite* soundMenuMid,
			   olc::Sprite* soundMenuHigh,
			   olc::Sprite* soundMenuMax,
			   olc::Sprite* soundMenuRightArrow,
			   olc::Sprite* soundMenuLeftArrow);

	bool Update(OneLoneCoder_Platformer* engine, float fElapsedTime, ControllerManager* controller);
	int GetSoundVolume();
	void UpdateVolume();

private:
	const int cnVolumeTextX = 245;
	const int cnVolumeTextY = 115;
	const int cnLeftArrowX = 420;
	const int cnRightArrowX = 560;

	int nSoundVolume = 10;

	olc::Sprite* sprSoundMenuNoSound;
	olc::Sprite* sprSoundMenuLow;
	olc::Sprite* sprSoundMenuMid;
	olc::Sprite* sprSoundMenuHigh;
	olc::Sprite* sprSoundMenuMax;
	olc::Sprite* sprSoundMenuRightArrow;
	olc::Sprite* sprSoundMenuLeftArrow;
};

#endif // !DEF_SOUND_MENU
