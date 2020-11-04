#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class OneLoneCoder_Platformer;

class cDynamicCreatureBomber : public cDynamicCreature
{
public:
	cDynamicCreatureBomber(cLevel* l);

public:
	enum AI_STATE
	{
		AI_WALKING = 0,
		AI_CHARGING,
		AI_EXPLODING,
	} nAIState, nAINextState;

	float fChargingTime = 2.0f;
	float fTimer = 0.0f;

	bool bHasExploded = false;

	std::vector<olc::Sprite*> explosionSprite;

	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};
