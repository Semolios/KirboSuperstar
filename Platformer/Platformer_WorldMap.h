#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Animator.h"

class cWorldMap
{
public:
	cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background);

public:
	// Constant values

	static cAnimator* animPlayer;

	olc::Sprite* sprBackGround;
public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};

