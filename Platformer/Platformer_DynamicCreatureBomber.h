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

	static OneLoneCoder_Platformer* engine;

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	bool RightObstacle();
	bool LeftObstacle();

private:
	const float cfTriggerDistance = 2.0f;
	const float cfExplosionDuration = 0.528f;
	const float cfExplosionNoHarmingDuration = 0.132f;
	const float cfKnockBackDuration = 0.3f;
	const int cnExplosionDmg = 3;

	enum AI_STATE
	{
		AI_WALKING = 0,
		AI_CHARGING,
		AI_EXPLODING,
	} nAIState, nAINextState;

	float fChargingTime = 2.0f;
	float fTimer = 0.0f;

	bool bHasExploded = false;
};

#endif // !DEF_BOMBER
