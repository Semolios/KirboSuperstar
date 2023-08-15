#include "Platformer_DynamicCreatureWhispyWood.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureWhispyWood::engine = nullptr;

cDynamicCreatureWhispyWood::cDynamicCreatureWhispyWood(cLevel* l) : cDynamicCreatureSpawnerBoss("whispyWood", cAssets::get().GetSprite("whispyWood"), 4)
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
	nDamage = 1;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureWhispyWood::~cDynamicCreatureWhispyWood()
{
	delete hitbox;
	delete dynSprite;
	delete level;
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
			fProjectilesTimer = cfProjectilesStartTimer;

			if (fBehaviourTimer >= fWaitingTime)
			{
				nChosenAttack = rand() % cnNumberOfAttack;

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

			if (fBehaviourTimer == 0.0f)
				engine->PlaySample("whispyScream");

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fScreamTime)
			{
				engine->ActivateShakeEffect(false);
				engine->StopSample("wetSlap");
				ChangeState(AI_WAITING);
			}

			// spawn a little ground effect during fRootSpawnTime to indicate the player that a root is gonna spawn
			fRootTimer += fElapsedTime;
			if (!bCantSpawnAOE)
			{
				rootSpawnX = playerX;
				engine->PlaySample("earthQuake");
				engine->AddProjectile(playerX, cfGroundPosition, true, 0.0f, 0.0f, fRootSpawnTime, "movingGround", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
				bCantSpawnAOE = true;
			}

			if (fRootTimer >= fRootSpawnTime)
			{
				fRootTimer = 0.0f;

				// spawn moving ground only if the root has the time to spawn
				if ((fBehaviourTimer + fRootSpawnTime) <= fScreamTime)
					bCantSpawnAOE = false;

				// Spawn the root to attack kirbo
				engine->PlaySample("wetSlap", false, true);
				engine->AddProjectile(rootSpawnX, cfRootPosY, false, 0.0f, 0.0f, cfRootDuration, "root", false, cnRootDmg, false, false, cnCorner);
			}
		}
		break;
		case AI_WIND:
		{
			if (fBehaviourTimer == 0.0f)
				engine->PlaySample("inhale");

			fBehaviourTimer += fElapsedTime;

			MapGraphicState(BLOW);

			if (fBehaviourTimer >= cfBlowingAnimationTime)
				engine->WindEffect(cfWindDirection, cfWindPower, true);

			if (fBehaviourTimer >= cfBlowingTime)
			{
				engine->WindEffect(0.0f, 0.0f, false);
				ChangeState(AI_WAITING);
			}

			// Freeze animation at the last frame to prevent looping animation while blowing
			if (fBehaviourTimer >= cfBlowingAnimationTime) nGraphicCounter = nFramesPerSecond - 1;
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
				float fProjectileSpeedY = ((float)(rand() % cnProjectilesYRange) / 10.0f) + cfProjectilesYRangeOffset;

				// Spawn the projectile to attack kirbo
				engine->PlaySample("blow");
				engine->AddProjectile(cfProjectilePosX, cfProjectilePosY, false, cfProjectileSpeedX, fProjectileSpeedY, cfProjectileDuration, "blow", false, cnProjectileDmg, false, false, 0, true, -3.0f, "", false, "", false, "", 0.0f, false, true);
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
		float fApplePosX = ((float)(rand() % cnApplePosXRange) / 10.0f) + cfApplePosXRangeOffset;

		// Spawn apple
		engine->AddProjectile(fApplePosX, cfApplePosY, false, 0.0f, 0.0f, cfAppleDuration, "apple", true, cnAppleDmg, true, false);
	}

	// if he dies while blowing, the wind stops
	if (nHealth <= 0)
		engine->WindEffect(0.0f, 0.0f, false);

	UpdateTimers();

	UpdateSpawn(fElapsedTime);

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
	if (nHealth >= cnHiHP)
	{
		fWaitingTime = cfWaitingTimeMaxHP;
		fRootSpawnTime = cfRootSpawnTimeMaxHP;
		fAppleSpawnTime = cfAppleSpawnTimeMaxHP;
	}
	else if (nHealth < cnHiHP && nHealth >= cnMiHP)
	{
		fWaitingTime = cfWaitingTimeHiHP;
		fRootSpawnTime = cfRootSpawnTimeHiHP;
		fAppleSpawnTime = cfAppleSpawnTimeHiHP;
	}
	else if (nHealth < cnMiHP && nHealth >= cnLoHP)
	{
		fWaitingTime = cfWaitingTimeMiHP;
		fRootSpawnTime = cfRootSpawnTimeMiHP;
		fAppleSpawnTime = cfAppleSpawnTimeMiHP;
	}
	else
	{
		fWaitingTime = cfWaitingTimeLoHP;
		fRootSpawnTime = cfRootSpawnTimeLoHP;
		fAppleSpawnTime = cfAppleSpawnTimeLoHP;
	}
}

void cDynamicCreatureWhispyWood::MapGraphicState(MAP_GRAPHIC_STATE mappedState)
{
	// Mapping state is not the same than other ennemies, so i must remap the states from Whispy wood to regular ennemy
	switch (mappedState)
	{
		case IDLE:		  nGraphicState = STANDING; break;
		case SCREAMING:   nGraphicState = WALKING;	break;
		case DAMAGE:	  nGraphicState = DAMAGED;	break;
		case PROJECTILES: nGraphicState = JUMPING;	break;
		case BLOW:		  nGraphicState = MOVE1;	break;
	}
}
