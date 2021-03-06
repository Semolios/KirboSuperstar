#ifndef DEF_TITLE_SCREEN
#define DEF_TITLE_SCREEN

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class cTitleScreen
{
public:
	cTitleScreen(olc::PixelGameEngine* gfx, olc::Sprite* background);

private:
	// Constant values
	const int cnTileWidth = 64;
	const int cnTileHeight = 64;
	const float cfStarsRotSpeed = 8.0f;

	const float p1x = 140.0f, p1y = 440.0f;
	const float p2x = 620.0f, p2y = 440.0f;

	olc::Sprite* sprBackGround;

	olc::Sprite* sprstars;
	float fTheta;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
	void DrawNewStar(olc::PixelGameEngine* gfx, float px, float py, float theta);
};

#endif // !DEF_TITLE_SCREEN