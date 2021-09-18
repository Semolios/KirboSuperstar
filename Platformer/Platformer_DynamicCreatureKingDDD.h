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
	const float cfRunSpeed = 3.0f;
	const float cfJumpSpeed = -10.0f;
	const float cfRecoverJumpSpeed = -25.0f;
	const float cfCollisionLowerLimit = 0.1f;
	const float cfCollisionUpperLimit = 1.9f;

	// Boundaries of the UP area
	const float cfUpAreaLeftPosX = -1.0f;
	const float cfUpAreaRightPosX = 3.0f;
	const float cfUpAreaTopPosY = -2.0f;
	const float cfUpAreaBotPosY = 0.0f;

	// Boundaries of the LEFT area
	const float cfLeftAreaLeftPosX = -2.0f;
	const float cfLeftAreaRightPosX = 0.0f;
	const float cfLeftAreaTopPosY = 0.0f;
	const float cfLeftAreaBotPosY = 2.0f;

	// Boundaries of the RIGHT area
	const float cfRightAreaLeftPosX = 2.0f;
	const float cfRightAreaRightPosX = 4.0f;
	const float cfRightAreaTopPosY = 0.0f;
	const float cfRightAreaBotPosY = 2.0f;

	// Boundaries of the DOWN area
	const float cfDownAreaLeftPosX = -1.0f;
	const float cfDownAreaRightPosX = 3.0f;
	const float cfDownAreaTopPosY = 2.0f;
	const float cfDownAreaBotPosY = 4.0f;

	const float cfWaitingTimeMaxHp = 2.5f;
	const float cfWaitingTimeHiHp = 2.0f;
	const float cfWaitingTimeMiHp = 1.8f;
	const float cfWaitingTimeLoHp = 1.5f;

	const float cfSmashChargeTimeMaxHp = 0.9f;
	const float cfSmashChargeTimeHiHp = 0.8f;
	const float cfSmashChargeTimeMiHp = 0.7f;
	const float cfSmashChargeTimeLoHp = 0.6f;

	const float cfWaitAfterSmashTimeMaxHp = 1.0f;
	const float cfWaitAfterSmashTimeHiHp = 0.95f;
	const float cfWaitAfterSmashTimeMiHp = 0.9f;
	const float cfWaitAfterSmashTimeLoHp = 0.8f;

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
		AI_RECOVERY,
	} nAIState, nAINextState;

	int nChosenAttack;

	// Standard Behaviour
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;
	float fKingDDDSemiWidth = 1.0f;
	float fKingDDDSemiHeight = 1.0f;
	float fKingDDDHeight = 2.0f;

	// Moving Behaviour
	int nFirstFrameRunning = 0;
	int nLastFrameRunning = 15;

	// Jumping Behaviour
	float fKirboNearDistance = 3.0f;
	float fKirboTooHighDistance = 3.0f;
	int nJumpingFrame = 0;
	int nFallingFrame = 1;
	bool bHasNotAlreadyJumped = true;

	// Recovery Behaviour
	float fLowerLimitOfStage = 16.0f;
	float fLeftRecoveryX = 0.0f;
	float fRightRecoveryX = 26.0f;
	float fMiddleOfStage = 14.0f;
	float fWaitAfterRecoveryTime = 1.0f;
	bool bJumpForRecover = true;

	// SMASH Behaviour
	float fSmashChargeTime = 0.9f;
	float fWaitAfterSmashTime = 1.0f;
	float fAOEsDuration = 0.05f;

	float fUpSmashPosX = -1.0f;
	float fUpSmashPosY = -2.0f;

	float fLeftSideSmashPosX = -2.0f;
	float fRightSideSmashPosX = 0.0f;
	float fSideSmashPosY = -2.0f;

	float fDownSmashPosX = -2.0f;
	float fDownSmashPosY = 0.0f;

	int nFirstFrameChargeUpSmash = 0;
	int nLastFrameChargeUpSmash = 26;
	int nFirstFrameUpSmash = 27;
	int nLastFrameUpSmash = 28;

	int nFirstFrameChargeSideSmash = 0;
	int nLastFrameChargeSideSmash = 7;
	int nFirstFrameSideSmash = 8;
	int nLastFrameSideSmash = 9;

	int nFirstFrameChargeDownSmash = 0;
	int nLastFrameChargeDownSmash = 7;
	int nFirstFrameDownSmash = 8;
	int nLastFrameDownSmash = 11;

	int nSmashSide = 0;

	// Side Special Attacks Behaviour
	float fSpikeLeftPX = -1.0f;
	float fSpikeRightPX = 3.0f;
	float fSpikeLeftVX = -3.0f;
	float fSpikeRightVX = 3.0f;
	float fSpikeVY = -6.0f;
	float fSpikeDuration = 2.0f;
	float fSideBSearchSpikeTime = 0.65f;
	float fSideBSpikeFoundTime = 0.35f;
	float fSideBSpikeThrownTime = 0.5f;

	int nFirstFrameSearchSpike = 0;
	int nLastFrameSearchSpike = 9;

	int nFrameSpikeFound = 10;

	int nFirstFrameSpikeThrown = 11;
	int nLastFrameSpikeThrown = 14;

	int nSpecialAttackSide = 0;

	// Down Special Attack Behaviour
	float fDownBWalkSpeed = 1.5f;
	float fDownBPrepareTime = 0.5f;
	float fDownBAttackTime = 1.0f;
	float fDownBAOEPosX = -2.0f;
	float fDownBAOEPosY = -2.0f;

	int nFirstFrameChargeDownB = 0;
	int nLastFrameChargeDownB = 7;

	int nFirstFramePrepareDownB = 8;
	int nLastFramePrepareDownB = 11;

	int nFirstFrameDownBAttack = 12;
	int nLastFrameDownBAttack = 16;

	bool bCloseFromKirbo = false;

	// Up special attack behaviour
	float fUpBJumpVelX = 3.0f;
	float fUpBJumpVelY = -23.0f;
	float fCrouchedDownTime = 0.6f;
	float fUpBLeftLimit = -2.0f;
	float fUpBRightLimit = 3.0f;
	int nCrouchedDownFrame = 0;
	int nUpBJumpFrame = 1;

	enum JumpSide
	{
		LEFT,
		RIGHT,
		MIDDLE,
	} nJumpSide;

	// Vacuum Behaviour
	float fPrepareVacuumTime = 1.0f;
	float fVacuumTime = 2.0f;
	float fSwallowKirboTime = 1.0f;
	float fSpitKirboTime = 1.0f;
	float fVacuumSpeed = 5.0f;

	int nPrepareVacuumFrame = 0;
	int nFirstFrameVacuum = 1;
	int nLastFrameVacuum = 2;
	int nSwallowKirboFrame = 3;
	int nSpitKirboFrame = 4;

	bool bKirboCaught = false;

	enum VACUUMSTATE
	{
		PREPARE,
		VACUUM,
		KIRBOCAUGHT,
		SWALLOW,
		SPIT,
	} nVacuumState, nVacuumNextState;

	// Air attacks Behaviour
	int nAirAttackSide = 0;

	bool bCanSpawnAOE = true;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	bool IsOneFrameAnimation();
	void ChangeState(AI_STATE state, bool resetBehaviourTimer = true);
	void ChangeVacuumState(VACUUMSTATE state);
	void UpdateTimers();
	void LoopAnimation(int firstFrame, int lastFrame);
	void SingleAnimation(int firstFrame, int lastFrame);
	bool OnGround();
	void ReturnMovingStateIfOnGround(float fElapsedTime, bool waitAfterJump = false, float timeToWait = 0.0f, GraphicState graphicState = GraphicState::STANDING);
	void CheckIfHasFallen();
	bool KirboIsInUpArea(float playerX, float playerY);
	bool KirboIsInLeftArea(float playerX, float playerY);
	bool KirboIsInRightArea(float playerX, float playerY);
	bool KirboIsInDownArea(float playerX, float playerY);
	bool KirboIsInFrontOfDDD(float playerX, float playerY);
	bool KirboIsBehindDDD(float playerX, float playerY);
};

#endif // !DEF_KINGDDD