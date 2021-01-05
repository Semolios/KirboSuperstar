#include "Platformer_DynamicCreature.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreature::engine = nullptr;

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
	int nSheetOffsetX = nGraphicCounter * fSpriteW;					// Same State of a sprite are stored in one line
	int nSheetOffsetY = (2 * nGraphicState + nFaceDir) * fSpriteH;	// 0 = Left Idle, 1 = Right Idle, 2 = Left Walking, 3 = Right Walking, 4 = Left Damaged, 5 = Right Damaged

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawPartialSprite((px - ox) * 64.0f + fSpriteOffsetX, (py - oy) * 64.0f + fSpriteOffsetY, sSprite, nSheetOffsetX, nSheetOffsetY, fSpriteW, fSpriteH);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cDynamicCreature::Update(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	fTimer += fElapsedTime;
	if (fTimer >= 1.0f / (float)nFramesPerSecond)
	{
		fTimer -= 1.0f / (float)nFramesPerSecond;
		nGraphicCounter++;
		nGraphicCounter %= nFramesPerSecond;
	}

	if (fKnockBackTimer > 0.0f)
	{
		vx = fKnockBackDX * cfKnockBackDX;
		vy = fKnockBackDY * cfKnockBackDY;
		bIsAttackable = false;
		fKnockBackTimer -= fElapsedTime;
		nGraphicState = DAMAGED;

		if (fKnockBackTimer <= 0.0f)
		{
			vx = fInitSpeed * signbit(vx) ? 1.0f : -1.0f; // The ennemy turns to kirbo
			if (nHealth <= 0)
			{
				if (bIsBoss)
				{
					bBossKilled = true;
				}
				else
				{
					nHealth = 0;
					bDead = true;
				}
			}
			else
			{
				bIsAttackable = true;
			}
		}

		if (bCanBehaveWhileAttacked && !bVacuumed && !bBossKilled)
			Behaviour(fElapsedTime, playerX, playerY, gfx);

		if (bVacuumed)
		{
			vx = (playerX - px) * cfVacuumSpeed;
			vy = (playerY - py) * cfVacuumSpeed;

			// ennemies under cfSwallowableLimit is swallowed when swallowing animation occurs but once kirbo swallowed he stops vacuuming
			float fTargetX = playerX - px;
			float fTargetY = playerY - py;
			float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

			if (fDistance < cfSwallowableLimit)
			{
				bSwallowable = true;
			}
		}
	}
	else if (!bBossKilled)
	{
		bSolidVsDyn = bSolidVsDynInitValue;

		if (fabs(vx) > 0)
			nGraphicState = WALKING;
		else
			nGraphicState = STANDING;

		if (vx < -0.1f) nFaceDir = 0;
		if (vx > 0.1f) nFaceDir = 1;

		Behaviour(fElapsedTime, playerX, playerY, gfx);
	}
	else
	{
		ExplodeAndDie(fElapsedTime);
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

void cDynamicCreature::Collision(float fElapsedTime)
{
	float fNewObjectPosX = px + vx * fElapsedTime;
	float fNewObjectPosY = py + vy * fElapsedTime;

	// Collision
	float fBorder = 0.1f;

	// Gravity
	if (bAffectedByGravity)
		vy += engine->GetGravityValue() * fElapsedTime;

	if (bSolidVsMap)
	{
		if (vx <= 0) // Moving Left
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, py + fBorder)) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, py + (fDynHeight / engine->GetTileHeight()) - fBorder)))
			{
				fNewObjectPosX = (int)fNewObjectPosX + 1;
				vx = 0;
			}
		}
		else // Moving Right
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + fBorder)) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + (fDynHeight / engine->GetTileHeight()) - fBorder)))
			{
				fNewObjectPosX = (int)fNewObjectPosX;
				vx = 0;
			}
		}

		if (vy <= 0) // Moving Up
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY)) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY)))
			{
				fNewObjectPosY = (int)fNewObjectPosY + 1;
				vy = 0;
			}
		}
		else // Moving Down
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSemiSolidTile(engine->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) || 
				engine->IsSemiSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))))
			{
				fNewObjectPosY = (int)fNewObjectPosY + engine->GetGroundDynamicOverlay();
				vy = 0;
			}
		}
	}

	float fDynObjectPosX = fNewObjectPosX;
	float fDynObjectPosY = fNewObjectPosY;

	// Object vs Object collisions
	// TODO REVOIR CA CAR CA MARCHE PAS CA FAIT DE LA MERDE
	//for (auto& dyn : vecEnnemies)
	//{
	//	if (dyn != object)
	//	{
	//		// If the objects are solid then they must not overlap
	//		if (dyn->bSolidVsDyn && object->bSolidVsDyn)
	//		{
	//			// Check if bounding rectangles overlap
	//			if (fDynObjectPosX < (dyn->px + (dyn->fDynWidth / (float)nTileWidth)) &&
	//				(fDynObjectPosX + (object->fDynWidth / (float)nTileWidth)) > dyn->px &&
	//				object->py < (dyn->py + (dyn->fDynHeight / (float)nTileHeight)) &&
	//				(object->py + (object->fDynHeight / (float)nTileHeight)) > dyn->py)
	//			{
	//				// First Check Horizontally - Check Left
	//				if (object->vx <= 0)
	//					fDynObjectPosX = dyn->px + (dyn->fDynWidth / (float)nTileWidth);
	//				else
	//					fDynObjectPosX = dyn->px - (object->fDynWidth / (float)nTileWidth);

	//				object->TurnAround();
	//			}

	//			if (fDynObjectPosX < (dyn->px + (dyn->fDynWidth / (float)nTileWidth)) &&
	//				(fDynObjectPosX + (object->fDynWidth / (float)nTileWidth)) > dyn->px &&
	//				fDynObjectPosY < (dyn->py + (dyn->fDynHeight / (float)nTileHeight)) &&
	//				(fDynObjectPosY + (object->fDynHeight / (float)nTileHeight)) > dyn->py)
	//			{
	//				// First Check Vertically - Check Top
	//				if (object->vy <= 0)
	//					fDynObjectPosY = dyn->py + (dyn->fDynHeight / (float)nTileHeight);
	//				else
	//					fDynObjectPosY = dyn->py - (object->fDynHeight / (float)nTileHeight);
	//			}
	//		}
	//	}
	//}

	px = fDynObjectPosX;
	py = fDynObjectPosY;
}

void cDynamicCreature::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// No default behaviour
}

void cDynamicCreature::ExplodeAndDie(float fElapsedTime)
{
	fDeadTimer += fElapsedTime;

	vx = 0.0f;
	vy = 0.0f;

	engine->ActivateShakeEffect(true);

	if (bBossExplosionAvailable)
	{
		float explosionWidth = 142.0f;
		float explosionHeight = 200.0f;

		float centerOfBossX = ((fDynWidth - explosionWidth) / 2.0f) / engine->GetTileWidth();
		float centerOfBossY = ((fDynHeight - explosionHeight) / 2.0f) / engine->GetTileHeight();
		engine->AddProjectile(engine->CreateProjectile(px + centerOfBossX, py + centerOfBossY, false, 0.0f, 0.0f, cfExplosionDuration, "bossExplosion", false, 0, false, false));

		bBossExplosionAvailable = false;
	}

	if (fDeadTimer >= cfExplosionDuration)
	{
		nHealth = 0;
		bDead = true;
		engine->ActivateShakeEffect(false);
	}
}
