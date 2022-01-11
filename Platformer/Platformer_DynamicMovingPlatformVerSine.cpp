#include "Platformer_DynamicMovingPlatformVerSine.h"

cDynamicMovingPlatformVerSine::cDynamicMovingPlatformVerSine(float ox, float oy, std::vector<olc::Sprite*> map, float A, float F, std::wstring trigger)
	: cDynamicMovingPlatform(ox, oy, map)
{
	fAmplitude = A;
	fFrequency = F;
	bTriggerable = (trigger == L"1");
}

cDynamicMovingPlatformVerSine::~cDynamicMovingPlatformVerSine()
{
}

void cDynamicMovingPlatformVerSine::UpdateTrajectory(float fElapsedTime)
{
	if (!bTriggerable || bTriggered)
	{
		fTheta += fElapsedTime;
		vy = fAmplitude * sinf(fFrequency * fTheta);
	}
}
