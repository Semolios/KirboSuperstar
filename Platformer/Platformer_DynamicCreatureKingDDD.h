#ifndef DEF_KINGDDD
#define DEF_KINGDDD

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureKingDDD : public cDynamicCreature
{
public:
	cDynamicCreatureKingDDD(cLevel* l);
	virtual ~cDynamicCreatureKingDDD();

private:

	const float cfWaitingTimeMaxHp = 2.5f;
	const float cfWaitingTimeHiHp = 2.0f;
	const float cfWaitingTimeMiHp = 1.8f;
	const float cfWaitingTimeLoHp = 1.5f;

	const int cnHiHP = 70;
	const int cnMiHP = 50;
	const int cnLoHP = 30;

	const int cnNumberOfAttack = 5;
	const int cnAttacksDmg = 4;

	enum AI_STATE
	{
		AI_IDLE = 0,
		AI_MOVING,
		AI_JUMPING,
		AI_UPSMASH,
		AI_SIDESMASH,
		AI_DOWNSMASH,
		AI_SIDEB,
		AI_DOWNB,
		AI_UPB,
		AI_VACUUM,
		AI_UPAIR,
		AI_FORWARDAIR,
		AI_BACKWARDAIR,
		AI_DOWNAIR,
	} nAIState, nAINextState;

	int nChosenAttack;

	// Standard Behaviour
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;

	bool bCanSpawnAOE = true;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	bool IsOneFrameAnimation();
	void ChangeState(AI_STATE state, bool resetBehaviourTimer = true);
	void UpdateTimers();
};

#endif // !DEF_KINGDDD