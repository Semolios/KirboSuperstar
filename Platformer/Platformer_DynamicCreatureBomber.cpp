#include "Platformer_DynamicCreatureBomber.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureBomber::engine = nullptr;

cDynamicCreatureBomber::cDynamicCreatureBomber(cLevel* l) : cDynamicCreature("bomber", cAssets::get().GetSprite("bomber"), 8)
{
	fDynWidth = 64.0f;
	fDynHeight = 64.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = 0.0f;
	fSpriteOffsetY = 0.0f;
	bFriendly = false;
	nHealth = 1;
	nHealthMax = 1;
	level = l;
	vx = -1;	// initialize the speed so the bomber goes left first
	fInitSpeed = -1;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = true;
	nDamage = 2;
	bIsKnockable = false;
	bCanBehaveWhileAttacked = true;
}

void cDynamicCreatureBomber::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// lambda fonction to check if there is a hole or a wall to change direction
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < level->GetWidth() && y >= 0 && y < level->GetHeight())
			return level->GetLevel()[y * level->GetWidth() + x];
		else
			return L' ';
	};

	// if hit by a player, instantly explodes
	if (nHealth <= 0) nAIState = AI_EXPLODING;

	switch (nAIState)
	{
		case AI_WALKING:
		{
			if (vy == 0)
			{
				if (vx < 0)
				{
					// Check left wall or hole
					if (engine->IsSolidTile(GetTile(px, py)) || (!engine->IsSolidTile(GetTile(px, py)) && !engine->IsSolidTile(GetTile(px, py + 1)) && !engine->IsSemiSolidTile(GetTile(px, py + 1))))
					{
						TurnAround();
					}
				}
				else if (vx > 0)
				{
					// Check right wall or hole
					if (engine->IsSolidTile(GetTile(px + 1, py)) || (!engine->IsSolidTile(GetTile(px + 1, py)) && !engine->IsSolidTile(GetTile(px + 1, py + 1)) && !engine->IsSemiSolidTile(GetTile(px + 1, py + 1))))
					{
						TurnAround();
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
				engine->AddProjectile(engine->CreateProjectile(px - 1, py - 1, false, -1.5f, -1.5f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));
				engine->AddProjectile(engine->CreateProjectile(px + 0, py - 1, false, +0.0f, -1.5f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));
				engine->AddProjectile(engine->CreateProjectile(px + 1, py - 1, false, +1.5f, -1.5f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));
				engine->AddProjectile(engine->CreateProjectile(px + 1, py + 0, false, +1.5f, +0.0f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));
				engine->AddProjectile(engine->CreateProjectile(px + 1, py + 1, false, +1.5f, +1.5f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));
				engine->AddProjectile(engine->CreateProjectile(px + 0, py + 1, false, +0.0f, +1.5f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));
				engine->AddProjectile(engine->CreateProjectile(px - 1, py + 1, false, -1.5f, +1.5f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));
				engine->AddProjectile(engine->CreateProjectile(px - 1, py + 0, false, -1.5f, +0.0f, cfExplosionDuration, "explosion", false, cnExplosionDmg, false));

				nHealth = 0;
				KnockBack(0.0f, 0.0f, cfKnockBackDuration);

				bHasExploded = true;
			}
		}
		break;
	}

	nAIState = nAINextState;
}
