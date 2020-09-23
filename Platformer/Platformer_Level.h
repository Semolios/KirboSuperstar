#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class cLevel
{
public:
	std::wstring sLevel;
	std::string sLevelName;
	int nLevelWidth = 0;
	int nLevelHeight = 0;

public:
	cLevel();

public:
	bool LoadLevel(std::string levelName);
	std::wstring GetLevel();
	int GetWidth();
	int GetHeight();
};

