#include "Platformer_DynamicCreatureWhispyWood.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureWhispyWood::engine = nullptr;

cDynamicCreatureWhispyWood::cDynamicCreatureWhispyWood(cLevel* l) : cDynamicCreature("whispyWood", cAssets::get().GetSprite("whispyWood"), 4)
{
	fDynWidth = 132.0f; // cheating a little bit so the player can't stay behind whispy wood
	fDynHeight = 256.0f;
	fSpriteW = 384.0f;
	fSpriteH = 320.0f;
	fSpriteOffsetX = -128.0f;
	fSpriteOffsetY = 0.0f;
	bFriendly = false;
	nHealth = 100;
	nHealthMax = 100;
	level = l;
	bSolidVsMap = true;
	bAffectedByGravity = true;
	nDamage = 3;
	bCanBehaveWhileAttacked = true;

	srand(time(NULL));
}

void cDynamicCreatureWhispyWood::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	vx = 0.0f; vy = 0.0f; nFaceDir = 0; // Whispy wood can't move and is always facing left

	switch (nAIState)
	{
		case AI_WAITING:
		{
			fBehaviourTimer += fElapsedTime;

			bCantSpawnAOE = false;
			fProjectilesTimer = 0.25f;

			if (fBehaviourTimer >= fWaitingTime)
			{
				nChosenAttack = rand() % 3;

				if (nChosenAttack == 0) ChangeState(AI_SCREAMING);
				if (nChosenAttack == 1) ChangeState(AI_WIND);
				if (nChosenAttack == 2) ChangeState(AI_PROJECTILES);
			}
		}
		break;
		case AI_SCREAMING:
		{
			MapGraphicState(SCREAMING);
			engine->ActivateShakeEffect(true);

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fScreamTime)
			{
				engine->ActivateShakeEffect(false);
				ChangeState(AI_WAITING);
			}

			// spawn a little ground effect during fRootSpawnTime to indicate the player that a root is gonna spawn
			fRootTimer += fElapsedTime;
			if (!bCantSpawnAOE)
			{
				rootSpawnX = playerX;
				engine->AddProjectile(engine->CreateProjectile(playerX, 9, false, 0.0f, 0.0f, fRootSpawnTime, "movingGround", 64.0f, 16.0f, false, 0, false, false));
				bCantSpawnAOE = true;
			}

			if (fRootTimer >= fRootSpawnTime)
			{
				fRootTimer = 0.0f;

				// spawn moving ground only if the root has the time to spawn
				if ((fBehaviourTimer + fRootSpawnTime) <= fScreamTime)
					bCantSpawnAOE = false;

				// Spawn the root to attack kirbo
				engine->AddProjectile(engine->CreateProjectile(rootSpawnX, 7, false, 0.0f, 0.0f, 0.6f, "root", 32.0f, 128.0f, false, 3, false, false));
			}
		}
		break;
		case AI_WIND:
		{
			fBehaviourTimer += fElapsedTime;

			MapGraphicState(BLOW);

			if (fBehaviourTimer >= cfBlowingAnimationTime)
				engine->WindEffect(-1.0f, 20.0f, true);

			if (fBehaviourTimer >= cfBlowingTime)
			{
				engine->WindEffect(0.0f, 0.0f, false);
				ChangeState(AI_WAITING);
			}

			// Freeze animation at the last frame to prevent looping animation while blowing
			if (fBehaviourTimer >= cfBlowingAnimationTime) nGraphicCounter = 3;
		}
		break;
		case AI_PROJECTILES:
		{
			fBehaviourTimer += fElapsedTime;

			MapGraphicState(PROJECTILES);

			fProjectilesTimer += fElapsedTime;

			if (fProjectilesTimer >= cfProjectilesSpawnTime)
			{
				fProjectilesTimer = 0.0f;

				// projectile Y speed betweene -5 and +5
				float projectileSpeedY = ((float)(rand() % 100) / 10.0f) - 5.0f;

				// Spawn the projectile to attack kirbo
				engine->AddProjectile(engine->CreateProjectile(14, 7.5f, false, -10.0f, projectileSpeedY, 2.0f, "blow", 64.0f, 64.0f, false, 2, false, false));
			}

			if (fBehaviourTimer >= cfProjectilesTime)
				ChangeState(AI_WAITING);
		}
		break;
	}

	// Periodically spawn apples at a random location
	fAppleTimer += fElapsedTime;

	if (fAppleTimer >= fAppleSpawnTime)
	{
		fAppleTimer = 0.0f;

		// Apple spawn between 1 and 16 (Whispy wood position)
		float applePosX = ((float)(rand() % 150) / 10.0f) + 1.0f;

		// Spawn apple
		engine->AddProjectile(engine->CreateProjectile(applePosX, 2, false, 0.0f, 0.0f, 10.0f, "apple", 64.0f, 64.0f, true, 2, true, false));
	}

	UpdateTimers();

	nAIState = nAINextState;
}

void cDynamicCreatureWhispyWood::ChangeState(AI_STATE state)
{
	nGraphicCounter = 0;
	fBehaviourTimer = 0.0f;
	fRootTimer = 0.0f;
	nAINextState = state;
}

void cDynamicCreatureWhispyWood::UpdateTimers()
{
	if (nHealth >= 70)
	{
		fWaitingTime = 2.5f;
		fRootSpawnTime = 1.3f;
		fAppleSpawnTime = 2.0f;
	}
	else if (nHealth < 70 && nHealth >= 50)
	{
		fWaitingTime = 2.0f;
		fRootSpawnTime = 1.1f;
		fAppleSpawnTime = 1.6f;
	}
	else if (nHealth < 50 && nHealth >= 30)
	{
		fWaitingTime = 1.5f;
		fRootSpawnTime = 1.0f;
		fAppleSpawnTime = 1.3f;
	}
	else
	{
		fWaitingTime = 1.0f;
		fRootSpawnTime = 0.75f;
		fAppleSpawnTime = 1.0f;
	}
}

void cDynamicCreatureWhispyWood::MapGraphicState(MAP_GRAPHIC_STATE mappedState)
{
	// Mapping state is not the same than other ennemies, so i must remap the states from Whispy wood to regular ennemy
	switch (mappedState)
	{
		case IDLE:
			nGraphicState = STANDING;
			break;
		case SCREAMING:
			nGraphicState = WALKING;
			break;
		case DAMAGE:
			nGraphicState = DAMAGED;
			break;
		case PROJECTILES:
			nGraphicState = JUMPING;
			break;
		case BLOW:
			nGraphicState = UPATTACK;
			break;
	}
}
