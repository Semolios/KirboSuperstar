#include "Platformer_HUD.h"

cHUD::cHUD()
{
}

void cHUD::HealthBar(olc::PixelGameEngine* gfx, olc::Sprite* healthBar)
{
	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawSprite(0, 0, healthBar);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cHUD::HealthPoints(olc::PixelGameEngine* gfx, olc::Sprite* healthPoint, int health)
{
	for (int i = 0; i < health; i++)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawSprite(nFirstHealthPointPosX + i * (healthPoint->width + 1), nHealthPointPosY, healthPoint);
		gfx->SetPixelMode(olc::Pixel::NORMAL);
	}
}

void cHUD::BossHealthBar(olc::PixelGameEngine* gfx, olc::Sprite* bossHealthBar, std::vector<cDynamicCreature*> vecEnnemies)
{
	// Health points of All ennemies in the arena
	int nBossHP = 0;
	for (auto& dyn : vecEnnemies)
	{
		nBossHP += dyn->GetHealth();
	}

	// Health
	gfx->FillRect(nBossHealthPosX + (nBossMaxHealth - nBossHP), nBossHealthPosY, nBossHP, nBossHealthHeight, olc::RED);

	// Health bar
	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawSprite(nBossHealthBarPosX, nBossHealthBarPosY, bossHealthBar);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cHUD::DamageBoost(olc::PixelGameEngine* gfx, olc::Sprite* tileMapSheet)
{
	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawPartialSprite(nBuffsPosX, 0, tileMapSheet, 0 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cHUD::DefenseBoost(olc::PixelGameEngine* gfx, olc::Sprite* tileMapSheet)
{
	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawPartialSprite(nBuffsPosX + nTileWidth, 0, tileMapSheet, 1 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}
