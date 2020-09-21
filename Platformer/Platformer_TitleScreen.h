#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class Platformer_Engine;

class cTitleScreen
{
public:
	cTitleScreen(olc::Sprite* background);

public:
	olc::Sprite* sprBackGround;
	static Platformer_Engine* g_engine;
public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
};

