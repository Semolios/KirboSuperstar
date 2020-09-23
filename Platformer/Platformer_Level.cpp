#include "Platformer_Level.h"
#include <strstream>

cLevel::cLevel()
{
}

bool cLevel::LoadLevel(std::string levelName)
{
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