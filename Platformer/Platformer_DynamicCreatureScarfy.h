#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureScarfy : public cDynamicCreature
{
public:
	cDynamicCreatureScarfy(cLevel* l);

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};

