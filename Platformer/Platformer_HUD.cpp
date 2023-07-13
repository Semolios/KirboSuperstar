#include "Platformer_HUD.h"

cHUD::cHUD()
{
}

void cHUD::HealthBar(olc::PixelGameEngine* gfx, olc::Decal* healthBar)
{
    gfx->DrawDecal({ 0, 0 }, healthBar);
}

void cHUD::HealthPoints(olc::PixelGameEngine* gfx, olc::Decal* healthPoint, int health)
{
	for (int i = 0; i < health; i++)
	{
		olc::vf2d pos;
		pos.x = nFirstHealthPointPosX + i * (healthPoint->sprite->width + 1);
		pos.y = nHealthPointPosY;
		gfx->DrawDecal(pos, healthPoint);
	}
}

void cHUD::BossHealthBar(olc::PixelGameEngine* gfx, olc::Decal* bossHealthBar, std::vector<cDynamicCreature*> vecEnnemies)
{
	// Health points of the boss
	int nBossHP = 0;
	if (vecEnnemies.size() != 0)
	{
		nBossHP = vecEnnemies[0]->GetHealth();
	}

	// Health
    olc::vf2d pos;
    pos.x = nBossHealthPosX + (nBossMaxHealth - nBossHP);
    pos.y = nBossHealthPosY;
	olc::vf2d size;
	size.x = nBossHP;
	size.y = nBossHealthHeight;
	gfx->FillRectDecal(pos, size, olc::RED);

	// Health bar
	pos.x = nBossHealthBarPosX;
	pos.y = nBossHealthBarPosY;
	gfx->DrawDecal(pos, bossHealthBar);
}

void cHUD::DamageBoost(olc::PixelGameEngine* gfx, olc::Decal* tileMapSheet)
{
	olc::vf2d pos; pos.x = nBuffsPosX; pos.y = 0;
	olc::vf2d sourcePos; sourcePos.x = 0 * nTileWidth; sourcePos.y = 1 * nTileHeight;
	olc::vf2d sourceSize; sourceSize.x = nTileWidth; sourceSize.y = nTileHeight;
	gfx->DrawPartialDecal(pos, tileMapSheet, sourcePos, sourceSize);
}

void cHUD::DefenseBoost(olc::PixelGameEngine* gfx, olc::Decal* tileMapSheet)
{
    olc::vf2d pos; pos.x = nBuffsPosX + nTileWidth; pos.y = 0;
	olc::vf2d sourcePos; sourcePos.x = 1 * nTileWidth; sourcePos.y = 1 * nTileHeight;
	olc::vf2d sourceSize; sourceSize.x = nTileWidth; sourceSize.y = nTileHeight;
	gfx->DrawPartialDecal(pos, tileMapSheet, sourcePos, sourceSize);
}
