#include "Platformer_DynamicCreatureBrontoBurt.h"
#include "Platformer_Level.h"

cDynamicCreatureBrontoBurt::cDynamicCreatureBrontoBurt(cLevel* l, float oscillationAmplitude) : cDynamicCreature("brontoBurt", cAssets::get().GetSprite("brontoBurt"), 4)
{
	fDynWidth = 32.0f;
	fDynHeight = 32.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = -16.0f;
	fSpriteOffsetY = -16.0f;
	bFriendly = false;
	nHealth = 3;
	nHealthMax = 3;
	level = l;
	fInitSpeed = -1;
	fTheta = 0.0f;
	bSolidVsMap = false;
	bAffectedByGravity = false;
	fOscillationAmplitude = oscillationAmplitude;
	nDamage = 1;
}

cDynamicCreatureBrontoBurt::~cDynamicCreatureBrontoBurt()
{
	delete hitbox;
	delete dynSprite;
	delete level;
}

void cDynamicCreatureBrontoBurt::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// Bronto Burt moves when kirbo is near (when bronto burt appears on the screen)
	if (abs((px - playerX) * fDynWidth) < gfx->ScreenWidth() / 2 + fDynWidth && abs((py - playerY) * fDynHeight) < gfx->ScreenHeight() / 2 + 2 * fOscillationAmplitude * fDynHeight)
	{
		fTheta += cfOscillationSpeed * fElapsedTime;
		vx = fInitSpeed * nFaceDir ? 1.0f : -1.0f;
		vy = sinf(fTheta) * fOscillationAmplitude;
	}
	else
	{
		vx = 0;
		vy = 0;
	}
}
