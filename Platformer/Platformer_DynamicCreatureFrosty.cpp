#include "Platformer_DynamicCreatureFrosty.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureFrosty::engine = nullptr;

cDynamicCreatureFrosty::cDynamicCreatureFrosty(cLevel* l) : cDynamicCreature("frosty", cAssets::get().GetSprite("frosty"), 8)
{
	fDynWidth = 128.0f;
	fDynHeight = 128.0f;
	fSpriteW = 256.0f;
	fSpriteH = 192.0f;
	fSpriteOffsetX = -64.0f;
	fSpriteOffsetY = -64.0f;
	bFriendly = false;
	nHealth = 100;
	nHealthMax = 100;
	level = l;
	bSolidVsMap = true;
	bAffectedByGravity = true;
	nDamage = 3;
	bCanBehaveWhileAttacked = true;

	srand(time(NULL));
}

void cDynamicCreatureFrosty::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
}
