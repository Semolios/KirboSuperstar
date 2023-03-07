#include "Platformer_DynamicCreatureBomber.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureBomber::engine = nullptr;

cDynamicCreatureBomber::cDynamicCreatureBomber(cLevel* l) : cDynamicCreature("bomber", cAssets::get().GetSprite("bomber"), 8)
{
	fDynWidth = 48.0f;
	fDynHeight = 40.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = -8.0f;
	fSpriteOffsetY = -24.0f;
	bFriendly = false;
	nHealth = 1;
	nHealthMax = 1;
	level = l;
	vx = -1;
	fInitSpeed = -1;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = true;
	nDamage = 2;
	bIsKnockable = false;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureBomber::~cDynamicCreatureBomber()
{
	delete hitbox;
	delete dynSprite;
	delete level;
}

void cDynamicCreatureBomber::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// if hit by a player, instantly explodes
	if (nHealth <= 0) nAIState = AI_EXPLODING;

	switch (nAIState)
	{
		case AI_WALKING:
		{
			bool mustTurnBack = true;

			if (vy == 0)
			{
				if (vx < 0)
				{
					if (LeftObstacle())
					{
						// Don't check the platforms if the bomber is on ground, or all the bombers will check all platforms
						for (auto& ptfm : engine->GetClosePlatforms(px, py))
						{
							if (ptfm->TopCollisionOneCorner(px, py + fNormalizedH) && !engine->IsSolidTile(level->GetTile(px, py)))
							{
								mustTurnBack = false;
							}
						}
						if (mustTurnBack)
							TurnBack();
					}
					else
					{
						mustTurnBack = false;
						for (auto& ptfm : engine->GetClosePlatforms(px, py))
						{
							if (ptfm->RightCollision(py, py + fNormalizedH, px))
							{
								mustTurnBack = true;
							}
						}
						if (mustTurnBack)
							TurnBack();
					}
				}
				else if (vx > 0)
				{
					if (RightObstacle())
					{
						// Don't check the platforms if the bomber is on ground, or all the bombers will check all platforms
						for (auto& ptfm : engine->GetClosePlatforms(px, py))
						{
							if (ptfm->TopCollisionOneCorner(px + fNormalizedW, py + fNormalizedH) && !ptfm->LeftCollision(py, py + fNormalizedH, px + fNormalizedW) && !engine->IsSolidTile(level->GetTile(px + fNormalizedW, py)))
							{
								mustTurnBack = false;
							}
						}
						if (mustTurnBack)
							TurnBack();
					}
					else
					{
						mustTurnBack = false;
						for (auto& ptfm : engine->GetClosePlatforms(px, py))
						{
							if (ptfm->LeftCollision(py, py + fNormalizedH, px + fNormalizedW))
							{
								mustTurnBack = true;
							}
						}
						if (mustTurnBack)
							TurnBack();
					}
				}
				else
				{
					vx = 1.0f;
				}
			}

			// Check if player is nearby
			float fTargetX = playerX - px;
			float fTargetY = playerY - py;
			float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

			if (fDistance < cfTriggerDistance)
				nAINextState = AI_CHARGING;
		}
		break;
		case AI_CHARGING:
		{
			vx = 0.0f;
			vy = 0.0f;

			fTimer += fElapsedTime;

			if (fTimer >= fChargingTime)
			{
				fTimer = 0.0f;
				nAINextState = AI_EXPLODING;
			}
		}
		break;
		case AI_EXPLODING:
		{
			if (!bHasExploded)
			{
				engine->PlaySample("explosion");
				float explosionWidth = 142.0f;
				float explosionHeight = 200.0f;

				float centerX = ((fDynWidth  - explosionWidth) /  2.0f) / engine->GetTileWidth();
				float centerY = ((fDynHeight - explosionHeight) / 2.0f) / engine->GetTileHeight();
				engine->AddProjectile(px + centerX, py + centerY, false, 0.0f, 0.0f, cfExplosionDuration - cfExplosionNoHarmingDuration, "explosion", false, cnExplosionDmg, false);
				engine->AddProjectile(px + centerX, py + centerY, true, 0.0f, 0.0f, cfExplosionDuration, "explosion", false, 0, false, true, 0, true, -3.0f, "", false, "", true);

				nHealth = 0;
				KnockBack(0.0f, 0.0f, cfKnockBackDuration);

				bHasExploded = true;
			}
		}
		break;
	}

	nAIState = nAINextState;
}

bool cDynamicCreatureBomber::RightObstacle()
{
	return engine->IsSolidTile(level->GetTile(px + fNormalizedW, py)) ||
		   (!engine->IsSolidTile(level->GetTile(px + fNormalizedW, py)) && 
		    !engine->IsSolidTile(level->GetTile(px + fNormalizedW, py + fNormalizedH)) &&
		    !engine->IsSemiSolidTile(level->GetTile(px + fNormalizedW, py + fNormalizedH)));
}

bool cDynamicCreatureBomber::LeftObstacle()
{
	return engine->IsSolidTile(level->GetTile(px, py)) ||
		   (!engine->IsSolidTile(level->GetTile(px, py)) &&
			!engine->IsSolidTile(level->GetTile(px, py + fNormalizedH)) &&
			!engine->IsSemiSolidTile(level->GetTile(px, py + fNormalizedH)));
}
