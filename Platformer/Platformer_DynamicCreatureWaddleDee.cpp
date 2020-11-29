#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureWaddleDee::engine = nullptr;

cDynamicCreatureWaddleDee::cDynamicCreatureWaddleDee(cLevel* l) : cDynamicCreature("waddleDee", cAssets::get().GetSprite("waddleDee"), 4)
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
	vx = -1;	// initialize the speed so the waddle dee goes left first
	fInitSpeed = -1;
	bSolidVsMap = true;
	bSolidVsDynInitValue = true;
	bAffectedByGravity = true;
	nDamage = 2;
}

void cDynamicCreatureWaddleDee::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// lambda fonction to check if there is a hole or a wall to change direction
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < level->GetWidth() && y >= 0 && y < level->GetHeight())
			return level->GetLevel()[y * level->GetWidth() + x];
		else
			return L' ';
	};

	if (vy == 0)
	{
		if (vx < 0)
		{
			// Check left wall or hole
			if (engine->IsSolidTile(GetTile(px, py)) || (!engine->IsSolidTile(GetTile(px, py)) && !engine->IsSolidTile(GetTile(px, py + 1)) && !engine->IsSemiSolidTile(GetTile(px, py + 1))))
			{
				TurnAround();
			}
		}
		else if (vx > 0)
		{
			// Check right wall or hole
			if (engine->IsSolidTile(GetTile(px + 1, py)) || (!engine->IsSolidTile(GetTile(px + 1, py)) && !engine->IsSolidTile(GetTile(px + 1, py + 1)) && !engine->IsSemiSolidTile(GetTile(px + 1, py + 1))))
			{
				TurnAround();
			}
		}
		else
		{
			vx = 1.0f;
		}
	}
}
