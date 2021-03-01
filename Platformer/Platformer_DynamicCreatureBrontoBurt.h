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

private:
	const float cfOscillationSpeed = 5.0f; // Bronto burt oscillation speed

	float fTheta;
	float fOscillationAmplitude;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};

#endif // !DEF_BRONTO_BURT