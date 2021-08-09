#ifndef DEF_SSTIERMETAKNIGHT
#define DEF_SSTIERMETAKNIGHT

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureSSTierMetaKnight : public cDynamicCreature
{
public:
	cDynamicCreatureSSTierMetaKnight(cLevel* l);
	virtual ~cDynamicCreatureSSTierMetaKnight();

private:
	const float cfMKWidth = 1.0f;
	const float cfOneSSTierMKFrame = 0.03125f;
	const int cnSSTierMKDmg = 4;

	const float cfWaitingTimeMaxHp = 2.5f;
	const float cfWaitingTimeHiHp = 2.0f;
	const float cfWaitingTimeMiHp = 1.8f;
	const float cfWaitingTimeLoHp = 1.5f;

	const float cfWaitBeforeAttackMaxHp = 1.0f;
	const float cfWaitBeforeAttackHiHp = 0.8f;
	const float cfWaitBeforeAttackMiHp = 0.7f;
	const float cfWaitBeforeAttackLoHp = 0.5f;

	const int cnHiHP = 70;
	const int cnMiHP = 50;
	const int cnLoHP = 30;

	// TODO
	enum AI_STATE
	{
		AI_IDLE = 0,
		AI_DASHING,
		AI_HIYAYAYAYA,
		AI_DISAPPEARING,
		AI_UPATTACK,
		AI_TORNADO,
		AI_TELEPORT,
		AI_DOWNAERIAL,
		AI_DOWNTILT,
	} nAIState, nAINextState;

	// Standard Behaviour
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;
	bool bCanSpawnAOE = true;

	// Dash behaviour
	float fKirboOnSameLine = 0.1f;
	float fKirboIsFarPoint = 3.0f;
	float fStayAwayDistance = 1.5f;

	// HIYAYAYAYA behaviour
	float fWaitBeforeAttack = 1.0f;
	float fAttackTime = 1.0f;
	float fAOEWidth = 114.0f / 64.0f;
	float fAOEHeight = 121.0f / 64.0f;
	int nFirstFrameHiyayayaya = 1;
	int nLastFrameHiyayayaya = 4;

	// Disappearing behaviour
	float fDisapearingTime = 0.5f;
	float fHigherLimitToAppear = 2.0f;
	float fLowerLimitToAppear = 5.0f;
	int nFirstFrameDisappear = 0;
	int nLastFrameDisappear = 5;

	// Up and Down attack behaviour
	float fReappearingDistance = 1.5f;
	float fGroundY = 7.0f;
	float fTimeBetweenUpStrikes = 0.09375f; // 3 frames
	float fTimeBetweenDownStrikes = 0.15625f; // 5 frames
	float fSwordAttackTimer = 0.03125f;
	int nFirstFrameReappear = 6;
	int nLastFrameReappear = 9;
	int nFirstFrameUpAttack = 5;
	int nLastFrameUpAttack = 12;
	int nFirstFrameDownAttack = 13;
	int nLastFrameDownAttack = 17;
	bool bPlaySound = true;

	// Tornado Behaviour
	int nFirstFramePreTornado = 0;
	int nLastFramePreTornado = 3;
	int nFirstFrameTornado = 4;
	int nLastFrameTornado = 7;
	int nNbFramesBeforeTornado = 3;

	// Teleport behaviour
	float fKirboPosX = 0.0f;
	float fKirboPosY = 0.0f;
	float fAppearingTime = 0.5f;
	float fAOEDuration = 0.05f;
	int nNbFramesBeforeTPAtk = 3;
	int nFirstFrameTeleport = 8;
	int nLastFrameTeleport = 11;
	bool bUseTPAttack = false;

	// DownTilt behaviour
	float fDownTiltTime = 0.0625f;
	float fTimeBetweenDownTilts = 0.125f; // 4 frames
	int nFirstFrameDownTilt = 1;
	int nLastFrameDownTilt = 4;
public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	void ChangeState(AI_STATE state, bool resetBehaviourTimer = true);
	void UpdateTimers();
	void LoopAnimation(int firstFrame, int lastFrame);
	void SingleAnimation(int firstFrame, int lastFrame);
};

#endif // !DEF_SSTIERMETAKNIGHT