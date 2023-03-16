#include "Platformer_DynamicMovingPlatformVerSine.h"

cDynamicMovingPlatformVerSine::cDynamicMovingPlatformVerSine(float ox, float oy, std::vector<olc::Decal*> map, std::wstring iced, float A, float F, std::wstring trigger)
	: cDynamicMovingPlatform(ox, oy, map, iced)
{
	fAmplitude = A;
	fFrequency = F;
	bTriggerable = (trigger == L"1");
}

cDynamicMovingPlatformVerSine::~cDynamicMovingPlatformVerSine()
{
}

void cDynamicMovingPlatformVerSine::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	if (!bTriggerable || bTriggered)
	{
		fTheta += fElapsedTime;
		vy = fAmplitude * sinf(fFrequency * fTheta);
	}
}
