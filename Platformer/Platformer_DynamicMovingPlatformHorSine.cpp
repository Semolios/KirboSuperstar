#include "Platformer_DynamicMovingPlatformHorSine.h"

cDynamicMovingPlatformHorSine::cDynamicMovingPlatformHorSine(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring iced, float A, float F, std::wstring trigger)
	: cDynamicMovingPlatform(ox, oy, map, iced)
{
	fAmplitude = A;
	fFrequency = F;
	bTriggerable = (trigger == L"1");
}

cDynamicMovingPlatformHorSine::~cDynamicMovingPlatformHorSine()
{
}

void cDynamicMovingPlatformHorSine::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	if (!bTriggerable || bTriggered)
	{
		fTheta += fElapsedTime;
		vx = fAmplitude * sinf(fFrequency * fTheta);
	}
}
