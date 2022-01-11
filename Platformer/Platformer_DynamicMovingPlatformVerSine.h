#ifndef DEF_MOVING_PLATFORM_VERSINE
#define DEF_MOVING_PLATFORM_VERSINE

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicMovingPlatformVerSine : public cDynamicMovingPlatform
{
public:
	cDynamicMovingPlatformVerSine(float ox, float oy, std::vector<olc::Sprite*> map, float A, float F, std::wstring trigger);
	virtual ~cDynamicMovingPlatformVerSine();

	void UpdateTrajectory(float fElapsedTime);

protected:
	float fTheta = 0.0f;
	float fAmplitude;
	float fFrequency;
	bool bTriggerable;
};

#endif // !DEF_MOVING_PLATFORM_VERSINE
