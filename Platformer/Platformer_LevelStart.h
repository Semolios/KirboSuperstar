#ifndef DEF_LEVELSTART
#define DEF_LEVELSTART

#pragma once

#include "Platformer_Animator.h"

class cLevelStart
{
public:
	cLevelStart(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim);

	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);

private:
	static cAnimator* animPlayer;
	olc::Sprite* sprBackGround;
};

#endif // !DEF_LEVELSTART
