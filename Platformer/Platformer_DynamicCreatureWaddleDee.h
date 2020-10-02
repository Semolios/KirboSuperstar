#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureWaddleDee : public cDynamicCreature
{
public:
	cDynamicCreatureWaddleDee(cLevel* l);

	void Behaviour(float fElapsedTime, float playerX, float playerY);
	bool IsSolidTile(wchar_t tile);
};

