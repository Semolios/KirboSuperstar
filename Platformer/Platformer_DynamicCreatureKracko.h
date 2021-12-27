#ifndef DEF_KRACKO
#define DEF_KRACKO

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureKracko : public cDynamicCreature
{
public:
	cDynamicCreatureKracko(cLevel* l);
	virtual ~cDynamicCreatureKracko();

private:
	const float pi = 3.14159f;

	const float cfInitPX = 4.0f;
	const float cfMiddleOfKracko = 1.0f; // 1.0f is not really the middle of kracko but when kirbo is in the middle of kracko he is a t px + 1.0
	const float cfKirboIsNearDistance = 3.0f;
	const float cfLBoundOfStage = 1.0f;
	const float cfRBoundOfStage = 14.0f;

	const float cfWaitingTimeMaxHp = 2.5f;
	const float cfWaitingTimeHiHp = 2.0f;
	const float cfWaitingTimeMiHp = 1.8f;
	const float cfWaitingTimeLoHp = 1.5f;

	const float cfMovingSpeedMaxHp = 2.5f;
	const float cfMovingSpeedHiHp = 2.8f;
	const float cfMovingSpeedMiHp = 3.0f;
	const float cfMovingSpeedLoHp = 3.5f;

	const float cfAimingTimeMaxHp = 3.0f;
	const float cfAimingTimeHiHp = 2.5f;
	const float cfAimingTimeMiHp = 2.0f;
	const float cfAimingTimeLoHp = 1.5f;

	const float cfFireTimeMaxHp = 1.0f;
	const float cfFireTimeHiHp = 0.9f;
	const float cfFireTimeMiHp = 0.7f;
	const float cfFireTimeLoHp = 0.5f;

	const int cnHiHP = 70;
	const int cnMiHP = 50;
	const int cnLoHP = 30;

	const int cnNumberOfAttack = 5;
	const int cnAttacksDmg = 4;

	enum AI_STATE
	{
		AI_IDLE = 0,
		AI_MOVING,
		AI_THUNDERUNDER,
		AI_SWIPEAREA,
		AI_AIMKIRBO,
		AI_THUNDERSTORM,
		AI_LIGHTNINGAROUND,
	} nAIState, nAINextState;

	int nChosenAttack;

	// Standard Behaviour
	float fBehaviourTimer = 0.0f;
	float fBehaviourTimer2 = 0.0f; // a second timer to prevent the player from cheesing kracko by passing under and reseting his timer with 'Thunder under' attack
	float fWaitingTime = 2.5f;

	// Moving Behaviour
	float fMovingSpeed = 2.0f;

	// Thunder under Behaviour
	float fThunderOffsetX = 0.5f;
	float fThunderOffsetY = 1.0f;
	float fThunderSpawnTime = 0.5f;
	float fThunderUnderTime = 1.0f;
	float fThunderUnderSpawnTime = 0.05f;

	// Swipe area Behaviour
	float fPlungeDirection = 1.0f;
	float fSwipeAmplitude = 10.0f;
	float fSwipeTime = 1.0f;

	// Aim kirbo Behaviour
	float fAimingTime = 3.0f;
	float fFireTime = 1.0f;
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;
	float fLightningSemiWidth = 600.0f;
	float fLightningSemiHeight = 16.0f;
	float fLightningOffsetX = 1.5f;
	float fAimingLightningDuration = 0.05f;

	// Thunder storm Behaviour
	float fLightningPosX1 = 0.0f;
	float fLightningPosX2 = 0.0f;
	float fLightningPosY = -5.0f;
	float fGroundPositionY = 8.0f;
	float fNumberOfAttacks = 3.0f;
	float fLightningSpdY = 100.0f;
	float fLightningDuration = 0.5f;
	bool bFireLastLightning = false;

	// Lightning around Behaviour
	float fLightningAroundTime = 2.0f;
	float fLightningSprSemiW = 0.125f;
	float fLightningSprSemiH = 1.5f;

	float fInitLightning1X = +1.5f;
	float fInitLightning1Y = -1.5f;

	float fInitLightning2X = +4.0f;
	float fInitLightning2Y = +1.0f;

	float fInitLightning3X = +1.5f;
	float fInitLightning3Y = +3.5f;

	float fInitLightning4X = -1.0f;
	float fInitLightning4Y = +1.0f;

	float fAngrot = 3.141859f / 2.0f;

	bool bCanSpawnAOE = true;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

	void AimForKirboUnder(float playerX, float playerY);

private:
	void ChangeState(AI_STATE state, bool resetBehaviourTimer = true);
	void UpdateTimers();
	void LookKirbo(float playerX);
};

#endif // !DEF_KRACKO
