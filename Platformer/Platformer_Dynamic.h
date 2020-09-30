#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class Platformer_Engine;

class cDynamic
{
public:
	cDynamic(std::string n);
	~cDynamic();

public:
	const float nTileSize = 64.0f;

	float px, py;
	float vx, vy;
	bool bSolidVsMap;
	bool bSolidVsDyn;
	bool bFriendly;
	bool bRedundant;
	bool bIsProjectile;
	bool bIsAttackable;
	bool bDead = false;
	std::string sName;

public:

public:
	virtual void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
	{
	}
	virtual void Update(float fElapsedTime, float playerX, float playerY)
	{
	}
	virtual void OnInteract(cDynamic* player = nullptr)
	{
	}

	static Platformer_Engine* g_engine;
};

