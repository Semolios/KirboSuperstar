#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Hitbox.h"

class Platformer_Engine;

class cDynamic
{
public:
	cDynamic(std::string n);
	virtual ~cDynamic();

protected:
	float fDynWidth;
	float fDynHeight;
	float fSpriteW;
	float fSpriteH;
	float fSpriteOffsetX;
	float fSpriteOffsetY;

	float px, py;
	float vx, vy;
	int nDamage;
	int nCornerSpr = 0;
	bool bSolidVsMap;
	bool bSolidVsDyn;
	bool bSolidVsDynInitValue;
	bool bFriendly;
	bool bIsProjectile;
	bool bIsAttackable;
	bool bDead = false;
	bool bAffectedByGravity = false;
	std::string sName;
	cHitbox* hitbox;

public:
	// Virtual methods
	virtual void DrawSelf(float ox, float oy)
	{
	}
	virtual void Update(float fElapsedTime, float playerX, float playerY)
	{
	}
	virtual void OnInteract(cDynamic* player = nullptr)
	{
	}
	virtual void UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
	{
	}

public:
	void SetPosition(float x, float y);
	float GetPX();
	float GetPY();
	int GetDamage();
	bool IsAttackable();
	bool IsFriendly();
	cHitbox* GetHitbox();
};

