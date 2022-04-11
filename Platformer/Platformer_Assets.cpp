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
	// TODO: penser à décommenter à la fin
	//load("frosty", "assets/gfx/frosty.png");
	//load("kracko", "assets/gfx/kracko.png");
	//load("SSTierMetaKnight", "assets/gfx/SSTierMetaKnight.png");
	//load("kingDDD", "assets/gfx/kingDDD.png");
}

void cAssets::LoadFont()
{
	auto load = [&](std::string sName, std::string sFileName)
	{
		olc::Sprite* s = new olc::Sprite(sFileName);
		m_mapSprites[sName] = s;
	};

	// Font
	load("A", "assets/gfx/charA.png");
	load("B", "assets/gfx/charB.png");
	load("C", "assets/gfx/charC.png");
	load("D", "assets/gfx/charD.png");
	load("E", "assets/gfx/charE.png");
	load("F", "assets/gfx/charF.png");
	load("G", "assets/gfx/charG.png");
	load("H", "assets/gfx/charH.png");
	load("I", "assets/gfx/charI.png");
	load("J", "assets/gfx/charJ.png");
	load("K", "assets/gfx/charK.png");
	load("L", "assets/gfx/charL.png");
	load("M", "assets/gfx/charM.png");
	load("N", "assets/gfx/charN.png");
	load("O", "assets/gfx/charO.png");
	load("P", "assets/gfx/charP.png");
	load("Q", "assets/gfx/charQ.png");
	load("R", "assets/gfx/charR.png");
	load("S", "assets/gfx/charS.png");
	load("T", "assets/gfx/charT.png");
	load("U", "assets/gfx/charU.png");
	load("V", "assets/gfx/charV.png");
	load("W", "assets/gfx/charW.png");
	load("X", "assets/gfx/charX.png");
	load("Y", "assets/gfx/charY.png");
	load("Z", "assets/gfx/charZ.png");
	load("0", "assets/gfx/char0.png");
	load("1", "assets/gfx/char1.png");
	load("2", "assets/gfx/char2.png");
	load("3", "assets/gfx/char3.png");
	load("4", "assets/gfx/char4.png");
	load("5", "assets/gfx/char5.png");
	load("6", "assets/gfx/char6.png");
	load("7", "assets/gfx/char7.png");
	load("8", "assets/gfx/char8.png");
	load("9", "assets/gfx/char9.png");
	load("_", "assets/gfx/char_.png");
	load("(", "assets/gfx/char(.png");
	load(")", "assets/gfx/char).png");
	load(".", "assets/gfx/char..png");
	load("%", "assets/gfx/char%.png");
}
