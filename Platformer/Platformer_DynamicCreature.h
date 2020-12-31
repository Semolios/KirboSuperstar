#pragma once
#include "Platformer_Dynamic.h"

class cLevel;

class OneLoneCoder_Platformer;

class cDynamicCreature : public cDynamic
{
public:
	cDynamicCreature(std::string n, olc::Sprite* sprite, int framesPerSecond);

private:
	// Constant values
	const float cfKnockBackDX = 10.0f;
	const float cfKnockBackDY = 10.0f;
	const float cfVacuumSpeed = 5.0f;
	const float cfSwallowableLimit = 0.5f;
	const float cfExplosionDuration = 0.85f;

protected:
	olc::Sprite* sSprite;
	float fTimer;
	int nGraphicCounter = 0;
	int nFramesPerSecond;
	int nFaceDir = 0; // 0 = Left, 1 = Right

	enum
	{
		STANDING,
		WALKING,
		DAMAGED,
		JUMPING,
		UPATTACK,
		SIDEATTACK,
		DOWNATTACK
	} nGraphicState;

public:
	cLevel* level = nullptr; // The Dynamic must be aware of the level to interact with (i.e. turn when encounter a wall or a hole)
	int nHealth;
	int nHealthMax;
	bool bDead = false;
	bool bAffectedByGravity = false;
	bool bIsKnockable = true;
	bool bCanBehaveWhileAttacked = false;
	bool bIsVacuumable = true;
	bool bVacuumed = false;
	bool bSwallowable = false;
	bool bIsBoss = false;
	bool bBossKilled = false;
	bool bBossExplosionAvailable = true;
	float fInitSpeed;

	static OneLoneCoder_Platformer* engine;

public:
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx) override;
	void KnockBack(float dx, float dy, float dist);
	void TurnAround();

	virtual void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	virtual void ExplodeAndDie(float fElapsedTime);

protected:
	float fKnockBackTimer = 0.0f;
	float fKnockBackDX = 0.0f;
	float fKnockBackDY = 0.0f;
	float fDeadTimer = 0.0f;
};

