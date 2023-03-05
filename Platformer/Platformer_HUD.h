#ifndef DEF_HUD
#define DEF_HUD

#pragma once

#include "Platformer_DynamicCreature.h"

class cHUD
{
public:
	cHUD();

	void HealthBar(olc::PixelGameEngine* gfx, olc::Decal* healthBar);
	void HealthPoints(olc::PixelGameEngine* gfx, olc::Decal* healthPoint, int health);
	void BossHealthBar(olc::PixelGameEngine* gfx, olc::Decal* bossHealthBar, std::vector<cDynamicCreature*> vecEnnemies);
	void DamageBoost(olc::PixelGameEngine* gfx, olc::Decal* tileMapSheet);
	void DefenseBoost(olc::PixelGameEngine* gfx, olc::Decal* tileMapSheet);

private:
	int nFirstHealthPointPosX = 13;
	int nHealthPointPosY = 14;
	int nBossHealthPosX = 663;
	int nBossHealthPosY = 30;
	int nBossMaxHealth = 100;
	int nBossHealthHeight = 25;
	int nBossHealthBarPosX = 650;
	int nBossHealthBarPosY = 0;
	int nBuffsPosX = 150;
	int nTileWidth = 64;
	int nTileHeight = 64;
};

#endif // !DEF_HUD
