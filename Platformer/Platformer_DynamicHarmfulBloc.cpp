#include "Platformer_DynamicHarmfulBloc.h"

cDynamicHarmfulBloc::cDynamicHarmfulBloc(float ox, float oy, std::vector<olc::Sprite*> map, float dmg, std::wstring bTangible)
	: cDynamicBloc(ox, oy, map)
{
	bFriendly = false;
	nDamage = dmg;
	bTangible = (bTangible == L"1");
}

cDynamicHarmfulBloc::~cDynamicHarmfulBloc()
{
}

void cDynamicHarmfulBloc::Behaviour(float fElapsedTime, float playerX, float playerY)
{
}

bool cDynamicHarmfulBloc::IsHarmfulblocTangible()
{
	return bTangible;
}
