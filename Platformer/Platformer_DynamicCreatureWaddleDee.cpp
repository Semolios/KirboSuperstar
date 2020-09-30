#include "Platformer_DynamicCreatureWaddleDee.h"

cDynamicCreatureWaddleDee::cDynamicCreatureWaddleDee() : cDynamicCreature("waddleDee", cAssets::get().GetSprite("waddleDee"), 4)
{
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
}

void cDynamicCreatureWaddleDee::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	// TODO faire bouger le waddle dee de gauche à droite sur sa plateforme
}
