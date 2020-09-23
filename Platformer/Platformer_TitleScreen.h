#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Spline.h"

class Platformer_Engine;

class cTitleScreen
{
public:
	cTitleScreen(olc::PixelGameEngine* gfx, olc::Sprite* background);

public:
	olc::Sprite* sprBackGround;
	static Platformer_Engine* g_engine;

	cSpline path;
	olc::Sprite* sprstars;
	float fMarker1;
	float fMarker2;
	float fTheta;
	int nTileWidth;
	int nTileHeight;
public:
	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime);
	void DrawNewStar(olc::PixelGameEngine* gfx, float fMarker);
	void DrawStar(olc::PixelGameEngine* gfx, olc::GFX2D::Transform2D& t);
};