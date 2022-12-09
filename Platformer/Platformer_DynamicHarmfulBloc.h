#ifndef DEF_HARMFULBLOC
#define DEF_HARMFULBLOC

#pragma once

#include "Platformer_DynamicBloc.h"

class cDynamicHarmfulBloc : public cDynamicBloc
{
private:
	bool bTangible;

public:
	cDynamicHarmfulBloc(float ox, float oy, std::vector<olc::Sprite*> map, float dmg, std::wstring bTangible);
	virtual ~cDynamicHarmfulBloc();

	void Behaviour(float fElapsedTime, float playerX, float playerY);
	bool IsHarmfulblocTangible();
};

#endif // !DEF_HARMFULBLOC
