#ifndef DEF_LEVELSTART
#define DEF_LEVELSTART

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Animator.h"

class cLevelStart
{
public:
	cLevelStart(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim);

private:
	static cAnimator* animPlayer;
	olc::Sprite* sprBackGround;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};

#endif // !DEF_LEVELSTART
