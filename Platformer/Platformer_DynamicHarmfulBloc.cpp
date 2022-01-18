#include "Platformer_DynamicHarmfulBloc.h"

cDynamicHarmfulBloc::cDynamicHarmfulBloc(float ox, float oy, std::vector<olc::Sprite*> map, float dmg)
	: cDynamicBloc(ox, oy, map)
{
	bFriendly = false;
	nDamage = dmg;
}

cDynamicHarmfulBloc::~cDynamicHarmfulBloc()
{
}

void cDynamicHarmfulBloc::Behaviour(float fElapsedTime, float playerX, float playerY)
{
}
