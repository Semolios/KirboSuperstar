#include "Platformer_DynamicCreatureBrontoBurt.h"
#include "Platformer_Level.h"

cDynamicCreatureBrontoBurt::cDynamicCreatureBrontoBurt(cLevel* l, float oscillationAmplitude) : cDynamicCreature("brontoBurt", cAssets::get().GetSprite("brontoBurt"), 4)
{
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
	level = l;
	fInitSpeed = -1;
	fTheta = 0.0f;
	bSolidVsMap = false;
	bAffectedByGravity = false;
	fOscillationAmplitude = oscillationAmplitude;
}

void cDynamicCreatureBrontoBurt::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// Bronto Burt moves when kirby is near (when bronto burt appears on the screen)
	if (abs((px - playerX) * nTileSize) < gfx->ScreenWidth() / 2 + nTileSize && abs((py - playerY) * nTileSize) < gfx->ScreenHeight() / 2 + 2 * fOscillationAmplitude * nTileSize)
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
