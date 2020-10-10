#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

class cDynamicCreatureWaddleDee : public cDynamicCreature
{
public:
	cDynamicCreatureWaddleDee(cLevel* l);

	void Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx);
	bool IsSolidTile(wchar_t tile);
	bool IsSemiSolidTile(wchar_t tile);
};

