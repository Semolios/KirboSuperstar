#pragma once

#include "Platformer_Dynamic.h"

class cDynamicProjectile : public cDynamic
{
public:
	cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, float spriteWidth, float spriteHeight, bool affectedByGravity, int damage, bool solidVSMap);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx) override;

public:
	std::vector<olc::Sprite*> mapStates;
	float fSpriteX;
	float fSpriteY;
	float fDuration;
	bool bOneHit;
	bool bAffectedByGravity;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.1f;
	float fTimeCounter = 0.0f;
};
