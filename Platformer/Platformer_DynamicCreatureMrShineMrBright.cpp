#include "Platformer_DynamicCreatureMrShineMrBright.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureMrShineMrBright::engine = nullptr;

cDynamicCreatureMrShineMrBright::cDynamicCreatureMrShineMrBright(cLevel* l) : cDynamicCreature("mrShineMrBright", cAssets::get().GetSprite("mrShineMrBright"), 16)
{
	fDynWidth = 192.0f;
	fDynHeight = 128.0f;
	fSpriteW = 256.0f;
	fSpriteH = 256.0f;
	fSpriteOffsetX = -32.0f;
	fSpriteOffsetY = -64.0f;
	bFriendly = false;
	nHealth = 100;
	nHealthMax = 100;
	level = l;
	bSolidVsMap = true;
	bAffectedByGravity = false;
	nDamage = 3;
	bCanBehaveWhileAttacked = true;

	srand(time(NULL));
}

void cDynamicCreatureMrShineMrBright::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	nFaceDir = 0; // they don't switch position, sun is always left and moon always right

	switch (nAIState)
	{
		case AI_STATIONARY:
		{
			// they move randomly to a position while waiting
			fMoveTimer += fElapsedTime;
			if (fMoveTimer >= fMoveTime)
			{
				fMoveTimer = 0.0f;

				fMoveLocX = (((float)(rand() % 150) / 10.0f) + 1.0f) - px;
				fMoveLocY = (((float)(rand() % 70) / 10.0f) + 1.0f) - py;
			}
			float fDistance = sqrtf(fMoveLocX * fMoveLocX + fMoveLocY * fMoveLocY);

			if (fDistance > 0.01f)
			{
				vx = (fMoveLocX / fDistance) * fMoveSpeed;
				vy = (fMoveLocY / fDistance) * fMoveSpeed;
			}
			else
			{
				vx = 0.0f; vy = 0.0f;
				px = fMoveLocX; py = fMoveLocY;
			}

			fBehaviourTimer += fElapsedTime;

			bCanSpawnAOE = true;

			if (fBehaviourTimer >= fWaitingTime)
			{
				nChosenAttack = rand() % 4;

				if (nChosenAttack == 0) ChangeState(AI_FLAMECOLUMN);
				if (nChosenAttack == 1) ChangeState(AI_LASER);
				if (nChosenAttack == 2) ChangeState(AI_GUN);
			}
		}
		break;
		case AI_FLAMECOLUMN:
		{
			vx = 0.0f; vy = 0.0f;
			nGraphicState = UPATTACK;

			// Animation loop
			if (bChargingColumn)
			{
				// Loop charging column animation
				if (nGraphicCounter > 1)
					nGraphicCounter = 0;
			}
			else
			{
				// Freeze animation at last frame
				if (nGraphicCounter == nFramesPerSecond - 1)
					bFreezeAnimation = true;

				if (bFreezeAnimation)
					nGraphicCounter = nFramesPerSecond - 1;
			}

			// spawn a little ground effect during fColumnSpawnTime to indicate the player that magma is gonna spawn
			fColumnTimer += fElapsedTime;
			if (bCanSpawnAOE)
			{
				engine->ActivateShakeEffect(true);

				engine->AddProjectile(engine->CreateProjectile(px + 1.8f, 8.6f, true, 0.0f, 0.0f, fColumnSpawnTime, "movingGroundLava", false, 0, false, false));
				bCanSpawnAOE = false;
			}

			if (fColumnTimer >= fColumnSpawnTime && bChargingColumn)
			{
				bChargingColumn = false;

				engine->ActivateShakeEffect(true, 200, 200);

				// Spawn magma to attack kirbo
				engine->AddProjectile(engine->CreateProjectile(px + 1.8f, 0.9f, false, 0.0f, 0.0f, (fColumnTime - fColumnSpawnTime), "magma", false, 3, false, false));

				// Spawn rocks
				engine->AddProjectile(engine->CreateProjectile(px + 1.8f, 8.0f, false, -2.4f, -10.0f, 10, "magmaBoulder", true, 3, true, false));
				engine->AddProjectile(engine->CreateProjectile(px + 2.1f, 7.7f, false, -0.6f, -12.0f, 10, "magmaBoulder", true, 3, true, false));
				engine->AddProjectile(engine->CreateProjectile(px + 2.0f, 8.0f, false, +4.8f, -10.0f, 10, "magmaBoulder", true, 3, true, false));
			}

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fColumnTime)
			{
				bChargingColumn = true;
				bFreezeAnimation = false;
				engine->ActivateShakeEffect(false);
				ChangeState(AI_STATIONARY);
			}
		}
		break;
		case AI_LASER:
		{
			// TODO
			vx = 0.0f; vy = 0.0f;
			nGraphicState = SIDEATTACK;
			nFramesPerSecond = 32; // double the framerate

			// Animation loop
			if (bChargingLaser)
			{
				nGraphicCounter = 0;
			}
			else
			{
				if (nGraphicCounter == 3)
					nGraphicCounter = 1;
			}

			// spawn a little ray during fLaserSpawnTime to indicate the player that magma is gonna spawn
			fLaserTimer += fElapsedTime;
			if (bCanSpawnAOE)
			{
				engine->ActivateShakeEffect(true, 40, 40);

				engine->AddProjectile(engine->CreateProjectile(px + 1.8f, py - 0.2f, true, 0.0f, 0.0f, fLaserSpawnTime, "chargeLaser", false, 0, false, false));
				bCanSpawnAOE = false;
			}

			if (fLaserTimer >= fLaserSpawnTime && bChargingLaser)
			{
				bChargingLaser = false;

				nGraphicCounter = 1;

				engine->ActivateShakeEffect(true, 200, 200);

				// Spawn laser to attack kirbo
				// must offset the laser to give impression it's going out from the moon's mouth
				engine->AddProjectile(engine->CreateProjectile(px - 8.0f, py + 3.0f, false, -0.01f, 0.01f, (fColumnTime - fLaserSpawnTime), "laser", false, 3, false, false));
			}

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fColumnTime)
			{
				nFramesPerSecond = 16; // reset framerate to normal
				bChargingLaser = true;
				engine->ActivateShakeEffect(false);
				ChangeState(AI_STATIONARY);
			}
		}
		break;
		case AI_GUN:
		{
			// TODO
			nGraphicState = DOWNATTACK;

			if (fBehaviourTimer <= 1.0f) // phase 1, they stop moving
			{
				vx = 0.0f; vy = 0.0f;

				// stop at 2nd frame
				if (nGraphicCounter == 2)
					nGraphicCounter = 1;
			}
			else if (fBehaviourTimer > 1.0f && fBehaviourTimer <= 2.0f) // phase 2, the sun shoot the moon
			{
				vx = 0.0f; vy = 0.0f;

				if (nGraphicCounter == 5)
					nFramesPerSecond = 32;

				// loop frames 5 and 6
				if (nGraphicCounter == 7)
					nGraphicCounter = 5;
			}
			else if (fBehaviourTimer > 2.0f && !OnGround()) // phase 3 they fall
			{
				vx = 0.0f; vy = 8.0f;

				// loop frames 5 and 6
				if (nGraphicCounter == 7)
					nGraphicCounter = 5;
			}
			else if (fBehaviourTimer > 2.0f && OnGround()) // phase 4 the sun keep shooting the moon on the ground
			{
				vx = 0.0f; vy = 0.0f;

				// loop frames 7 and 8
				if (nGraphicCounter == 9)
					nGraphicCounter = 7;
			}

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fGunTime)
			{
				nFramesPerSecond = 16;
				bFreezeAnimation = false;
				ChangeState(AI_STATIONARY);
			}
		}
		break;
	}

	UpdateTimers();

	nAIState = nAINextState;
}

void cDynamicCreatureMrShineMrBright::ChangeState(AI_STATE state)
{
	nGraphicCounter = 0;
	fBehaviourTimer = 0.0f;
	fColumnTimer = 0.0f;
	fLaserTimer = 0.0f;
	nAINextState = state;
}

void cDynamicCreatureMrShineMrBright::UpdateTimers()
{
	if (nHealth >= 70)
	{
		fMoveSpeed = 3.5f;
		fColumnSpawnTime = 1.3f;
		fWaitingTime = 2.5f;
	}
	else if (nHealth < 70 && nHealth >= 50)
	{
		fMoveSpeed = 4.2f;
		fColumnSpawnTime = 1.1f;
		fWaitingTime = 2.0f;
	}
	else if (nHealth < 50 && nHealth >= 30)
	{
		fMoveSpeed = 4.6f;
		fColumnSpawnTime = 0.9f;
		fWaitingTime = 1.8f;
	}
	else
	{
		fMoveSpeed = 5.0f;
		fColumnSpawnTime = 0.6f;
		fWaitingTime = 1.5f;
	}
}

bool cDynamicCreatureMrShineMrBright::OnGround()
{
	// lambda fonction to check if there is a hole or a wall to change direction
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < level->GetWidth() && y >= 0 && y < level->GetHeight())
			return level->GetLevel()[y * level->GetWidth() + x];
		else
			return L' ';
	};

	return (engine->IsSolidTile(GetTile(px + 0.1f, py + 2)) || engine->IsSolidTile(GetTile(px + 0.9f, py + 2)));
}
