#include "Platformer_DynamicCreatureBomber.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureBomber::engine = nullptr;

cDynamicCreatureBomber::cDynamicCreatureBomber(cLevel* l) : cDynamicCreature("bomber", cAssets::get().GetSprite("bomber"), 8)
{
	fDynWidth = 64.0f;
	fDynHeight = 64.0f;
	bFriendly = false;
	nHealth = 5;
	nHealthMax = 5;
	level = l;
	vx = -1;	// initialize the speed so the bomber goes left first
	fInitSpeed = -1;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = true;
	nDamage = 1;
	bIsKnockable = false;
	bCanBehaveWhileAttacked = true;

	explosionSprite.push_back(new olc::Sprite("assets/gfx/explosion00.png"));
	explosionSprite.push_back(new olc::Sprite("assets/gfx/explosion01.png"));
	explosionSprite.push_back(new olc::Sprite("assets/gfx/explosion02.png"));
	explosionSprite.push_back(new olc::Sprite("assets/gfx/explosion03.png"));
	explosionSprite.push_back(new olc::Sprite("assets/gfx/explosion04.png"));
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
	if (nHealth == 0) nAIState = AI_EXPLODING;

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

			if (fDistance < 1.0f)
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
				cDynamicProjectile* p1 = new cDynamicProjectile(px, py, false, -1.5f, -1.5f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p1->bSolidVsMap = false;
				engine->AddProjectile(p1);

				cDynamicProjectile* p2 = new cDynamicProjectile(px, py, false, +0.0f, -1.5f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p2->bSolidVsMap = false;
				engine->AddProjectile(p2);

				cDynamicProjectile* p3 = new cDynamicProjectile(px, py, false, +1.5f, -1.5f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p3->bSolidVsMap = false;
				engine->AddProjectile(p3);

				cDynamicProjectile* p4 = new cDynamicProjectile(px, py, false, +1.5f, +0.0f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p4->bSolidVsMap = false;
				engine->AddProjectile(p4);

				cDynamicProjectile* p5 = new cDynamicProjectile(px, py, false, +1.5f, +1.5f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p5->bSolidVsMap = false;
				engine->AddProjectile(p5);

				cDynamicProjectile* p6 = new cDynamicProjectile(px, py, false, +0.0f, +1.5f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p6->bSolidVsMap = false;
				engine->AddProjectile(p6);

				cDynamicProjectile* p7 = new cDynamicProjectile(px, py, false, -1.5f, +1.5f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p7->bSolidVsMap = false;
				engine->AddProjectile(p7);

				cDynamicProjectile* p8 = new cDynamicProjectile(px, py, false, -1.5f, +0.0f, 0.3f, explosionSprite, 64.0f, 64.0f, false, 5);
				p8->bSolidVsMap = false;
				engine->AddProjectile(p8);

				nHealth = 0;
				KnockBack(0.0f, 0.0f, 0.3f);

				bHasExploded = true;
			}
		}
		break;
	}

	nAIState = nAINextState;
}
