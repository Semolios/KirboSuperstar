#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Spline.h"
#include "Platformer_Animator.h"

class cWorldMap
{
public:
	cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background);

private:
	int nSelectedLevel = 0;

public:
	static cAnimator* animPlayer;
	cSpline path;
	olc::Sprite* sprBackGround;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
	int GetSelectedLevel();
};

