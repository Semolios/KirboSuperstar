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
	load("frosty", "assets/gfx/frosty.png");
	load("kracko", "assets/gfx/kracko.png");
	load("SSTierMetaKnight", "assets/gfx/SSTierMetaKnight.png");
	load("kingDDD", "assets/gfx/kingDDD.png");
}
