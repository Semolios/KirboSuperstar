#ifndef DEF_MOVING_PLATFORM_HORSINGLE
#define DEF_MOVING_PLATFORM_HORSINGLE

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicMovingPlatformHorSingle : public cDynamicMovingPlatform
{
public:
	cDynamicMovingPlatformHorSingle(float ox, float oy, std::vector<olc::Decal*> map, std::wstring iced, float tx, float vx, std::wstring trigger);
	virtual ~cDynamicMovingPlatformHorSingle();

	void Behaviour(float fElapsedTime, float playerX, float playerY);

protected:
	const float cfTargetDelta = 0.1f;
	float fTargetX;
	float fPtfmVelX;
};

#endif // !DEF_MOVING_PLATFORM_HORSINGLE
