#include "Platformer_Level.h"
#include <strstream>
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cLevel::engine = nullptr;

cLevel::cLevel()
{
}

bool cLevel::LoadLevel(std::string levelName)
{
	nLevelWidth = 0;
	nLevelHeight = 0;
	sLevel = L"";

	std::wifstream file(levelName);

	if (file)
	{
		std::wstring line;

		while (std::getline(file, line))
		{
			if (line[0] == 'w')
			{
				// Width
				nLevelWidth = std::stoi(line.substr(1, line.size() - 1));
			}
			else if (line[0] == 'h')
			{
				// Height
				nLevelHeight = std::stoi(line.substr(1, line.size() - 1));
			}
			else if (line[0] == 'x')
			{
				// Init player pos X
				nInitPlayerPosX = std::stoi(line.substr(1, line.size() - 1));
			}
			else if (line[0] == 'y')
			{
				// Init player pos Y
				nInitPlayerPosY = std::stoi(line.substr(1, line.size() - 1));
			}
			else
			{
				// Rest of file is the level
				sLevel += line;
			}
		}
	}

	return true;
}

std::wstring cLevel::GetLevel()
{
	return sLevel;
}

int cLevel::GetWidth()
{
	return nLevelWidth;
}

int cLevel::GetHeight()
{
	return nLevelHeight;
}

int cLevel::GetInitPlayerPosX()
{
	return nInitPlayerPosX;
}

int cLevel::GetInitPlayerPoxY()
{
	return nInitPlayerPosY;
}

bool cLevel::PopulateEnnemies(std::vector<cDynamicCreature*>& vecDyns, std::string levelName)
{
	std::wifstream file(levelName);

	if (file)
	{
		std::wstring line;

		while (std::getline(file, line))
		{
			std::wstring temp;
			std::vector<std::wstring> parts;
			std::wstringstream wss(line);
			while (std::getline(wss, temp, L','))
			{
				parts.push_back(temp);
			}

			// Populate a Blade Knight
			if (parts[0] == L"bladeKnight")
			{
				cDynamicCreature* mob = new cDynamicCreatureBladeKnight(this);
				vecDyns.push_back(mob);

				// get position
				mob->px = std::stoi(parts[1]);
				mob->py = std::stoi(parts[2]);
			}

			// Populate a bomber
			if (parts[0] == L"bomber")
			{
				cDynamicCreature* mob = new cDynamicCreatureBomber(this);
				vecDyns.push_back(mob);

				// get position
				mob->px = std::stoi(parts[1]);
				mob->py = std::stoi(parts[2]);
			}

			// Populate a Bronto Burt
			if (parts[0] == L"brontoBurt")
			{
				cDynamicCreature* mob = new cDynamicCreatureBrontoBurt(this, std::stof(parts[1]));
				vecDyns.push_back(mob);

				// get position
				mob->px = std::stoi(parts[2]);
				mob->py = std::stoi(parts[3]);
			}

			// Populate a rocky
			if (parts[0] == L"rocky")
			{
				cDynamicCreature* mob = new cDynamicCreatureRocky(this);
				vecDyns.push_back(mob);

				// get position
				mob->px = std::stoi(parts[1]);
				mob->py = std::stoi(parts[2]);
			}

			// Populate a scarfy
			if (parts[0] == L"scarfy")
			{
				cDynamicCreature* mob = new cDynamicCreatureScarfy(this);
				vecDyns.push_back(mob);

				// get position
				mob->px = std::stoi(parts[1]);
				mob->py = std::stoi(parts[2]);
			}

			// Populate a waddle dee
			if (parts[0] == L"waddleDee")
			{
				cDynamicCreature* mob = new cDynamicCreatureWaddleDee(this);
				vecDyns.push_back(mob);

				// get position
				mob->px = std::stoi(parts[1]);
				mob->py = std::stoi(parts[2]);
			}
		}
	}

	return true;
}

bool cLevel::PopulateBoss(std::vector<cDynamicCreature*>& vecDyns, int currentLvl)
{
	// TODO en fonction du numéro de niveau instancier le boss correspondant
	cDynamicCreature* mob = nullptr;
	switch (currentLvl)
	{
		case 0:
		{
			mob = new cDynamicCreatureWhispyWood(this);
			vecDyns.push_back(mob);
			mob->px = cfWhispyWoodPX;
			mob->py = cfWhispyWoodPY;
		}
		break;
		case 1:
		{
			mob = new cDynamicCreatureMrShineMrBright(this);
			vecDyns.push_back(mob);
			mob->px = cfMrShineMrBrightPX;
			mob->py = cfMrShineMrBrightPY;
		}
		break;
		case 2:
		{
			// TODO Dyna Blade
			mob = new cDynamicCreatureWaddleDee(this);
			vecDyns.push_back(mob);
			mob->px = 5.0f;
			mob->py = 5.0f;
		}
		break;
		case 3:
		{
			// TODO kracko
			mob = new cDynamicCreatureWaddleDee(this);
			vecDyns.push_back(mob);
			mob->px = 5.0f;
			mob->py = 5.0f;
		}
		break;
		case 4:
		{
			// TODO Meta Knight
			mob = new cDynamicCreatureWaddleDee(this);
			vecDyns.push_back(mob);
			mob->px = 5.0f;
			mob->py = 5.0f;
		}
		break;
		case 5:
		{
			// TODO Roi Dadidou
			mob = new cDynamicCreatureWaddleDee(this);
			vecDyns.push_back(mob);
			mob->px = 5.0f;
			mob->py = 5.0f;
		}
		break;
	}

	mob->bIsVacuumable = false;
	mob->bIsBoss = true;
	mob->bIsKnockable = false;

	return true;
}

std::vector<std::string> cLevel::LoadLevelsList()
{
	std::vector<std::string> levelList;
	levelList.push_back("assets/lvls/lvl1.txt");
	levelList.push_back("assets/lvls/lvl2.txt");
	levelList.push_back("assets/lvls/lvl3.txt");
	levelList.push_back("assets/lvls/lvl4.txt");
	levelList.push_back("assets/lvls/lvl5.txt");
	levelList.push_back("assets/lvls/lvl6.txt");

	return levelList;
}

std::vector<std::string> cLevel::LoadBossLevelsList()
{
	std::vector<std::string> bossLevels;
	bossLevels.push_back("assets/lvls/Bosslvl1.txt");
	bossLevels.push_back("assets/lvls/Bosslvl2.txt");
	bossLevels.push_back("assets/lvls/Bosslvl3.txt");
	bossLevels.push_back("assets/lvls/Bosslvl4.txt");
	bossLevels.push_back("assets/lvls/Bosslvl5.txt");
	bossLevels.push_back("assets/lvls/Bosslvl6.txt");

	return bossLevels;
}

std::vector<std::string> cLevel::LoadLevelsEnnemiesList()
{
	std::vector<std::string> levelsEnnemies;
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl1.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl2.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl3.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl4.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl5.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl6.txt");

	return levelsEnnemies;
}

std::vector<std::string> cLevel::LoadLevelsTilesList()
{
	std::vector<std::string> levelsTiles;
	levelsTiles.push_back("assets/gfx/tilemap00.png");
	levelsTiles.push_back("assets/gfx/tilemap01.png");
	levelsTiles.push_back("assets/gfx/tilemap02.png");
	levelsTiles.push_back("assets/gfx/tilemap03.png");
	levelsTiles.push_back("assets/gfx/tilemap04.png");
	levelsTiles.push_back("assets/gfx/tilemap05.png");

	return levelsTiles;
}

std::vector<std::string> cLevel::LoadLevelsGrdTilesList()
{
	std::vector<std::string> groundTiles;
	groundTiles.push_back("assets/gfx/grdTileMap00.png");
	groundTiles.push_back("assets/gfx/grdTileMap01.png");
	groundTiles.push_back("assets/gfx/grdTileMap02.png");
	groundTiles.push_back("assets/gfx/grdTileMap03.png");
	groundTiles.push_back("assets/gfx/grdTileMap04.png");
	groundTiles.push_back("assets/gfx/grdTileMap05.png");

	return groundTiles;
}

std::vector<std::string> cLevel::LoadLevelsBackGroundList()
{
	std::vector<std::string> levelsBackgrounds;
	levelsBackgrounds.push_back("assets/gfx/BckGrd00.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd01.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd02.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd03.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd04.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd05.png");

	return levelsBackgrounds;
}

std::vector<std::string> cLevel::LoadLevelsBossBckGrdList()
{
	std::vector<std::string> bossLevelsBackgrounds;
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd00.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd01.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd02.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd03.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd04.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd05.png");

	return bossLevelsBackgrounds;
}
