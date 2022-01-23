#include "Platformer_DynamicBloc.h"

cDynamicBloc::cDynamicBloc(float ox, float oy, std::vector<olc::Sprite*> map)
	: cDynamicMovingPlatform(ox, oy, map, L"0")
{
	bSolidBot = true;
	bSolidTop = true;
	bSolidLeft = true;
	bSolidRight = true;
}

cDynamicBloc::~cDynamicBloc()
{
}

void cDynamicBloc::Behaviour(float fElapsedTime, float playerX, float playerY)
{
}
