#include "Platformer_DynamicCreatureMrShineMrBright.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureMrShineMrBright::engine = nullptr;

cDynamicCreatureMrShineMrBright::cDynamicCreatureMrShineMrBright(cLevel* l) : cDynamicCreatureSpawnerBoss("mrShineMrBright", cAssets::get().GetSprite("mrShineMrBright"), 16)
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
	bSolidVsMap = false;
	bAffectedByGravity = false;
	nDamage = 1;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureMrShineMrBright::~cDynamicCreatureMrShineMrBright()
{
	delete hitbox;
	delete dynSprite;
	delete level;
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

				fMoveLocX = (((float)(rand() % cnMoveLocXRange) / 10.0f) + 1.0f) - px;
				fMoveLocY = (((float)(rand() % cnMoveLocYRange) / 10.0f) + 1.0f) - py;
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
				nChosenAttack = rand() % cnNumberOfAttack;

				if (!bAlreadyUsedGunLastTime && nChosenAttack == 4)
				{
					bAlreadyUsedGunLastTime = true;
				}
				else if (bAlreadyUsedGunLastTime && nChosenAttack == 4)
				{
					bAlreadyUsedGunLastTime = false;
					nChosenAttack = rand() % (cnNumberOfAttack - 1);
				}
				else if (nChosenAttack != 4)
				{
					bAlreadyUsedGunLastTime = false;
				}

				if (nChosenAttack == 0) ChangeState(AI_FLAMECOLUMN);
				if (nChosenAttack == 1) ChangeState(AI_FLAMECOLUMN);
				if (nChosenAttack == 2) ChangeState(AI_BEAM);
				if (nChosenAttack == 3) ChangeState(AI_BEAM);
				if (nChosenAttack == 4) ChangeState(AI_GUN);
			}
		}
		break;
		case AI_FLAMECOLUMN:
		{
			vx = 0.0f; vy = 0.0f;
			nGraphicState = MOVE1;

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
				engine->PlaySample("earthQuake");
				engine->ActivateShakeEffect(true);

                engine->AddProjectile(px + cfMovingGroundLavaOffsetX, cfMovingGroundLavaPosY, true, 0.0f, 0.0f, fColumnSpawnTime, "movingGroundLava", false, 0, false, false, 0, true, -3.0f, "", false, "", true);
				bCanSpawnAOE = false;
			}

			if (fColumnTimer >= fColumnSpawnTime && bChargingColumn)
			{
				engine->PlaySample("volcanoBoom");

				bChargingColumn = false;

				engine->ActivateShakeEffect(true, cnHighShakeAmplitudeX, cnHighShakeAmplitudeY);

				// Spawn magma to attack kirbo
				engine->AddProjectile(px + cfMagmaOffsetX, cfMagmaPosY, false, 0.0f, 0.0f, (fColumnTime - fColumnSpawnTime), "magma", false, cnMagmaDmg, false, false);

				// Spawn rocks
				engine->AddProjectile(px + cfMagmaRock1OffsetX, cfMagmaRock1PosY, false, cfMagmaRock1VelX, cfMagmaRock1VelY, cfMagmaRockDuration, "magmaBoulder", true, cnMagmaDmg, true, false);
				engine->AddProjectile(px + cfMagmaRock2OffsetX, cfMagmaRock2PosY, false, cfMagmaRock2VelX, cfMagmaRock2VelY, cfMagmaRockDuration, "magmaBoulder", true, cnMagmaDmg, true, false);
				engine->AddProjectile(px + cfMagmaRock3OffsetX, cfMagmaRock3PosY, false, cfMagmaRock3VelX, cfMagmaRock3VelY, cfMagmaRockDuration, "magmaBoulder", true, cnMagmaDmg, true, false);
				engine->AddProjectile(px + cfMagmaRock4OffsetX, cfMagmaRock4PosY, false, cfMagmaRock4VelX, cfMagmaRock4VelY, cfMagmaRockDuration, "magmaBoulder", true, cnMagmaDmg, true, false);
			}

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fColumnTime)
			{
				engine->StopSample("volcanoBoom");
				bChargingColumn = true;
				bFreezeAnimation = false;
				engine->ActivateShakeEffect(false);
				ChangeState(AI_STATIONARY);
			}
		}
		break;
		case AI_BEAM:
		{
			vx = 0.0f; vy = 0.0f;
			nGraphicState = MOVE2;
			nFramesPerSecond = cnDoubledFrameRate;

			// Animation loop
			if (bChargingBeam)
			{
				nGraphicCounter = 0;
			}
			else
			{
				if (nGraphicCounter > cnLastBeamFrame)
					nGraphicCounter = cnFirstBeamFrame;
			}

			// spawn a little ray during fBeamSpawnTime to indicate the player that magma is gonna spawn
			fBeamTimer += fElapsedTime;
			if (bCanSpawnAOE)
			{
				engine->PlaySample("beamCharge");
				engine->ActivateShakeEffect(true, cnLowShakeAmplitudeX, cnLowShakeAmplitudeY);

                engine->AddProjectile(px + cfChargeBeamOffsetX, py + cfChargeBeamOffsetY, false, -0.1f, 0.1f, fBeamSpawnTime, "chargeBeam", false, 0, false, false, 0, true, -3.0f, "", false, "", true);
				bCanSpawnAOE = false;
			}

			if (fBeamTimer >= fBeamSpawnTime && bChargingBeam)
			{
				engine->StopSample("beamCharge");
				engine->PlaySample("beam");

				bChargingBeam = false;

				nGraphicCounter = cnFirstBeamFrame;

				engine->ActivateShakeEffect(true, cnHighShakeAmplitudeX, cnHighShakeAmplitudeY);

				// Spawn beam to attack kirbo
				// must offset the beam to give impression it's going out from the moon's mouth
				engine->AddProjectile(px - cfBeamOffsetX, py + cfBeamOffsetY, false, -0.01f, 0.01f, (fColumnTime - fBeamSpawnTime), "beam", false, cnBeamDmg, false, false);
			}

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fColumnTime)
			{
				nFramesPerSecond = 16; // reset framerate to normal
				bChargingBeam = true;
				engine->ActivateShakeEffect(false);
				ChangeState(AI_STATIONARY);
			}
		}
		break;
		case AI_GUN:
		{
			if (fBehaviourTimer == 0.0f)
				engine->PlaySample("sunShootingMoon");

			nGraphicState = MOVE3;

			if (fBehaviourTimer <= cfSunTakePunchTime) // phase 1, they stop moving
			{
				vx = 0.0f; vy = 0.0f;

				// stop at 2nd frame
				if (nGraphicCounter > cnSunTakePunchLastFrame)
					nGraphicCounter = cnSunTakePunchLastFrame;
			}
			else if (fBehaviourTimer > cfSunTakePunchTime && fBehaviourTimer <= cfSunShootMoonTime) // phase 2, the sun shoot the moon
			{
				vx = 0.0f; vy = 0.0f;

				if (nGraphicCounter == cnMoonScreamingFirstFrame)
					nFramesPerSecond = cnDoubledFrameRate;

				// loop frames 5 and 6
				if (nGraphicCounter > cnMoonScreamingLastFrame)
					nGraphicCounter = cnMoonScreamingFirstFrame;
			}
			else if (fBehaviourTimer > cfSunShootMoonTime && !OnGround()) // phase 3 they fall
			{
				vx = 0.0f; vy = cfFallingSpeed;

				// loop frames 5 and 6
				if (nGraphicCounter > cnMoonScreamingLastFrame)
					nGraphicCounter = cnMoonScreamingFirstFrame;
			}
			else if (fBehaviourTimer > cfSunShootMoonTime && OnGround()) // phase 4 the sun keep shooting the moon on the ground
			{
				engine->PlaySample("sunShootUp", true, true);
				vx = 0.0f; vy = 0.0f;

				// loop frames 7 and 8
				if (nGraphicCounter > cnSunShootingMoonLastFrame)
					nGraphicCounter = cnSunShootingMoonFirstFrame;
			}

			fBehaviourTimer += fElapsedTime;
			if (fBehaviourTimer >= fGunTime)
			{
				engine->StopSample("sunShootUp");

				nFramesPerSecond = cnStandardFramerate;
				bFreezeAnimation = false;
				ChangeState(AI_STATIONARY);
			}
		}
		break;
	}

    if (nHealth > 0)
    {
        // Periodically spawn meteors at a random location
        fMeteorsTimer += fElapsedTime;

        if (fMeteorsTimer >= fMeteorsSpawnTime)
        {
            fMeteorsTimer = 0.0f;

            // meteors spawn between 1 and 16 (MrShineAndMrBright position)
            float fMeteorPosX = ((float)(rand() % cnMeteorPosXRange) / 10.0f) + cfMeteorPosXRangeOffset;

            int nChosenMeteor = rand() % 2;

            // Spawn meteor
            if (nChosenMeteor == 0)
                engine->AddProjectile(fMeteorPosX, cfMeteorPosY, false, +cfMeteorVelX, cfMeteorVelY, cfMeteorDuration, "magmaBoulder", false, cnMeteorDmg, true, false);
            else
                engine->AddProjectile(fMeteorPosX, cfMeteorPosY, false, -cfMeteorVelX, cfMeteorVelY, cfMeteorDuration, "stars", false, cnMeteorDmg, true, false);
        }
    }

	UpdateTimers();

	UpdateSpawn(fElapsedTime);

	nAIState = nAINextState;
}

void cDynamicCreatureMrShineMrBright::ChangeState(AI_STATE state)
{
	nGraphicCounter = 0;
	fBehaviourTimer = 0.0f;
	fColumnTimer = 0.0f;
	fBeamTimer = 0.0f;
	nAINextState = state;
}

void cDynamicCreatureMrShineMrBright::UpdateTimers()
{
	if (nHealth >= cnHiHP)
	{
		fMoveSpeed = cfMoveSpeedMaxHP;
		fColumnSpawnTime = cfColumnSpawnTimeMaxHP;
		fWaitingTime = cfWaitingTimeMaxHP;
		fMeteorsSpawnTime = cfMeteorSpawnTimeMaxHP;
	}
	else if (nHealth < cnHiHP && nHealth >= cnMiHP)
	{
		fMoveSpeed = cfMoveSpeedHiHP;
		fColumnSpawnTime = cfColumnSpawnTimeHiHP;
		fWaitingTime = cfWaitingTimeHiHP;
		fMeteorsSpawnTime = cfMeteorSpawnTimeHiHP;
	}
	else if (nHealth < cnMiHP && nHealth >= cnLoHP)
	{
		fMoveSpeed = cfMoveSpeedMiHP;
		fColumnSpawnTime = cfColumnSpawnTimeMiHP;
		fWaitingTime = cfWaitingTimeMiHP;
		fMeteorsSpawnTime = cfMeteorSpawnTimeMiHP;
	}
	else
	{
		fMoveSpeed = cfMoveSpeedLoHP;
		fColumnSpawnTime = cfColumnSpawnTimeLoHP;
		fWaitingTime = cfWaitingTimeLoHP;
		fMeteorsSpawnTime = cfMeteorSpawnTimeLoHP;
	}
}

bool cDynamicCreatureMrShineMrBright::OnGround()
{
	return py >= cfGroundYPos;
}
