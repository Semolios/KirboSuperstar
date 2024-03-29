#include "Platformer_DynamicMovingPlatformHorSingle.h"

cDynamicMovingPlatformHorSingle::cDynamicMovingPlatformHorSingle(float ox, float oy, std::vector<olc::Decal*> map, std::wstring iced, float tx, float vx, std::wstring trigger)
	: cDynamicMovingPlatform(ox, oy, map, iced)
{
	fTargetX = tx;
	fPtfmVelX = vx;
	bTriggerable = (trigger == L"1");
}

cDynamicMovingPlatformHorSingle::~cDynamicMovingPlatformHorSingle()
{
}

void cDynamicMovingPlatformHorSingle::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	if (!bTriggerable || bTriggered)
	{
		if (fabs(px - fTargetX) >= cfTargetDelta)
		{
			vx = (px <= fTargetX ? 1.0f : -1.0f) * fPtfmVelX;
		}
		else
		{
			vx = 0.0f;
		}
	}
}
