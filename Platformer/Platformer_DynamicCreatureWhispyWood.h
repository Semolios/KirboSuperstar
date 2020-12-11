#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureWhispyWood : public cDynamicCreature
{
public:
	cDynamicCreatureWhispyWood(cLevel* l);

public:

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

	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	void ChangeState(AI_STATE state);
	void UpdateTimers();
	void MapGraphicState(MAP_GRAPHIC_STATE mappedState);
};
