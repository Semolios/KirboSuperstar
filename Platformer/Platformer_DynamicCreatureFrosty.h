#ifndef DEF_FROSTY
#define DEF_FROSTY

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureFrosty : public cDynamicCreature
{
public:
	cDynamicCreatureFrosty(cLevel* l);
	virtual ~cDynamicCreatureFrosty();

private:
	enum AI_STATE
	{
		AI_IDLE = 0,
		AI_GLIDING,
		AI_ICECUBE,
		AI_ICEBLOW,
		AI_JUMPING,
	} nAIState, nAINextState;

	int nChosenAttack;

	// Standard Behaviour
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;

	bool bCantSpawnAOE = false;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};

#endif // !DEF_FROSTY