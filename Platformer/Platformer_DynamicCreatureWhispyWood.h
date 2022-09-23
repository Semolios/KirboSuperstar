#ifndef DEF_WHISPY_WOOD
#define DEF_WHISPY_WOOD

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureWhispyWood : public cDynamicCreature
{
public:
	cDynamicCreatureWhispyWood(cLevel* l);
	virtual ~cDynamicCreatureWhispyWood();

private:
	const float cfProjectilesStartTimer = 0.25f;
	const float cfGroundPosition = 9.0f;
	const float cfRootPosY = 9.1f;
	const float cfRootDuration = 0.396f;
	const float cfWindDirection = -1.0f;
	const float cfWindPower = 20.0f;
	const float cfProjectilesYRangeOffset = -5.0f;
	const float cfProjectilePosX = 14.0f;
	const float cfProjectilePosY = 7.5f;
	const float cfProjectileSpeedX = -10.0f;
	const float cfProjectileDuration = 2.0f;
	const float cfApplePosXRangeOffset = 1.0f;
	const float cfApplePosY = 2.0f;
	const float cfAppleDuration = 10.0f;

	const float cfWaitingTimeMaxHP = 2.5f;
	const float cfWaitingTimeHiHP = 2.0f;
	const float cfWaitingTimeMiHP = 1.5f;
	const float cfWaitingTimeLoHP = 1.0f;

	const float cfRootSpawnTimeMaxHP = 1.3f;
	const float cfRootSpawnTimeHiHP = 1.1f;
	const float cfRootSpawnTimeMiHP = 0.9f;
	const float cfRootSpawnTimeLoHP = 0.6f;

	const float cfAppleSpawnTimeMaxHP = 2.0f;
	const float cfAppleSpawnTimeHiHP = 1.6f;
	const float cfAppleSpawnTimeMiHP = 1.3f;
	const float cfAppleSpawnTimeLoHP = 1.0f;

	const int cnNumberOfAttack = 3;
	const int cnRootDmg = 3;
	const int cnCorner = 3;
	const int cnProjectilesYRange = 100;
	const int cnProjectileDmg = 1;
	const int cnApplePosXRange = 150;
	const int cnAppleDmg = 1;
	const int cnHiHP = 70;
	const int cnMiHP = 50;
	const int cnLoHP = 30;

	enum AI_STATE
	{
		AI_WAITING = 0,
		AI_SCREAMING,
		AI_WIND,
		AI_PROJECTILES,
	} nAIState, nAINextState;

	enum MAP_GRAPHIC_STATE
	{
		IDLE,
		SCREAMING,
		DAMAGE,
		PROJECTILES,
		BLOW,
	} nMappedGraphicState;

	int nChosenAttack;
	// Standard Behaviour
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;
	// Root attack
	const float fScreamTime = 3.0f;
	float fRootTimer = 0.0f;
	float fRootSpawnTime = 1.0f;
	// Apple spawn
	float fAppleTimer = 0.0f;
	float fAppleSpawnTime = 1.5f;
	// Wind, push kirbo away
	const float cfBlowingAnimationTime = 0.75f;
	const float cfBlowingTime = 3.25f;
	// Blow projectiles to attack kirbo
	const float cfProjectilesTime = 2.0f;
	const float cfProjectilesSpawnTime = 0.5f;
	float fProjectilesTimer = 0.25f; // first projectile spawn at 0.25, the other spawn each 0.5 seconds

	float rootSpawnX;

	bool bCantSpawnAOE = false;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	void ChangeState(AI_STATE state);
	void UpdateTimers();
	void MapGraphicState(MAP_GRAPHIC_STATE mappedState);
};

#endif // !DEF_WHISPY_WOOD
