#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class OneLoneCoder_Platformer;

class cDynamicCreatureRocky : public cDynamicCreature
{
public:
	cDynamicCreatureRocky(cLevel* l);

public:
	const float cfRockyLowerBoundary = 0.1f;
	const float cfRockyUpperBoundary = 0.9f;
	const float cfGoingBackUpSpeed = -2.0f;

	enum AI_STATE
	{
		AI_WAITING = 0,
		AI_FALLING,
		AI_LANDING,
		AI_GOING_BACK_UP,
	} nAIState, nAINextState;

	float fLandingTime = 1.0f;
	float fTimer = 0.0f;

	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};

