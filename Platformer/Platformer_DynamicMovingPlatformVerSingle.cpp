#include "Platformer_DynamicMovingPlatformVerSingle.h"

cDynamicMovingPlatformVerSingle::cDynamicMovingPlatformVerSingle(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring iced, float ty, float vy, std::wstring trigger)
	: cDynamicMovingPlatform(ox, oy, map, iced)
{
	fTargetY = ty;
	fPtfmVelY = vy;
	bTriggerable = (trigger == L"1");
}

cDynamicMovingPlatformVerSingle::~cDynamicMovingPlatformVerSingle()
{
}

void cDynamicMovingPlatformVerSingle::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	if (!bTriggerable || bTriggered)
	{
		if (fabs(py - fTargetY) >= cfTargetDelta)
		{
			vy = (py <= fTargetY ? 1.0f : -1.0f) * fPtfmVelY;
		}
		else
		{
			vy = 0.0f;
			bTriggered = false;
		}
	}
}
