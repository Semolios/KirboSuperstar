#ifndef DEF_MR_SHINE_MR_BRIGHT
#define DEF_MR_SHINE_MR_BRIGHT

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureMrShineMrBright : public cDynamicCreature
{
public:
	cDynamicCreatureMrShineMrBright(cLevel* l);
	virtual ~cDynamicCreatureMrShineMrBright();

	static OneLoneCoder_Platformer* engine;

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	const float cfGroundYPos = 7.0f;
	const float cfMovingGroundLavaOffsetX = 1.8f;
	const float cfMovingGroundLavaPosY = 8.6f;
	const float cfMagmaOffsetX = 1.8f;
	const float cfMagmaPosY = 0.9f;

	const float cfMagmaRock1OffsetX = 1.8f;
	const float cfMagmaRock1PosY = 8.0f;
	const float cfMagmaRock1VelX = -3.5f;
	const float cfMagmaRock1VelY = -7.0f;

	const float cfMagmaRock2OffsetX = 2.1f;
	const float cfMagmaRock2PosY = 7.7f;
	const float cfMagmaRock2VelX = -2.6f;
	const float cfMagmaRock2VelY = -12.0f;

	const float cfMagmaRock3OffsetX = 2.0f;
	const float cfMagmaRock3PosY = 8.0f;
	const float cfMagmaRock3VelX = 6.0f;
	const float cfMagmaRock3VelY = -13.0f;

	const float cfMagmaRock4OffsetX = 1.9f;
	const float cfMagmaRock4PosY = 7.7f;
	const float cfMagmaRock4VelX = 5.0f;
	const float cfMagmaRock4VelY = -10.0f;

	const float cfMagmaRockDuration = 10.0f;

	const float cfChargeBeamOffsetX = 0.2f;
	const float cfChargeBeamOffsetY = 0.5f;
	const float cfBeamOffsetX = 8.0f;
	const float cfBeamOffsetY = 3.0f;
	const float cfSunTakePunchTime = 1.0f;
	const float cfSunShootMoonTime = 2.0f;
	const float cfFallingSpeed = 10.0f;

	const float cfMoveSpeedMaxHP = 3.5f;
	const float cfMoveSpeedHiHP = 4.2f;
	const float cfMoveSpeedMiHP = 4.6f;
	const float cfMoveSpeedLoHP = 5.0f;

	const float cfColumnSpawnTimeMaxHP = 1.3f;
	const float cfColumnSpawnTimeHiHP = 1.1f;
	const float cfColumnSpawnTimeMiHP = 0.9f;
	const float cfColumnSpawnTimeLoHP = 0.6f;

	const float cfWaitingTimeMaxHP = 2.5f;
	const float cfWaitingTimeHiHP = 2.0f;
	const float cfWaitingTimeMiHP = 1.8f;
	const float cfWaitingTimeLoHP = 1.5f;

	const int cnMoveLocXRange = 150;
	const int cnMoveLocYRange = 70;
	const int cnNumberOfAttack = 5; // reduce the GUN attack ratio, too easy free dps
	const int cnLowShakeAmplitudeX = 40;
	const int cnLowShakeAmplitudeY = 40;
	const int cnHighShakeAmplitudeX = 200;
	const int cnHighShakeAmplitudeY = 200;
	const int cnMagmaDmg = 3;
	const int cnDoubledFrameRate = 32;
	const int cnStandardFramerate = 16;
	const int cnLastBeamFrame = 2;
	const int cnFirstBeamFrame = 1;
	const int cnBeamDmg = 3;
	const int cnSunTakePunchLastFrame = 1;
	const int cnMoonScreamingFirstFrame = 5;
	const int cnMoonScreamingLastFrame = 6;
	const int cnSunShootingMoonFirstFrame = 7;
	const int cnSunShootingMoonLastFrame = 8;
	const int cnHiHP = 70;
	const int cnMiHP = 50;
	const int cnLoHP = 30;

	enum AI_STATE
	{
		AI_STATIONARY = 0,
		AI_FLAMECOLUMN,
		AI_BEAM,
		AI_GUN,
	} nAIState, nAINextState;

	int nChosenAttack;
	// Standard Behaviour
	const float fMoveTime = 0.7f;
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;
	float fMoveLocX = 7.5f;
	float fMoveLocY = 3.0f;
	float fMoveSpeed = 3.5f;
	float fMoveTimer = 0.0f;
	bool bFreezeAnimation = false;
	// Moving Behaviour
	const float fMovingTime = 2.5f;
	int nChosenTrajectory;
	bool bTrajectoryChosen = false;
	// Flame column attack
	const float fColumnTime = 3.0f;
	float fColumnTimer = 0.0f;
	float fColumnSpawnTime = 1.0f;
	bool bChargingColumn = true;
	// Beam attack
	const float fBeamTime = 4.0f;
	float fBeamTimer = 0.0f;
	float fBeamSpawnTime = 1.0f;
	bool bChargingBeam = true;
	// Gun attack
	const float fGunTime = 4.0f;
	// meteors spawn
	const float cfMeteorPosXRangeOffset = 1.0f;
	const float cfMeteorPosY = 0.0f;
	const float cfMeteorDuration = 10.0f;
	const float cfMeteorVelX = 5.0f;
	const float cfMeteorVelY = 5.0f;
	const float cfMeteorSpawnTimeMaxHP = 2.0f;
	const float cfMeteorSpawnTimeHiHP = 1.6f;
	const float cfMeteorSpawnTimeMiHP = 1.3f;
	const float cfMeteorSpawnTimeLoHP = 1.0f;
	const int cnMeteorPosXRange = 150;
	const int cnMeteorDmg = 1;
	float fMeteorsTimer = 0.0f;
	float fMeteorsSpawnTime = 1.5f;

	bool bCanSpawnAOE = true;
	bool bAlreadyUsedGunLastTime = false;

	void ChangeState(AI_STATE state);
	void UpdateTimers();
	bool OnGround();
};

#endif // !DEF_MR_SHINE_MR_BRIGHT
