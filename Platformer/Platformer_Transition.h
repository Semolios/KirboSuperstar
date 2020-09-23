#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Animator.h"

class Platformer_Engine;

class cTransition
{
public:
	static Platformer_Engine* g_engine;
	static cAnimator* animPlayer;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};