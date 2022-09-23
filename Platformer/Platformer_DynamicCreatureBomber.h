#ifndef DEF_BOMBER
#define DEF_BOMBER

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class OneLoneCoder_Platformer;

class cDynamicCreatureBomber : public cDynamicCreature
{
public:
	cDynamicCreatureBomber(cLevel* l);
	virtual ~cDynamicCreatureBomber();

private:
	const float cfTriggerDistance = 2.0f;
	const float cfExplosionDuration = 0.561f;
	const float cfKnockBackDuration = 0.3f;
	const int cnExplosionDmg = 5;

	enum AI_STATE
	{
		AI_WALKING = 0,
		AI_CHARGING,
		AI_EXPLODING,
	} nAIState, nAINextState;

	float fChargingTime = 2.0f;
	float fTimer = 0.0f;

	bool bHasExploded = false;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	bool RightObstacle();
	bool LeftObstacle();
};

#endif // !DEF_BOMBER
