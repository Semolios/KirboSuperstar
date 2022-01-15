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
	// don't loop jump animation
	bLoopAnimation = nAIState != AI_JUMPING;

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

			bool goJumpingState = true;
			int retflag;

			// If there is an obstacle, he jump
			if (LeftObstacle())
			{
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->TopCollisionOneCorner(px, py + 1.0f) && !LeftWall())
					{
						goJumpingState = false;
					}
				}

				Jump(goJumpingState, retflag);
				if (retflag == 2) break;
			}
			else if (RightObstacle())
			{
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->TopCollisionOneCorner(px + 1.0f, py + 1.0f) && !RightWall())
					{
						goJumpingState = false;
					}
				}

				Jump(goJumpingState, retflag);
				if (retflag == 2) break;
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
			}
			else
			{
				if ((abs(playerX - px) < 0.5f && playerY > py))
				{
					ChangeState(AI_DOWNATTACK);
				}
				else if (TouchGround() && vy >= 0)
				{
					ChangeState(AI_WALKING);
				}
				else if (vy >= 0)
				{
					for (auto& ptfm : engine->GetClosePlatforms(px, py))
					{
						if (ptfm->TopCollision(px, px + 1.0f, py + 1.0f))
						{
							ChangeState(AI_WALKING);
						}
					}
				}
			}
		}
		break;
		case AI_SIDEATTACK:
		{
			nGraphicState = MOVE2;
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
			nGraphicState = MOVE3;
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
					engine->AddProjectile(px + cfDownAOESprOffsetX, py + cfDownAOESprOffsetY, false, 0.0f, cfDownAtkSpeed, cfDownAtkAOEDuration, "swordDownAOE", false, cnDownAtkDmg, true);
					bCantSpawnAOE1 = true;
				}
			}

			if (TouchGround())
			{
				bCantSpawnAOE1 = false;
				ChangeState(AI_WALKING);
			}
			else
			{
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->TopCollision(px, px + 1.0f, py + 1.0f))
					{
						bCantSpawnAOE1 = false;
						ChangeState(AI_WALKING);
					}
				}
			}
		}
		break;
		case AI_UPATTACK:
		{
			nGraphicState = MOVE1;
			vx = 0.0f;

			fTimer += fElapsedTime;

			// when blade knight raise the sword, an AOE appears
			if (fTimer >= cfFirstAttackTime && !bCantSpawnAOE1)
			{
				engine->AddProjectile(px + cfUpAOESprOffsetX, py + cfUpAOESprOffsetY, false, 0.0f, cfUpAtkSpeed, cfUpAtkDuration, "swordUpAOE", false, cnUpAtkDmg, true);
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

void cDynamicCreatureBladeKnight::Jump(bool goJumpingState, int& retflag)
{
	retflag = 1;
	if (goJumpingState)
	{
		ChangeState(AI_JUMPING);
		{
			retflag = 2; return;
		};
	}
}

bool cDynamicCreatureBladeKnight::RightObstacle()
{
	return (vx > 0) && (RightWall() || RightHole());
}

bool cDynamicCreatureBladeKnight::RightHole()
{
	return !engine->IsSolidTile(level->GetTile(px + 1, py)) && !engine->IsSolidTile(level->GetTile(px + 1, py + 1)) && !engine->IsSemiSolidTile(level->GetTile(px + 1, py + 1));
}

bool cDynamicCreatureBladeKnight::RightWall()
{
	bool mobileWall = false;
	for (auto& ptfm : engine->GetClosePlatforms(px, py))
	{
		if (ptfm->LeftCollision(py, py + 1, px + 2))
		{
			mobileWall = true;
		}
	}
	return engine->IsSolidTile(level->GetTile(px + 2, py)) || engine->IsSolidTile(level->GetTile(px + 1, py)) || mobileWall;
}

bool cDynamicCreatureBladeKnight::LeftObstacle()
{
	return (vx < 0) && (LeftWall() || LeftHole());
}

bool cDynamicCreatureBladeKnight::LeftHole()
{
	return !engine->IsSolidTile(level->GetTile(px, py)) && !engine->IsSolidTile(level->GetTile(px, py + 1)) && !engine->IsSemiSolidTile(level->GetTile(px, py + 1));
}

bool cDynamicCreatureBladeKnight::LeftWall()
{
	bool mobileWall = false;
	for (auto& ptfm : engine->GetClosePlatforms(px, py))
	{
		if (ptfm->RightCollision(py, py + 1, px - 1))
		{
			mobileWall = true;
		}
	}
	return engine->IsSolidTile(level->GetTile(px - 1, py)) || engine->IsSolidTile(level->GetTile(px, py)) || mobileWall;
}

void cDynamicCreatureBladeKnight::ChangeState(AI_STATE state)
{
	nGraphicCounter = 0;
	fTimer = 0.0f;
	nAINextState = state;
}

bool cDynamicCreatureBladeKnight::TouchGround()
{
	return engine->IsSolidTile(level->GetTile(px + cfCollisionLowerLimit, py + 1)) ||
		engine->IsSolidTile(level->GetTile(px + cfCollisionUpperLimit, py + 1)) ||
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionLowerLimit, py + 1)) ||
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionUpperLimit, py + 1));
}
