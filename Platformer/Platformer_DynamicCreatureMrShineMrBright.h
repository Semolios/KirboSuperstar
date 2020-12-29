#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureMrShineMrBright : public cDynamicCreature
{
public:
	cDynamicCreatureMrShineMrBright(cLevel* l);

public:

	enum AI_STATE
	{
		AI_STATIONARY = 0,
		AI_FLAMECOLUMN,
		AI_LASER,
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
	// Laser attack
	const float fLaserTime = 4.0f;
	float fLaserTimer = 0.0f;
	float fLaserSpawnTime = 1.0f;
	bool bChargingLaser = true;
	// Gun attack
	const float fGunTime = 4.0f;

	bool bCanSpawnAOE = true;

	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	void ChangeState(AI_STATE state);
	void UpdateTimers();
	bool OnGround();
};
