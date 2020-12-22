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
	float fDynWidth;
	float fDynHeight;
	float fSpriteW;
	float fSpriteH;
	float fSpriteOffsetX;
	float fSpriteOffsetY;

	float px, py;
	float vx, vy;
	int nDamage;
	int nCornerSpr = 0;
	bool bSolidVsMap;
	bool bSolidVsDyn;
	bool bSolidVsDynInitValue;
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
	virtual void Update(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
	{
	}
	virtual void OnInteract(cDynamic* player = nullptr)
	{
	}

	static Platformer_Engine* g_engine;
};

