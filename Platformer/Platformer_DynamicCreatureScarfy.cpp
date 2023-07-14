#include "Platformer_DynamicCreatureScarfy.h"

cDynamicCreatureScarfy::cDynamicCreatureScarfy(cLevel* l) : cDynamicCreature("scarfy", cAssets::get().GetSprite("scarfy"), 16)
{
	fDynWidth = 40.0f;
	fDynHeight = 40.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = -12.0f;
	fSpriteOffsetY = -12.0f;
	bFriendly = false;
	nHealth = 1;
	nHealthMax = 1;
	level = l;
	vx = 0;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = false;
	nDamage = 2;
}

cDynamicCreatureScarfy::~cDynamicCreatureScarfy()
{
	delete hitbox;
	delete dynSprite;
	delete level;
}

void cDynamicCreatureScarfy::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// Check if player is nearby
	float fTargetX = playerX - px;
	float fTargetY = playerY - py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	if (fDistance < cfTriggerDistance)
	{
		vx = (fTargetX / fDistance) * cfSpeed;
		vy = (fTargetY / fDistance) * cfSpeed;
	}
	else
	{
		vx = 0;
		vy = 0;
	}
}
