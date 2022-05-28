#ifndef DEF_END_SCREEN
#define DEF_END_SCREEN

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Animator.h"

class cEndScreen
{
public:
	cEndScreen(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim);

private:
	static cAnimator* animPlayer;
	olc::Sprite* sprBackGround;

public:
	bool Update(olc::PixelGameEngine* gfx , float fElapsedTime);
};

#endif // !DEF_END_SCREEN
