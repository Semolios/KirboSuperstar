#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_Level.h"

cDynamicCreatureWaddleDee::cDynamicCreatureWaddleDee(cLevel* l) : cDynamicCreature("waddleDee", cAssets::get().GetSprite("waddleDee"), 4)
{
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
	level = l;
	vx = -1;	// initialize the speed so the waddle dee goes left first
}

void cDynamicCreatureWaddleDee::Behaviour(float fElapsedTime, float playerX, float playerY)
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
			if (IsSolidTile(GetTile(px, py)) || (!IsSolidTile(GetTile(px, py)) && !IsSolidTile(GetTile(px, py + 1))))
			{
				vx = -vx;
			}
		}
		else
		{
			// Check right wall or hole
			if (IsSolidTile(GetTile(px + 1, py)) || (!IsSolidTile(GetTile(px + 1, py)) && !IsSolidTile(GetTile(px + 1, py + 1))))
			{
				vx = -vx;
			}
		}
	}
}

bool cDynamicCreatureWaddleDee::IsSolidTile(wchar_t tile)
{
	return tile != '.' && tile != 'o' && tile != 'w';
}
