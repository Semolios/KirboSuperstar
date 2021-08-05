#ifndef DEF_DYNAMIC_CREATURE
#define DEF_DYNAMIC_CREATURE

#pragma once
#include "Platformer_Dynamic.h"

class cLevel;

class OneLoneCoder_Platformer;

class cDynamicCreature : public cDynamic
{
public:
	cDynamicCreature(std::string n, olc::Sprite* sprite, int framesPerSecond);
	virtual ~cDynamicCreature();

private:
	// Constant values
	const float cfKnockBackDX = 10.0f;
	const float cfKnockBackDY = 10.0f;
	const float cfVacuumSpeed = 5.0f;
	const float cfSwallowableLimit = 0.5f;
	const float cfExplosionDuration = 0.85f;

protected:
	olc::Sprite* sSprite;
	cLevel* level = nullptr; // The Dynamic must be aware of the level to interact with (i.e. turn when encounter a wall or a hole)
	float fTimer;
	float fInitSpeed;
	float fKnockBackTimer = 0.0f;
	float fKnockBackDX = 0.0f;
	float fKnockBackDY = 0.0f;
	float fDeadTimer = 0.0f;
	int nHealth;
	int nHealthMax;
	int nGraphicCounter = 0;
	int nFramesPerSecond;
	int nFaceDir = 0; // 0 = Left, 1 = Right
	bool bDead = false;
	bool bIsKnockable = true;
	bool bCanBehaveWhileAttacked = false;
	bool bIsVacuumable = true;
	bool bVacuumed = false;
	bool bSwallowable = false;
	bool bIsBoss = false;
	bool bBossKilled = false;
	bool bBossExplosionAvailable = true;
	bool bLoopAnimation = true;

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

	static OneLoneCoder_Platformer* engine;

public:
	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void KnockBack(float dx, float dy, float dist);
	void TurnAround();
	void Collision(float fElapsedTime);
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;
	void Vacuumed(bool vaccumedState);
	int GetHealth();
	void TakeDamage(int damage);
	int GetGraphicCounter();
	void SetGraphicCounter(int counter);
	bool IsVacuumable();
	void SetVacuumable(bool vacuumable);
	bool IsVacuumed();
	bool IsSwallowable();
	void SetSwallowable(bool swallowable);
	bool IsBoss();
	void SetBoss(bool boss);
	bool IsKnockable();
	void SetKnockable(bool knockable);
	bool IsDead();

	virtual void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	virtual void ExplodeAndDie(float fElapsedTime);
};

#endif // !DEF_DYNAMIC_CREATURE