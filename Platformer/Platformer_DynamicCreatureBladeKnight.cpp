#include "Platformer_DynamicCreatureBladeKnight.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureBladeKnight::engine = nullptr;

cDynamicCreatureBladeKnight::cDynamicCreatureBladeKnight(cLevel* l) : cDynamicCreature("bladeKnight", cAssets::get().GetSprite("bladeKnight"), 8)
{
	fDynWidth = 64.0f;
	fDynHeight = 64.0f;
	fSpriteW = 128.0f;
	fSpriteH = 96.0f;
	fSpriteOffsetX = -32.0f;
	fSpriteOffsetY = -32.0f;
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
	level = l;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true; // Blade knight is not stopped by other ennemies
	bAffectedByGravity = true;
	nDamage = 5;
}

void cDynamicCreatureBladeKnight::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// lambda fonction to check if there is a hole or a wall to change direction
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < level->GetWidth() && y >= 0 && y < level->GetHeight())
			return level->GetLevel()[y * level->GetWidth() + x];
		else
			return L' ';
	};

	switch (nAIState)
	{
		case AI_WAITING:
		{
			vx = 0.0f;

			// Check if player is nearby
			float fTargetX = playerX - px;
			float fTargetY = playerY - py;
			float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

			if (fDistance < 5.0f)
				ChangeState(AI_WALKING);
		}
		break;
		case AI_WALKING:
		{
			bOnGround = true;

			// Run in kirbo's direction
			vx = (playerX < px) ? -2.5f : 2.5f;

			// Check if player is still nearby
			float fTargetX = playerX - px;
			float fTargetY = playerY - py;
			float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

#pragma region GO WAITING STATE

			if (fDistance > 5.0f)
			{
				ChangeState(AI_WAITING);
				break;
			}

#pragma endregion

#pragma region GO JUMPING STATE

			// If there is an obstacle, he jump
			if (((vx < 0) && (engine->IsSolidTile(GetTile(px - 1, py)) || engine->IsSolidTile(GetTile(px, py)) || (!engine->IsSolidTile(GetTile(px, py)) && !engine->IsSolidTile(GetTile(px, py + 1)) && !engine->IsSemiSolidTile(GetTile(px, py + 1))))) ||
				((vx > 0) && (engine->IsSolidTile(GetTile(px + 2, py)) || engine->IsSolidTile(GetTile(px + 1, py)) || (!engine->IsSolidTile(GetTile(px + 1, py)) && !engine->IsSolidTile(GetTile(px + 1, py + 1)) && !engine->IsSemiSolidTile(GetTile(px + 1, py + 1)))))
				)
			{
				ChangeState(AI_JUMPING);
				break;
			}

#pragma endregion

#pragma region GO ATTACK STATE

			// Randomly choose to directly attack or jump over the player to attack him with plungeon attack when he's near the player
			srand(time(NULL));
			int chosenAttack = rand() % 2;

			if (fDistance < 1.5f)
			{
				ChangeState((chosenAttack == 0) ? AI_SIDEATTACK : AI_JUMPING);
				break;
			}

			// If player is above but not near he UP ATTACK
			if ((abs(playerX - px) < 0.5f && playerY < py))
			{
				ChangeState(AI_UPATTACK);
				break;
			}

#pragma endregion

		}
		break;
		case AI_JUMPING:
		{
			nGraphicState = JUMPING;
			
			vx = (playerX < px) ? -2.5f : 2.5f;

			if (bOnGround)
			{
				vy = -10.0f;
				bOnGround = false;
				fTimer = 0.0f;
			}
			else
			{
				fTimer += fElapsedTime;
				if ((abs(playerX - px) < 0.5f && playerY > py))
					ChangeState(AI_DOWNATTACK);
				else if (engine->IsSolidTile(GetTile(px + 0.1f, py + 1)) || engine->IsSolidTile(GetTile(px + 0.9f, py + 1)))
					ChangeState(AI_WALKING);
			}

			// Freeze animation at the last frame to prevent looping animation while falling
			if (fTimer >= fFallingAnimationTime) nGraphicCounter = 7;
		}
		break;
		case AI_SIDEATTACK:
		{
			nGraphicState = SIDEATTACK;
			vx = 0.0f;

			fTimer += fElapsedTime;

			// first attack
			if (fTimer >= 0.375f && !bCantSpawnAOE1)
			{
				bCantSpawnAOE1 = true;

				// slightly moves toward the player
				px += (playerX < px) ? -0.75f : 0.75f;
			}

			// second attack
			if (fTimer >= 0.625f && !bCantSpawnAOE2)
			{
				bCantSpawnAOE2 = true;

				// slightly moves toward the player
				px += (playerX < px) ? -0.75f : 0.75f;
			}

			if (fTimer >= fAttackTime)
			{
				bCantSpawnAOE1 = false;
				bCantSpawnAOE2 = false;
				ChangeState(AI_WALKING);
			}
		}
		break;
		case AI_DOWNATTACK:
		{
			nGraphicState = DOWNATTACK;
			vx = 0.0f;

			// Wait 0.5 seconds then fall on the player
			fTimer += fElapsedTime;
			if (fTimer <= fTimeBeforeDownAttack)
			{
				nGraphicCounter = 0;
				vy = 0.0f;
			}
			else
			{
				nGraphicCounter = 1;
				vy = 8.0f;

				// Create an AOE
				if (!bCantSpawnAOE1)
				{
					engine->AddProjectile(engine->CreateProjectile(px + 0.25f, py + 0.25f, false, 0.0f, 8.0f, 10.0f, "swordDownAOE", 32.0f, 64.0f, false, 5, true));
					bCantSpawnAOE1 = true;
				}
			}

			if (engine->IsSolidTile(GetTile(px + 0.1f, py + 1)) || engine->IsSolidTile(GetTile(px + 0.9f, py + 1)))
			{
				bCantSpawnAOE1 = false;
				ChangeState(AI_WALKING);
			}
		}
		break;
		case AI_UPATTACK:
		{
			nGraphicState = UPATTACK;
			vx = 0.0f;

			fTimer += fElapsedTime;

			// when blade knight raise the sword, an AOE appears
			if (fTimer >= 0.375f && !bCantSpawnAOE1)
			{
				engine->AddProjectile(engine->CreateProjectile(px + 0.25f, py - 0.5f, false, 0.0f, -10.0f, 0.1f, "swordUpAOE", 32.0f, 32.0f, false, 5, true));
				bCantSpawnAOE1 = true;
			}

			if (fTimer >= fAttackTime)
			{
				bCantSpawnAOE1 = false;
				ChangeState(AI_WALKING);
			}
		}
		break;
	}

	nAIState = nAINextState;
}

void cDynamicCreatureBladeKnight::ChangeState(AI_STATE state)
{
	nGraphicCounter = 0;
	fTimer = 0.0f;
	nAINextState = state;
}
