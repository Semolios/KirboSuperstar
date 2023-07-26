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

	static OneLoneCoder_Platformer* engine;

	void InitDecal();
	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void GeneratePickUp();
	void KnockBack(float dx, float dy, float dist);
	void TurnBack();
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
	float GetNormalizedW();
	float GetNormalizedH();
	void AlwaysGeneratePickUp(bool alwaysGenerate);

	virtual void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	virtual void ExplodeAndDie(float fElapsedTime);

private:
	// Constant values
	const float cfKnockBackDX = 10.0f;
	const float cfKnockBackDY = 10.0f;
	const float cfVacuumSpeed = 5.0f;
	const float cfSwallowableLimit = 0.6f;
	const float cfExplosionDuration = 0.528f;

protected:
	olc::Sprite* dynSprite;
	olc::Decal*  dynDecal;
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
	bool bHitShake = true;
	bool bCanBehaveWhileAttacked = false;
	bool bIsVacuumable = true;
	bool bVacuumed = false;
	bool bSwallowable = false;
	bool bIsBoss = false;
	bool bBossKilled = false;
	bool bBossExplosionAvailable = true;
	bool bLoopAnimation = true;
	bool bCanGeneratePickUp = true;
	bool bAlwaysGeneratePickUp = false;

	enum GraphicState
	{
		STANDING,
		WALKING,
		DAMAGED,
		JUMPING,
		MOVE1,	  // Lines 9-10  of the spritesheet
		MOVE2,	  // Lines 11-12 of the spritesheet
		MOVE3,	  // Lines 13-14 of the spritesheet
		MOVE4,	  // Lines 15-16 of the spritesheet
		MOVE5,	  // Lines 17-18 of the spritesheet
		MOVE6,	  // Lines 19-20 of the spritesheet
		MOVE7,	  // Lines 21-22 of the spritesheet
		MOVE8,	  // Lines 23-24 of the spritesheet
		MOVE9,	  // Lines 25-26 of the spritesheet
		MOVE10,	  // Lines 27-28 of the spritesheet
		MOVE11,	  // Lines 29-30 of the spritesheet
		MOVE12,	  // Lines 31-32 of the spritesheet
		MOVE13,	  // Lines 33-34 of the spritesheet
		MOVE14,	  // Lines 35-36 of the spritesheet
		MOVE15,	  // Lines 37-38 of the spritesheet
	} nGraphicState;
};

#endif // !DEF_DYNAMIC_CREATURE
