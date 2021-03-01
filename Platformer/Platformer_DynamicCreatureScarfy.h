#ifndef DEF_SCARFY
#define DEF_SCARFY

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureScarfy : public cDynamicCreature
{
public:
	cDynamicCreatureScarfy(cLevel* l);
	virtual ~cDynamicCreatureScarfy();

private:
	const float cfTriggerDistance = 6.0f;
	const float cfSpeed = 2.0f;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};

#endif // !DEF_SCARFY