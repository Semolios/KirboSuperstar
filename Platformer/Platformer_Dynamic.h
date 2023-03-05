#ifndef DEF_DYNAMIC
#define DEF_DYNAMIC

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Hitbox.h"

class cDynamic
{
public:
	cDynamic(std::string n);
	virtual ~cDynamic();

	// Virtual methods
	virtual void DrawSelf(float ox, float oy)
	{
	}
	virtual void Update(float fElapsedTime, float playerX, float playerY)
	{
	}
	virtual void UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
	{
	}

	void SetPosition(float x, float y);
	float GetPX();
	float GetPY();
	float GetVX();
	float GetVY();
	int GetDamage();
	bool IsAttackable();
	bool IsFriendly();
	cHitbox* GetHitbox();

protected:
	olc::Sprite* dynSprite;
	olc::Decal*  dynDecal;

	float fDynWidth;
	float fDynHeight;
	float fSpriteW;
	float fSpriteH;
	float fSpriteOffsetX;
	float fSpriteOffsetY;
	float fNormalizedW;
	float fNormalizedH;

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
};

#endif // !DEF_DYNAMIC
