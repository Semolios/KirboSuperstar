#include "Platformer_DynamicWall.h"

cDynamicWall::cDynamicWall(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY)
	: cDynamicMovingPlatform(ox, oy, map)
{
	bSolidTop = false;
	bSolidLeft = (leftSolid == L"1");
	bSolidRight = (rightSolid == L"1");
	bTriggerable = (trigger == L"1");

	fTriggerX = trgX;
	fTriggerY = trgY;

	originY = oy;
}

cDynamicWall::~cDynamicWall()
{
}

void cDynamicWall::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	if (bTriggerable)
	{
		// If Kirbo is close to the trigger Point, open the door
		float fTargetX = playerX - fTriggerX;
		float fTargetY = playerY - fTriggerY;
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

		if (fDistance <= cfTriggerDist)
		{
			bTriggered = true;
		}
	}

	if (bTriggered)
	{
		if (fabs(py - originY) <= (mapStates[nCurrentFrame]->height / 64.0f))
		{
			vy = cfOpeningSpeed;
		}
		else
		{
			vy = 0.0f;
		}
	}
}
