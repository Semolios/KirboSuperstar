#ifndef DEF_HARMFULBLOC
#define DEF_HARMFULBLOC

#pragma once

#include "Platformer_DynamicBloc.h"

class cDynamicHarmfulBloc : public cDynamicBloc
{
public:
	cDynamicHarmfulBloc(float ox, float oy, std::vector<olc::Sprite*> map, float dmg);
	virtual ~cDynamicHarmfulBloc();

	void Behaviour(float fElapsedTime, float playerX, float playerY);
};

#endif // !DEF_HARMFULBLOC
