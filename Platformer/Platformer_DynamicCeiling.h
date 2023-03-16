#ifndef DEF_CEILING
#define DEF_CEILING

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicCeiling : public cDynamicMovingPlatform
{
public:
	cDynamicCeiling(float ox, float oy, std::vector<olc::Decal*> map, std::wstring topSolid, std::wstring linkToPreviousPlatform);
	virtual ~cDynamicCeiling();

	void Behaviour(float fElapsedTime, float playerX, float playerY);
};

#endif // !DEF_CEILING
