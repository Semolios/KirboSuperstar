#pragma once
#include "Platformer_Dynamic.h"

class cLevel;

class cDynamicCreature : public cDynamic
{
public:
	cDynamicCreature(std::string n, olc::Sprite* sprite, int framesPerSecond);

private:
	// Constant values
	const float cfKnockBackDX = 10.0f;
	const float cfKnockBackDY = 10.0f;

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
		DAMAGED
	} nGraphicState;

public:
	cLevel* level = nullptr; // The Dynamic must be aware of the level to interact with (i.e. turn when encounter a wall or a hole)
	int nHealth;
	int nHealthMax;
	bool bDead = false;
	float fInitSpeed;

public:
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void KnockBack(float dx, float dy, float dist);

	virtual void Behaviour(float fElapsedTime, float playerX, float playerY);
	virtual void PerformAttack();

protected:
	float fStateTick;
	float fKnockBackTimer = 0.0f;
	float fKnockBackDX = 0.0f;
	float fKnockBackDY = 0.0f;
};

