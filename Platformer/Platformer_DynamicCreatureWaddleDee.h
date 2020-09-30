#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureWaddleDee : public cDynamicCreature
{
public:
	cDynamicCreatureWaddleDee();

	void Behaviour(float fElapsedTime, float playerX, float playerY);
};

