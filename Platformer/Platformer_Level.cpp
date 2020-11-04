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

			// Populate a bomber
			if (parts[0] == L"bomber")
			{
				cDynamicCreature* mob = new cDynamicCreatureBomber(this);
				vecDyns.push_back(mob);
				cDynamicCreatureBomber::engine = engine;

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
