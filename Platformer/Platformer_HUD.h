#ifndef DEF_HUD
#define DEF_HUD

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_DynamicCreature.h"

class cHUD
{
public:
	cHUD();

public:
	void HealthBar(olc::PixelGameEngine* gfx, olc::Sprite* healthBar);
	void HealthPoints(olc::PixelGameEngine* gfx, olc::Sprite* healthPoint, int health);
	void BossHealthBar(olc::PixelGameEngine* gfx, olc::Sprite* bossHealthBar, std::vector<cDynamicCreature*> vecEnnemies);
};

#endif // !DEF_HUD