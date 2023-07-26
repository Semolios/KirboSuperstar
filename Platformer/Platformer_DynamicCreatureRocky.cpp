#include "Platformer_DynamicCreatureRocky.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureRocky::engine = nullptr;

cDynamicCreatureRocky::cDynamicCreatureRocky(cLevel* l) : cDynamicCreature("rocky", cAssets::get().GetSprite("rocky"), 16)
{
	fDynWidth = 56.0f;
	fDynHeight = 56.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = -4.0f;
	fSpriteOffsetY = -4.0f;
	bFriendly = false;
	nHealth = 9;
	nHealthMax = 9;
	level = l;
	vx = 0;
	fInitSpeed = 0;
	bSolidVsMap = true;
	bAffectedByGravity = false;
	nDamage = 2;
}

cDynamicCreatureRocky::~cDynamicCreatureRocky()
{
	delete hitbox;
	delete dynSprite;
	delete level;
}

void cDynamicCreatureRocky::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	vx = 0;

	switch (nAIState)
	{
		case AI_WAITING:
		{
			bAffectedByGravity = false;
			vy = 0;

			if (abs(playerX - px) < 0.5f && playerY > py)
			{
				if (!HasObstacleUnder(playerY))
				{
					engine->PlaySample("rockyFall");
					nAINextState = AI_FALLING;
				}
			}
		}
		break;
		case AI_FALLING:
		{
			bAffectedByGravity = true;
			nGraphicState = WALKING;

			if (engine->IsSolidTile(level->GetTile(px,				  py + fNormalizedH)) ||
				engine->IsSolidTile(level->GetTile(px + fNormalizedW, py + fNormalizedH)) ||
				engine->IsSemiSolidTile(level->GetTile(px,				  py + fNormalizedH)) ||
				engine->IsSemiSolidTile(level->GetTile(px + fNormalizedW, py + fNormalizedH)))
			{
				engine->StopSample("rockyFall");
				nAINextState = AI_LANDING;
			}
			else
			{
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->TopCollision(px, px + fNormalizedW, py + fNormalizedH))
					{
						engine->StopSample("rockyFall");
						nAINextState = AI_LANDING;
					}
				}
			}
		}
		break;
		case AI_LANDING:
		{
			fTimer += fElapsedTime;

			if (fTimer >= fLandingTime)
			{
				fTimer = 0.0f;
				nAINextState = AI_GOING_BACK_UP;
			}
		}
		break;
		case AI_GOING_BACK_UP:
		{
			bAffectedByGravity = false;
			vy = cfGoingBackUpSpeed;

			if (engine->IsSolidTile(level->GetTile(px,				  py - 0.01f)) ||
				engine->IsSolidTile(level->GetTile(px + fNormalizedW, py - 0.01f)) ||
				DynamicCeiling() || py < 1)
			{
				vy = 0;
				nAINextState = AI_WAITING;
			}
		}
		break;
	}

	nAIState = nAINextState;
}

bool cDynamicCreatureRocky::DynamicCeiling()
{
	// Moving platforms collision
	for (auto& ptfm : engine->GetClosePlatforms(px, py))
	{
		if (ptfm->BotCollision(px, px + (fDynWidth / engine->GetTileWidth()), py))
		{
			return true;
		}
	}
	return false;
}

bool cDynamicCreatureRocky::HasObstacleUnder(float playerY)
{
	bool isObstacle = false;

	for (int i = ((int)py + 1); i < playerY; i++)
	{
		if (engine->IsSolidTile(level->GetTile(px,				  i)) ||
			engine->IsSolidTile(level->GetTile(px + fNormalizedW, i)) ||
			engine->IsSemiSolidTile(level->GetTile(px,				  i)) ||
			engine->IsSemiSolidTile(level->GetTile(px + fNormalizedW, i)))
		{
			isObstacle = true;
		}
		else
		{
			for (auto& ptfm : engine->GetClosePlatforms(px, i))
			{
				if (ptfm->TopCollision(px, px + fNormalizedW, i + 1.0f))
				{
					isObstacle = true;
				}
			}
		}
	}

	return isObstacle;
}
