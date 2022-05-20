#include "Platformer_DynamicCreatureBrontoBurt.h"
#include "Platformer_Level.h"

cDynamicCreatureBrontoBurt::cDynamicCreatureBrontoBurt(cLevel* l, float oscillationAmplitude) : cDynamicCreature("brontoBurt", cAssets::get().GetSprite("brontoBurt"), 4)
{
	fDynWidth = 64.0f;
	fDynHeight = 64.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = 0.0f;
	fSpriteOffsetY = 0.0f;
	bFriendly = false;
	nHealth = 6;
	nHealthMax = 6;
	level = l;
	fInitSpeed = -1;
	fTheta = 0.0f;
	bSolidVsMap = false;
	bSolidVsDynInitValue = false;
	bAffectedByGravity = false;
	fOscillationAmplitude = oscillationAmplitude;
	nDamage = 2;
}

cDynamicCreatureBrontoBurt::~cDynamicCreatureBrontoBurt()
{
	delete hitbox;
	delete sSprite;
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
