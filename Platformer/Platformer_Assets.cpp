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

	// Ennemies
	load("bomber", "assets/gfx/bomber.png");
	load("brontoBurt", "assets/gfx/brontoBurt.png");
	load("waddleDee", "assets/gfx/waddleDee.png");
	load("rocky", "assets/gfx/rocky.png");
	load("scarfy", "assets/gfx/scarfy.png");
	load("bladeKnight", "assets/gfx/bladeKnight.png");

	// Bosses
	load("whispyWood", "assets/gfx/whispyWood.png");
	load("mrShineMrBright", "assets/gfx/mrShineMrBright.png");
}

void cAssets::LoadItems()
{
	/*
	* TODO quand la classe item sera créée
	auto load = [&](cItem* i)
	{
		m_mapItems[i->sName] = i;
	};

	load(new cItem_Health());
	load(new cItem_HealthBoost());
	load(new cWeapon_Sword());
	*/
}
