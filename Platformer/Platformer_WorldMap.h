#ifndef DEF_WORLD_MAP
#define DEF_WORLD_MAP

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Spline.h"
#include "Platformer_Animator.h"

class cWorldMap
{
public:
	cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim);

private:
	const int cnKirboTileWidth = 192;
	const int cnKirboTileHeight = 192;

	int nSelectedLevel = 0;
	int nUnlockedLevels = 0;

	static cAnimator* animPlayer;
	cSpline path;
	olc::Sprite* sprBackGround;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
	int GetSelectedLevel();
	void SetUnlockedLevel(int unlockedLevels);
};

#endif // !DEF_WORLD_MAP