#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Animator.h"

class cTransition
{
public:
	cTransition(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim);

private:
	static cAnimator* animPlayer;
	olc::Sprite* sprBackGround;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};