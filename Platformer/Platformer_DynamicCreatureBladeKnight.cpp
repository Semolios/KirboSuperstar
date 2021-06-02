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
	bSolidVsDynInitValue = true;
	bAffectedByGravity = true;
	nDamage = 5;
}

cDynamicCreatureBladeKnight::~cDynamicCreatureBladeKnight()
{
	delete hitbox;
	delete sSprite;
	delete level;
}

void cDynamicCreatureBladeKnight::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	switch (nAIState)
	{
		case AI_WAITING:
		{
			vx = 0.0f;

			// Check if player is nearby
			float fTargetX = playerX - px;
			float fTargetY = playerY - py;
			float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

			if (fDistance < cfTriggerDistance)
				ChangeState(AI_WALKING);
		}
		break;
		case AI_WALKING:
		{
			bOnGround = true;

			// Run in kirbo's direction
			vx = (playerX < px) ? -cfRunSpeed : cfRunSpeed;

			// Check if player is still nearby
			float fTargetX = playerX - px;
			float fTargetY = playerY - py;
			float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

#pragma region GO WAITING STATE

			if (fDistance > cfTriggerDistance)
			{
				ChangeState(AI_WAITING);
				break;
			}

#pragma endregion

#pragma region GO JUMPING STATE

			// If there is an obstacle, he jump
			if (((vx < 0) && (engine->IsSolidTile(level->GetTile(px - 1, py)) || engine->IsSolidTile(level->GetTile(px, py)) || (!engine->IsSolidTile(level->GetTile(px, py)) && !engine->IsSolidTile(level->GetTile(px, py + 1)) && !engine->IsSemiSolidTile(level->GetTile(px, py + 1))))) ||
				((vx > 0) && (engine->IsSolidTile(level->GetTile(px + 2, py)) || engine->IsSolidTile(level->GetTile(px + 1, py)) || (!engine->IsSolidTile(level->GetTile(px + 1, py)) && !engine->IsSolidTile(level->GetTile(px + 1, py + 1)) && !engine->IsSemiSolidTile(level->GetTile(px + 1, py + 1)))))
				)
			{
				ChangeState(AI_JUMPING);
				break;
			}

#pragma endregion

#pragma region GO ATTACK STATE

			// Randomly choose to directly attack or jump over the player to attack him with plungeon attack when he's near the player
			int chosenAttack = rand() % 2;

			if (fDistance < cfAttackTriggerDistance)
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

			vx = (playerX < px) ? -cfRunSpeed : cfRunSpeed;

			if (bOnGround)
			{
				vy = cfJumpSpeed;
				bOnGround = false;
				fTimer = 0.0f;
			}
			else
			{
				fTimer += fElapsedTime;
				if ((abs(playerX - px) < 0.5f && playerY > py))
					ChangeState(AI_DOWNATTACK);
				else if (OnGround() && vy >= 0)
					ChangeState(AI_WALKING);
			}

			// Freeze animation at the last frame to prevent looping animation while falling
			if (fTimer >= cfFallingAnimationTime) nGraphicCounter = nFramesPerSecond - 1;
		}
		break;
		case AI_SIDEATTACK:
		{
			nGraphicState = SIDEATTACK;
			vx = 0.0f;

			fTimer += fElapsedTime;

			// first attack
			if (fTimer >= cfFirstAttackTime && !bCantSpawnAOE1)
			{
				bCantSpawnAOE1 = true;

				// slightly moves toward the player
				px += (playerX < px) ? -cfAttackDash : cfAttackDash;
			}

			// second attack
			if (fTimer >= cfSecondAttackTime && !bCantSpawnAOE2)
			{
				bCantSpawnAOE2 = true;

				// slightly moves toward the player
				px += (playerX < px) ? -cfAttackDash : cfAttackDash;
			}

			if (fTimer >= cfAttackTime)
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
			if (fTimer <= cfTimeBeforeDownAttack)
			{
				nGraphicCounter = 0;
				vy = 0.0f;
			}
			else
			{
				nGraphicCounter = 1;
				vy = cfDownAtkSpeed;

				// Create an AOE
				if (!bCantSpawnAOE1)
				{
					engine->AddProjectile(engine->CreateProjectile(px + cfDownAOESprOffsetX, py + cfDownAOESprOffsetY, false, 0.0f, cfDownAtkSpeed, cfDownAtkAOEDuration, "swordDownAOE", false, cnDownAtkDmg, true));
					bCantSpawnAOE1 = true;
				}
			}

			if (OnGround())
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
			if (fTimer >= cfFirstAttackTime && !bCantSpawnAOE1)
			{
				engine->AddProjectile(engine->CreateProjectile(px + cfUpAOESprOffsetX, py + cfUpAOESprOffsetY, false, 0.0f, cfUpAtkSpeed, cfUpAtkDuration, "swordUpAOE", false, cnUpAtkDmg, true));
				bCantSpawnAOE1 = true;
			}

			if (fTimer >= cfAttackTime)
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

bool cDynamicCreatureBladeKnight::OnGround()
{
	return engine->IsSolidTile(level->GetTile(px + cfCollisionLowerLimit, py + 1)) ||
		engine->IsSolidTile(level->GetTile(px + cfCollisionUpperLimit, py + 1)) ||
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionLowerLimit, py + 1)) ||
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionUpperLimit, py + 1));
}
