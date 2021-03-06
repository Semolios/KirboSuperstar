#ifndef DEF_WADDLE_DEE
#define DEF_WADDLE_DEE

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class OneLoneCoder_Platformer;

class cDynamicCreatureWaddleDee : public cDynamicCreature
{
public:
	cDynamicCreatureWaddleDee(cLevel* l);
	virtual ~cDynamicCreatureWaddleDee();

public:
	static OneLoneCoder_Platformer* engine;

public:
	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
};

#endif // !DEF_WADDLE_DEE