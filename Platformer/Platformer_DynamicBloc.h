#ifndef DEF_BLOC
#define DEF_BLOC

#pragma once

#include "Platformer_DynamicMovingPlatform.h"

class cDynamicBloc : public cDynamicMovingPlatform
{
public:
	cDynamicBloc(float ox, float oy, std::vector<olc::Decal*> map);
	virtual ~cDynamicBloc();

	virtual void Behaviour(float fElapsedTime, float playerX, float playerY);
};

#endif // !DEF_BLOC
