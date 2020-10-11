#include "Platformer_Assets.h"

cAssets::cAssets()
{
}

cAssets::~cAssets()
{
}

void cAssets::LoadSprites()
{
	auto load = [&](std::string sName, std::string sFileName)
	{
		olc::Sprite* s = new olc::Sprite(sFileName);
		m_mapSprites[sName] = s;
	};

	load("waddleDee", "assets/gfx/waddleDee.png");
	load("brontoBurt", "assets/gfx/brontoBurt.png");
	load("scarfy", "assets/gfx/scarfy.png");
}

void cAssets::LoadItems()
{
	/*
	* TODO quand la classe item sera cr��e
	auto load = [&](cItem* i)
	{
		m_mapItems[i->sName] = i;
	};

	load(new cItem_Health());
	load(new cItem_HealthBoost());
	load(new cWeapon_Sword());
	*/
}
