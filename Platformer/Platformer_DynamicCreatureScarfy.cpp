#include "Platformer_DynamicCreatureScarfy.h"

cDynamicCreatureScarfy::cDynamicCreatureScarfy(cLevel* l) : cDynamicCreature("scarfy", cAssets::get().GetSprite("scarfy"), 16)
{
	fDynWidth = 64.0f;
	fDynHeight = 64.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = 0.0f;
	fSpriteOffsetY = 0.0f;
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
	level = l;
	vx = 0;	// initialize the speed so the scarfy waits for kirby
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = false;
	nDamage = 4;
}

void cDynamicCreatureScarfy::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// Check if player is nearby
	float fTargetX = playerX - px;
	float fTargetY = playerY - py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	if (fDistance < 6.0f)
	{
		vx = (fTargetX / fDistance) * 2.0f;
		vy = (fTargetY / fDistance) * 2.0f;
	}
	else
	{
		vx = 0;
		vy = 0;
	}
}
