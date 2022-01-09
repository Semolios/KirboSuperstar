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

cDynamicCreatureWaddleDee::~cDynamicCreatureWaddleDee()
{
	delete hitbox;
	delete sSprite;
	delete level;
}

void cDynamicCreatureWaddleDee::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	if (vy == 0)
	{
		if (vx < 0)
		{
			// Check left wall or hole
			if (engine->IsSolidTile(level->GetTile(px, py)) || (!engine->IsSolidTile(level->GetTile(px, py)) && !engine->IsSolidTile(level->GetTile(px, py + 1)) && !engine->IsSemiSolidTile(level->GetTile(px, py + 1))))
			{
				// Don't check the platforms if the waddle dee is on ground, or all the waddle dees will check all platforms
				for (auto& ptfm : engine->GetPlatforms())
				{
					if (!ptfm->TopCollisionOneCorner(px, py + 1.0f))
					{
						TurnAround();
					}
				}
			}
		}
		else if (vx > 0)
		{
			// Check right wall or hole
			if (engine->IsSolidTile(level->GetTile(px + 1, py)) || (!engine->IsSolidTile(level->GetTile(px + 1, py)) && !engine->IsSolidTile(level->GetTile(px + 1, py + 1)) && !engine->IsSemiSolidTile(level->GetTile(px + 1, py + 1))))
			{
				// Don't check the platforms if the waddle dee is on ground, or all the waddle dees will check all platforms
				for (auto& ptfm : engine->GetPlatforms())
				{
					if (!ptfm->TopCollisionOneCorner(px + 1.0f, py + 1.0f))
					{
						TurnAround();
					}
				}
			}
		}
		else
		{
			vx = 1.0f;
		}
	}
}
