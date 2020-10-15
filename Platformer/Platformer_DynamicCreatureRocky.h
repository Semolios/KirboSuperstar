#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureRocky : public cDynamicCreature
{
public:
	cDynamicCreatureRocky(cLevel* l);

public:
	enum AI_STATE
	{
		AI_WAITING = 0,
		AI_FALLING,
		AI_LANDING,
		AI_GOING_BACK_UP,
	} nAIState, nAINextState;

	float fLandingTime = 1.0f;
	float fTimer = 0.0f;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	bool IsSolidTile(wchar_t tile);
	bool IsSemiSolidTile(wchar_t tile);
};

