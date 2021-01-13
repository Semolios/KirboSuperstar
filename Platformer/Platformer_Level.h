#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicCreatureBladeKnight.h"
#include "Platformer_DynamicCreatureBomber.h"
#include "Platformer_DynamicCreatureBrontoBurt.h"
#include "Platformer_DynamicCreatureMrShineMrBright.h"
#include "Platformer_DynamicCreatureRocky.h"
#include "Platformer_DynamicCreatureScarfy.h"
#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_DynamicCreatureWhispyWood.h"

class OneLoneCoder_Platformer;

class cLevel
{
public:
	const float cfWhispyWoodPX = 14.0f;
	const float cfWhispyWoodPY = 5.0f;

	const float cfMrShineMrBrightPX = 7.5f;
	const float cfMrShineMrBrightPY = 3.0f;

	std::wstring sLevel;
	int nLevelWidth = 0;
	int nLevelHeight = 0;
	int nInitPlayerPosX = 0;
	int nInitPlayerPosY = 0;

	static OneLoneCoder_Platformer* engine;

public:
	cLevel();

public:
	bool LoadLevel(std::string levelName);
	std::wstring GetLevel();
	int GetWidth();
	int GetHeight();
	int GetInitPlayerPosX();
	int GetInitPlayerPosY();
	bool PopulateEnnemies(std::vector<cDynamicCreature*>& vecDyns, std::string levelName);
	bool PopulateBoss(std::vector<cDynamicCreature*>& vecDyns, int currentLvl);
	void DrawTiles(int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY);
	void DrawGroundTile(int x, int y, float fTileOffsetX, float fTileOffsetY, float fCamOffsetX, float fCamOffsetY , olc::Sprite* spriteTiles, wchar_t tile);
	wchar_t GetTile(int x, int y);
	void SetTile(int x, int y, wchar_t c);
	std::vector<std::string> LoadLevelsList();
	std::vector<std::string> LoadBossLevelsList();
	std::vector<std::string> LoadLevelsEnnemiesList();
	std::vector<std::string> LoadLevelsTilesList();
	std::vector<std::string> LoadLevelsGrdTilesList();
	std::vector<std::string> LoadLevelsBackGroundList();
	std::vector<std::string> LoadLevelsBossBckGrdList();
};

