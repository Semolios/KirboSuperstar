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
	int nSheetOffsetX = nGraphicCounter * fDynWidth;					// Same State of a sprite are stored in one line
	int nSheetOffsetY = (2 * nGraphicState + nFaceDir) * fDynHeight;		// 0 = Left Idle, 1 = Right Idle, 2 = Left Walking, 3 = Right Walking, 4 = Left Damaged, 5 = Right Damaged

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawPartialSprite((px - ox) * 64.0f, (py - oy) * 64.0f, sSprite, nSheetOffsetX, nSheetOffsetY, fDynWidth, fDynHeight);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cDynamicCreature::Update(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	if (fKnockBackTimer > 0.0f)
	{
		vx = fKnockBackDX * cfKnockBackDX;
		vy = fKnockBackDY * cfKnockBackDY;
		bIsAttackable = false;
		fKnockBackTimer -= fElapsedTime;
		nGraphicState = DAMAGED;

		if (fKnockBackTimer <= 0.0f)
		{
			fStateTick = 0.0f;
			bIsAttackable = true;
			vx = fInitSpeed * signbit(vx) ? 1.0f : -1.0f; // The ennemy turns to kirbo
			if (nHealth <= 0)
			{
				nHealth = 0;
				bDead = true;
			}
		}

		if (bCanBehaveWhileAttacked && !bVacuumed)
			Behaviour(fElapsedTime, playerX, playerY, gfx);

		if (bVacuumed)
		{
			vx = (playerX - px) * cfVacuumSpeed;
			vy = (playerY - py) * cfVacuumSpeed;

			// ennemies under cfSwallowableLimit is swallowed when swallowing animation occurs but once kirby swallowed he stops vacuuming
			float fTargetX = playerX - px;
			float fTargetY = playerY - py;
			float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

			if (fDistance < cfSwallowableLimit)
			{
				bSwallowable = true;
			}
		}
	}
	else
	{
		bSolidVsDyn = bSolidVsDynInitValue;
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

		if (vx < -0.1f) nFaceDir = 0;
		if (vx > 0.1f) nFaceDir = 1;

		Behaviour(fElapsedTime, playerX, playerY, gfx);
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

void cDynamicCreature::TurnAround()
{
	vx = -vx;
}

void cDynamicCreature::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// No default behaviour
}

void cDynamicCreature::PerformAttack()
{
}
