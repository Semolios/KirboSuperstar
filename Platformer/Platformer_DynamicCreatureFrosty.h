#ifndef DEF_FROSTY
#define DEF_FROSTY

#pragma once

#include "Platformer_DynamicCreatureSpawnerBoss.h"
#include "Platformer_Assets.h"

class OneLoneCoder_Platformer;

class cDynamicCreatureFrosty : public cDynamicCreatureSpawnerBoss
{
public:
	cDynamicCreatureFrosty(cLevel* l);
	virtual ~cDynamicCreatureFrosty();

	static OneLoneCoder_Platformer* engine;

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	const float cfMinPosX = 0.0f;
	const float cfMaxPosX = 15.0f;

	const float cfWaitingTimeMaxHp = 2.5f;
	const float cfWaitingTimeHiHp = 2.0f;
	const float cfWaitingTimeMiHp = 1.8f;
	const float cfWaitingTimeLoHp = 1.5f;

	const float cfWalkingSpeedMaxHp = 2.0f;
	const float cfWalkingSpeedHiHp = 2.2f;
	const float cfWalkingSpeedMiHp = 2.5f;
	const float cfWalkingSpeedLoHp = 2.8f;

	const float cfGlidingSpeedMaxHp = 2.5f;
	const float cfGlidingSpeedHiHp = 2.8f;
	const float cfGlidingSpeedMiHp = 3.0f;
	const float cfGlidingSpeedLoHp = 3.5f;

	const float cfWaitingBeforeBlowMaxHp = 1.5f;
	const float cfWaitingBeforeBlowHiHp = 1.2f;
	const float cfWaitingBeforeBlowMiHp = 1.0f;
	const float cfWaitingBeforeBlowLoHp = 0.7f;

	const float cfWaitingBeforeFallMaxHp = 0.8f;
	const float cfWaitingBeforeFallHiHp = 0.7f;
	const float cfWaitingBeforeFallMiHp = 0.6f;
	const float cfWaitingBeforeFallLoHp = 0.5f;

	const int cnMaxJumpNumberMaxHp = 1;
	const int cnMaxJumpNumberHiHp = 2;
	const int cnMaxJumpNumberMiHp = 3;
	const int cnMaxJumpNumberLoHp = 4;

	const std::string csIceCubeMaxHp = "tinyIceCube";
	const std::string csIceCubeHiHp = "mediumIceCube";
	const std::string csIceCubeMiHp = "bigIceCube";
	const std::string csIceCubeLoHp = "hugeIceCube";

	const int cnNumberOfAttack = 4;
	const int cnHiHP = 70;
	const int cnMiHP = 50;
	const int cnLoHP = 30;

	const int cnShakeAmplitude = 50;

	enum AI_STATE
	{
		AI_IDLE = 0,
		AI_WALKING,
		AI_GLIDING,
		AI_ICECUBE,
		AI_ICEBLOW,
		AI_JUMPING,
		AI_JUMPED,
		AI_FALLING,
	} nAIState, nAINextState;

	int nChosenAttack;

	// Standard Behaviour
	float fBehaviourTimer = 0.0f;
	float fWaitingTime = 2.5f;

	// Gliding Behaviour
	float fWalkingTime = 1.0f;
	float fGlidingTime = 2.5f;
	float fWalkingSpeed = 2.0f;
	float fGlidingSpeed = 2.5f;
	float fGlideDirection = 1.0f;

	// IceCube behaviour
	float fIceCubeDirection = 1.0f;
	float fIceCubeSpawnTime = 0.875f;
	float fIceCubeAnimationDuration = 1.0f;
	float fIceCubeVX = 7.0f;
	float fIceCubeVY = -7.0f;
	float fIceCubeDuration = 4.0f;
	float fIceCubeDrag = -1.0f;
	float fIceCubeOffset = 0.0f;
	int nIceCubeDmg = 2;
	std::string sIceCube;

	// Iceblow behaviour
	float fWaitBeforeBlow = 1.5f;
	float fBlowTime = 2.0f;
	float fWindOffsetX = -2.0f;
	int nWindDmg = 3;

	// Jumping behaviour
	float fWaitBeforeJump = 0.5f;
	float fWaitBeforeFall = 0.8f;
	float fJumpVX = 5.0f;
	float fJumpVY = -10.0f;
	float fFallVY = 8.0f;
	float fGroundPoundShakeEffectTimer = 0.2f;
	float fGroundPoundTimer = 0.0f;
	int nMaxJumpsNumber = 1;
	int nRemainingJumps = 1;

	bool bCanSpawnAOE = true;

	void ChangeState(AI_STATE state);
	void UpdateTimers();
	bool OnGround();
	void JumpAgainOrReturnIdleState();
};

#endif // !DEF_FROSTY
