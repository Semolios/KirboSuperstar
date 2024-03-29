#ifndef DEF_BRONTO_BURT
#define DEF_BRONTO_BURT

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureBrontoBurt : public cDynamicCreature
{
public:
	cDynamicCreatureBrontoBurt(cLevel* l, float oscillationAmplitude);
	virtual ~cDynamicCreatureBrontoBurt();

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);

private:
	const float cfOscillationSpeed = 5.0f; // Bronto burt oscillation speed

	float fTheta;
	float fOscillationAmplitude;
};

#endif // !DEF_BRONTO_BURT
