#include "Platformer_DynamicCreatureRocky.h"
#include "Platformer_Level.h"

cDynamicCreatureRocky::cDynamicCreatureRocky(cLevel* l) : cDynamicCreature("rocky", cAssets::get().GetSprite("rocky"), 16)
{
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
	level = l;
	vx = 0;	// initialize the speed so the waddle dee goes left first
	fInitSpeed = 0;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = false;
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

			if (abs(playerX - px) < 0.5f && playerY > py)
				nAINextState = AI_FALLING;
		}
		break;
		case AI_FALLING:
		{
			bAffectedByGravity = true;
			nGraphicState = WALKING;

			if (IsSolidTile(GetTile(px, py + 1)))
				nAINextState = AI_LANDING;
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
			vy -= 0.1f;

			// Clamping going back up speed
			if (vy < -2) vy = -2;

			if (IsSolidTile(GetTile(px, py - 0.01f)) || py == 1)
			{
				vy = 0;
				nAINextState = AI_WAITING;
			}
		}
		break;
	}

	nAIState = nAINextState;
}

bool cDynamicCreatureRocky::IsSolidTile(wchar_t tile)
{
	// List Here all the tiles that are not solid (if there are less non solid tile than solid ones)
	return tile != '.' && tile != 'o' && tile != 'w' && tile != '?';
}

bool cDynamicCreatureRocky::IsSemiSolidTile(wchar_t tile)
{
	// List Here all the tiles that are semi solid
	return tile == '?';
}
