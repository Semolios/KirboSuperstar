#ifndef DEF_MOVING_PLATFORM_VERSINE
#define DEF_MOVING_PLATFORM_VERSINE

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicMovingPlatformVerSine : public cDynamicMovingPlatform
{
public:
	cDynamicMovingPlatformVerSine(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring iced, float A, float F, std::wstring trigger);
	virtual ~cDynamicMovingPlatformVerSine();

	void Behaviour(float fElapsedTime, float playerX, float playerY);

protected:
	float fTheta = 0.0f;
	float fAmplitude;
	float fFrequency;
};

#endif // !DEF_MOVING_PLATFORM_VERSINE
