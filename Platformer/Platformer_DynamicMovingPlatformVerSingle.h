#ifndef DEF_MOVING_PLATFORM_VERSINGLE
#define DEF_MOVING_PLATFORM_VERSINGLE

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicMovingPlatformVerSingle : public cDynamicMovingPlatform
{
public:
	cDynamicMovingPlatformVerSingle(float ox, float oy, std::vector<olc::Sprite*> map, float ty, float vy, std::wstring trigger);
	virtual ~cDynamicMovingPlatformVerSingle();

	void Behaviour(float fElapsedTime, float playerX, float playerY);

protected:
	const float cfTargetDelta = 0.1f;
	float fTargetY;
	float fPtfmVelY;
};

#endif // !DEF_MOVING_PLATFORM_VERSINGLE
