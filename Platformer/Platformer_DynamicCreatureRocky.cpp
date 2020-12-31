#include "Platformer_DynamicCreatureRocky.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureRocky::engine = nullptr;

cDynamicCreatureRocky::cDynamicCreatureRocky(cLevel* l) : cDynamicCreature("rocky", cAssets::get().GetSprite("rocky"), 16)
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
	vx = 0;	// initialize the speed so the waddle dee goes left first
	fInitSpeed = 0;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = false;
	nDamage = 4;
}

void cDynamicCreatureRocky::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// lambda fonction to check if there is a hole or a wall to change direction
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < level->GetWidth() && y >= 0 && y < level->GetHeight())
			return level->GetLevel()[y * level->GetWidth() + x];
		else
			return L' ';
	};

	vx = 0;

	switch (nAIState)
	{
		case AI_WAITING:
		{
			bAffectedByGravity = false;
			vy = 0;

			if (abs(playerX - px) < 0.5f && playerY > py)
				nAINextState = AI_FALLING;
		}
		break;
		case AI_FALLING:
		{
			bAffectedByGravity = true;
			nGraphicState = WALKING;

			if (engine->IsSolidTile(GetTile(px + cfRockyLowerBoundary, py + 1)) ||
				engine->IsSolidTile(GetTile(px + cfRockyUpperBoundary, py + 1)) ||
				engine->IsSemiSolidTile(GetTile(px + cfRockyLowerBoundary, py + 1)) ||
				engine->IsSemiSolidTile(GetTile(px + cfRockyUpperBoundary, py + 1)))
			{
				nAINextState = AI_LANDING;
			}
		}
		break;
		case AI_LANDING:
		{
			fTimer += fElapsedTime;

			if (fTimer >= fLandingTime)
			{
				fTimer = 0.0f;
				nAINextState = AI_GOING_BACK_UP;
			}
		}
		break;
		case AI_GOING_BACK_UP:
		{
			bAffectedByGravity = false;
			vy = cfGoingBackUpSpeed;

			if (engine->IsSolidTile(GetTile(px + cfRockyLowerBoundary, py - 0.01f)) ||
				engine->IsSolidTile(GetTile(px + cfRockyUpperBoundary, py - 0.01f)) ||
				py < 1)
			{
				vy = 0;
				nAINextState = AI_WAITING;
			}
		}
		break;
	}

	nAIState = nAINextState;
}
