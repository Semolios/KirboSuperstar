#include "Platformer_DynamicHarmfulBloc.h"

cDynamicHarmfulBloc::cDynamicHarmfulBloc(float ox, float oy, std::vector<olc::Sprite*> map, float dmg, std::wstring tangible)
	: cDynamicBloc(ox, oy, map)
{
	bFriendly = false;
	nDamage = dmg;
	bTangible = (tangible == L"1");
}

cDynamicHarmfulBloc::~cDynamicHarmfulBloc()
{
}

void cDynamicHarmfulBloc::Behaviour(float fElapsedTime, float playerX, float playerY)
{
}
