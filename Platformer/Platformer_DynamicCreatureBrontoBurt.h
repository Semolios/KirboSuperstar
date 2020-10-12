#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureBrontoBurt : public cDynamicCreature
{
public:
	cDynamicCreatureBrontoBurt(cLevel* l, float oscillationAmplitude);

public:
	const float cfOscillationSpeed = 5.0f; // Bronto burt oscillation speed

	float fTheta;
	float fOscillationAmplitude;

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};

