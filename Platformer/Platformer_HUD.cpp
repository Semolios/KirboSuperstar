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
		gfx->DrawSprite(13 + i * 10, 14, healthPoint);
		gfx->SetPixelMode(olc::Pixel::NORMAL);
	}
}

void cHUD::BossHealthBar(olc::PixelGameEngine* gfx, olc::Sprite* bossHealthBar, std::vector<cDynamicCreature*> vecEnnemies)
{		
	// Health points
	int nBossHP = 0;
	for (auto& dyn : vecEnnemies)
	{
		nBossHP += dyn->GetHealth();
	}

	gfx->FillRect(663 + (100 - (nBossHP)), 30, nBossHP, 25, olc::VERY_DARK_MAGENTA);

	// Health bar
	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawSprite(650, 0, bossHealthBar);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}
