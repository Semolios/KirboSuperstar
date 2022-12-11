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

	static OneLoneCoder_Platformer* engine;

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	bool RightObstacle();
	bool LeftObstacle();
};

#endif // !DEF_WADDLE_DEE
