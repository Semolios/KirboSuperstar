#include "Platformer_DynamicCreature.h"

cDynamicCreature::cDynamicCreature(std::string n, olc::Sprite* sprite, int framesPerSecond) : cDynamic(n)
{
	sSprite = sprite;
	nHealth = 10;
	nHealthMax = 10;
	nGraphicState = STANDING;
	nGraphicCounter = 0;
	nFramesPerSecond = framesPerSecond;
	fTimer = 0.0f;
	bIsAttackable = true;
}

void cDynamicCreature::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	int nSheetOffsetX = nGraphicCounter * nTileSize;					// Same State of a sprite are stored in one line
	int nSheetOffsetY = (2 * nGraphicState + nFaceDir) * nTileSize;		// 0 = Left Idle, 1 = Right Idle, 2 = Left Walking, 3 = Right Walking, 4 = Left Dead, 5 = Right Dead

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawPartialSprite((px - ox) * nTileSize, (py - oy) * nTileSize, sSprite, nSheetOffsetX, nSheetOffsetY, nTileSize, nTileSize);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cDynamicCreature::Update(float fElapsedTime, float playerX, float playerY)
{
	// TODO revoir l'animation de mort pour qu'il disparaisse après avoir terminé une boucle de l'animation
	// si pv <= 0 effectuer une boucle d'animation puis quand elle est terminée, mettre bDead à true, dans le moteur faire disparaitre les ennemis ayant bDead = true
	if (fKnockBackTimer > 0.0f)
	{
		vx = fKnockBackDX * cfKnockBackDX;
		vy = fKnockBackDY * cfKnockBackDY;
		bIsAttackable = false;
		fKnockBackTimer -= fElapsedTime;
		if (fKnockBackTimer <= 0.0f)
		{
			fStateTick = 0.0f;
			bIsAttackable = true;
		}
	}
	else
	{
		bSolidVsDyn = true;
		fTimer += fElapsedTime;
		if (fTimer >= 1.0f / (float)nFramesPerSecond)
		{
			fTimer -= 1.0f / (float)nFramesPerSecond;
			nGraphicCounter++;
			nGraphicCounter %= nFramesPerSecond;
		}

		// TODO eventuellement exporter cette méthode dans les classes filles pour avoir différentes animations
		if (fabs(vx) > 0)
			nGraphicState = WALKING;
		else
			nGraphicState = STANDING;

		if (nHealth <= 0)
			nGraphicState = DEAD;

		if (vx < -0.1f) nFaceDir = 0;
		if (vx > 0.1f) nFaceDir = 1;

		Behaviour(fElapsedTime, playerX, playerY);
	}
}

void cDynamicCreature::KnockBack(float dx, float dy, float dist)
{
	fKnockBackDX = dx;
	fKnockBackDY = dy;
	fKnockBackTimer = dist;
	bSolidVsDyn = false;
	bIsAttackable = false;
}

void cDynamicCreature::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	// No default behaviour
}

void cDynamicCreature::PerformAttack()
{
}
