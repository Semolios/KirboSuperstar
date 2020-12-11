#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureBladeKnight : public cDynamicCreature
{
public:
	cDynamicCreatureBladeKnight(cLevel* l);

public:
	const float fFallingAnimationTime = 0.875f;
	const float fAttackTime = 1.0f;
	const float fTimeBeforeDownAttack = 0.5f;

	enum AI_STATE
	{
		AI_WAITING = 0,
		AI_WALKING,
		AI_JUMPING,
		AI_SIDEATTACK,
		AI_DOWNATTACK,
		AI_UPATTACK,
	} nAIState, nAINextState;

	float fTimer = 0.0f;

	bool bOnGround = true;
	bool bCantSpawnAOE1 = false;	// in side attack, blade knight send 2 hits
	bool bCantSpawnAOE2 = false;	// in side attack, blade knight send 2 hits

	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	void ChangeState(AI_STATE state);
};

