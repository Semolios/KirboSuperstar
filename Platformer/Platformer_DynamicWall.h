#ifndef DEF_WALL
#define DEF_WALL

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicWall : public cDynamicMovingPlatform
{
public:
	cDynamicWall(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY);
	virtual ~cDynamicWall();

	void Behaviour(float fElapsedTime, float playerX, float playerY);

protected:
	const float cfTriggerDist = 0.5f;
	const float cfOpeningSpeed = -2.0f;
	float originY;
	float fTriggerX;
	float fTriggerY;
};

#endif //DEF_WALL
