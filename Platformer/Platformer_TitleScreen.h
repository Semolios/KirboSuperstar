#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Spline.h"

class cTitleScreen
{
public:
	cTitleScreen(olc::PixelGameEngine* gfx, olc::Sprite* background);

private:
	// Constant values
	const int cnTileWidth = 64;
	const int cnTileHeight = 64;
	const float cfStarsMovSpeed = 600.0f;
	const float cfStarsRotSpeed = 8.0f;

	olc::Sprite* sprBackGround;

	cSpline path;
	olc::Sprite* sprstars;
	float fMarker1;
	float fMarker2;
	float fTheta;

public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
	void DrawNewStar(olc::PixelGameEngine* gfx, float fMarker);
	void DrawStar(olc::PixelGameEngine* gfx, olc::GFX2D::Transform2D& t);
};