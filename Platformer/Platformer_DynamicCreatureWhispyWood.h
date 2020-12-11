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

	int nStateAfterWait;
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;
	float fRootTimer = 0.0f;
	float fRootSpawnTime = 1.0f;
	float fAppleTimer = 0.0f;
	float fAppleSpawnTime = 1.5f;
	float fScreamTime = 3.0f;

	float rootSpawnX;

	bool bCantSpawnAOE1 = false;	// Whispy wood can blow two times and can spawn two roots
	bool bCantSpawnAOE2 = false;	// Whispy wood can blow two times and can spawn two roots

	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	void ChangeState(AI_STATE state);
	void UpdateTimers();
	void MapGraphicState(MAP_GRAPHIC_STATE mappedState);
};
