#ifndef DEF_MOVING_PLATFORM_HORSINE
#define DEF_MOVING_PLATFORM_HORSINE

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicMovingPlatformHorSine : public cDynamicMovingPlatform
{
public:
	cDynamicMovingPlatformHorSine(float ox, float oy, std::vector<olc::Sprite*> map, float A, float F, std::wstring trigger);
	virtual ~cDynamicMovingPlatformHorSine();

	void UpdateTrajectory(float fElapsedTime);

protected:
	float fTheta = 0.0f;
	float fAmplitude;
	float fFrequency;
	bool bTriggerable;
};

#endif // !DEF_MOVING_PLATFORM_HORSINE
