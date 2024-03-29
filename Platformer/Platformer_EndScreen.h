#ifndef DEF_END_SCREEN
#define DEF_END_SCREEN

#pragma once

#include "Platformer_Animator.h"

class cEndScreen
{
public:
	cEndScreen(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim);

	bool Update(olc::PixelGameEngine* gfx , float fElapsedTime, olc::Sprite* playerSprite, olc::Decal* playerDecal);

private:
	static cAnimator* animPlayer;
	olc::Sprite* sprBackGround;
};

#endif // !DEF_END_SCREEN
