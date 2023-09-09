#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureWaddleDee::engine = nullptr;

cDynamicCreatureWaddleDee::cDynamicCreatureWaddleDee(cLevel* l) : cDynamicCreature("waddleDee", cAssets::get().GetSprite("waddleDee"), 4)
{
	fDynWidth = 48.0f;
	fDynHeight = 32.0f;
	fSpriteW = 64.0f;
	fSpriteH = 64.0f;
	fSpriteOffsetX = -8.0f;
	fSpriteOffsetY = -32.0f;
	bFriendly = false;
	nHealth = 1;
	nHealthMax = 1;
	level = l;
	vx = -1;
	fInitSpeed = -1;
	bSolidVsMap = true;
	bAffectedByGravity = true;
	nDamage = 1;
}

cDynamicCreatureWaddleDee::~cDynamicCreatureWaddleDee()
{
	delete hitbox;
	delete dynSprite;
	delete level;
}

void cDynamicCreatureWaddleDee::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	bool mustTurnBack = true;

	if (vy == 0)
	{
		if (vx < 0)
		{
			if (LeftObstacle())
			{
				// Don't check the platforms if the waddle dee is on ground, or all the waddle dees will check all platforms
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->TopCollisionOneCorner(px, py + fNormalizedH) && !engine->IsSolidTile(level->GetTile(px, py)))
					{
						mustTurnBack = false;
					}
				}
				if (mustTurnBack)
					TurnBack();
			}
			else
			{
				mustTurnBack = false;
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->RightCollision(py, py + fNormalizedH, px))
					{
						mustTurnBack = true;
					}
				}
				if (mustTurnBack)
					TurnBack();
			}
		}
		else if (vx > 0)
		{
			if (RightObstacle())
			{
				// Don't check the platforms if the waddle dee is on ground, or all the waddle dees will check all platforms
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->TopCollisionOneCorner(px + fNormalizedW, py + fNormalizedH) && !ptfm->LeftCollision(py, py + fNormalizedH, px + fNormalizedW) && !engine->IsSolidTile(level->GetTile(px + fNormalizedW, py)))
					{
						mustTurnBack = false;
					}
				}
				if (mustTurnBack)
					TurnBack();
			}
			else
			{
				mustTurnBack = false;
				for (auto& ptfm : engine->GetClosePlatforms(px, py))
				{
					if (ptfm->LeftCollision(py, py + fNormalizedH, px + fNormalizedW))
					{
						mustTurnBack = true;
					}
				}
				if (mustTurnBack)
					TurnBack();
			}
		}
		else
		{
			vx = 1.0f;
		}
	}
}

bool cDynamicCreatureWaddleDee::RightObstacle()
{
	return engine->IsSolidTile(level->GetTile(px + fNormalizedW, py)) || // Wall
		   (!engine->IsSolidTile(level->GetTile(px + fNormalizedW, py)) &&
			!engine->IsSolidTile(level->GetTile(px + fNormalizedW, py + fNormalizedH)) &&
			!engine->IsSemiSolidTile(level->GetTile(px + fNormalizedW, py + fNormalizedH))); // Hole
}

bool cDynamicCreatureWaddleDee::LeftObstacle()
{
	return engine->IsSolidTile(level->GetTile(px, py)) || // Wall
		   (!engine->IsSolidTile(level->GetTile(px, py)) &&
			!engine->IsSolidTile(level->GetTile(px, py + fNormalizedH)) &&
			!engine->IsSemiSolidTile(level->GetTile(px, py + fNormalizedH))); // Hole
}
