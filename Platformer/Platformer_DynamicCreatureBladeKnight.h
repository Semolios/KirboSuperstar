#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureBladeKnight : public cDynamicCreature
{
public:
	cDynamicCreatureBladeKnight(cLevel* l);
	virtual ~cDynamicCreatureBladeKnight();

private:
	const float cfFallingAnimationTime = 0.875f;
	const float cfAttackTime = 1.0f;
	const float cfTimeBeforeDownAttack = 0.5f;
	const float cfTriggerDistance = 5.0f;
	const float cfAttackTriggerDistance = 1.5f;
	const float cfRunSpeed = 2.5f;
	const float cfJumpSpeed = -10.0f;
	const float cfDownAtkSpeed = 8.0f;
	const float cfCollisionLowerLimit = 0.1f;
	const float cfCollisionUpperLimit = 0.9f;
	const float cfFirstAttackTime = 0.375f;
	const float cfSecondAttackTime = 0.625f;
	const float cfAttackDash = 0.75f;
	const float cfDownAOESprOffsetX = 0.25f;
	const float cfDownAOESprOffsetY = 0.25f;
	const float cfUpAOESprOffsetX = 0.25f;
	const float cfUpAOESprOffsetY = -0.5f;
	const float cfUpAtkSpeed = -10.0f;
	const float cfUpAtkDuration = 0.1f;
	const float cfDownAtkAOEDuration = 10.0f;
	const int cnDownAtkDmg = 5;
	const int cnUpAtkDmg = 5;

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

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	void ChangeState(AI_STATE state);
};

