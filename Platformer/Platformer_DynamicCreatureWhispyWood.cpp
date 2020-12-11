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
	nHealth = 50;
	nHealthMax = 50;
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

			bCantSpawnAOE1 = false;
			bCantSpawnAOE2 = false;

			if (fBehaviourTimer >= fWaitingTime)
			{
				nStateAfterWait = rand() % 3;

				// TODO PENSER A REMETTRE LE STATE RANDOM
				//if (nStateAfterWait == 0) ChangeState(AI_SCREAMING);
				//if (nStateAfterWait == 1) ChangeState(AI_WIND);
				//if (nStateAfterWait == 2) ChangeState(AI_PROJECTILES);
				ChangeState(AI_SCREAMING);
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
			if (!bCantSpawnAOE1)
			{
				rootSpawnX = playerX;
				engine->AddProjectile(engine->CreateProjectile(playerX, 9, false, 0.0f, 0.0f, fRootSpawnTime, "movingGround", 64.0f, 16.0f, false, 0, false, false));
				bCantSpawnAOE1 = true;
			}

			if (fRootTimer >= fRootSpawnTime)
			{
				fRootTimer = 0.0f;

				// spawn moving ground only if the root has the time to spawn
				if ((fBehaviourTimer + fRootSpawnTime) <= fScreamTime)
					bCantSpawnAOE1 = false;

				// Spawn the root to attack kirbo
				engine->AddProjectile(engine->CreateProjectile(rootSpawnX, 7, false, 0.0f, 0.0f, 0.6f, "root", 32.0f, 128.0f, false, 3, false, false));
			}
		}
		break;
		case AI_WIND:
		{

		}
		break;
		case AI_PROJECTILES:
		{

		}
		break;
	}

	// Periodically spawn apples at a random location
	fAppleTimer += fElapsedTime;

	if (fAppleTimer >= fRootSpawnTime)
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
	if (nHealth >= 35)
	{
		fWaitingTime = 2.5f;
		fRootSpawnTime = 1.3f;
		fAppleSpawnTime = 2.0f;
	}
	else if (nHealth < 35 && nHealth >= 25)
	{
		fWaitingTime = 2.0f;
		fRootSpawnTime = 1.1f;
		fAppleSpawnTime = 1.6f;
	}
	else if (nHealth < 25 && nHealth >= 15)
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
