#ifndef DEF_WALL
#define DEF_WALL

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicWall : public cDynamicMovingPlatform
{
public:
	cDynamicWall(float ox, float oy, std::vector<olc::Decal*> map, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY);
	virtual ~cDynamicWall();

	void Behaviour(float fElapsedTime, float playerX, float playerY);
	void DrawSwitch(float cameraX, float cameraY);
	bool IsTriggered();
	void SetTriggered(bool triggered);

	static OneLoneCoder_Platformer* engine;

protected:
	const float cfTriggerDist = 0.5f;
	const float cfOpeningSpeed = -2.0f;
	const int	cnSwitchOffsetY = 10;
	float fOriginY;
	float fTriggerX;
	float fTriggerY;
	bool bSwitchSound = true;
	bool bGateOpenningSound = true;
	bool bGateOpennedSound = true;
};

#endif //DEF_WALL
