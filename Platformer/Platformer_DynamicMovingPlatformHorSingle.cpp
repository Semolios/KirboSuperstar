#include "Platformer_DynamicMovingPlatformHorSingle.h"

cDynamicMovingPlatformHorSingle::cDynamicMovingPlatformHorSingle(float ox, float oy, std::vector<olc::Sprite*> map, float tx, float vx, std::wstring trigger)
	: cDynamicMovingPlatform(ox, oy, map)
{
	fTargetX = tx;
	fPtfmVelX = vx;
	bTriggerable = (trigger == L"1");
}

cDynamicMovingPlatformHorSingle::~cDynamicMovingPlatformHorSingle()
{
}

void cDynamicMovingPlatformHorSingle::UpdateTrajectory(float fElapsedTime)
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
