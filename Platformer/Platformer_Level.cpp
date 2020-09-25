#include "Platformer_Level.h"
#include <strstream>

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
			if (nLevelWidth == 0)
			{
				// First line is width
				nLevelWidth = std::stoi(line);
			}
			else if (nLevelHeight == 0)
			{
				// second line is height
				nLevelHeight = std::stoi(line);
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