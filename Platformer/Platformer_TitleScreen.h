#ifndef DEF_TITLE_SCREEN
#define DEF_TITLE_SCREEN

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class cTitleScreen
{
public:
	cTitleScreen(olc::PixelGameEngine* gfx, olc::Sprite* background);

	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
	void DrawNewStar(olc::PixelGameEngine* gfx, float px, float py, float theta);

private:
	// Constant values
	const int cnTileWidth = 64;
	const int cnTileHeight = 64;
	const float cfStarsRotSpeed = 8.0f;

	const float p1x = 120.0f, p1y = 435.0f;
	const float p2x = 600.0f, p2y = 435.0f;

	olc::Sprite* sprBackGround;

	olc::Sprite* sprstars;
	float fTheta;
};

#endif // !DEF_TITLE_SCREEN
