#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Animator.h"

class cTransition
{
public:
	static cAnimator* animPlayer;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};