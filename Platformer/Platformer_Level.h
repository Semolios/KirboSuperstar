#ifndef DEF_LEVEL
#define DEF_LEVEL

#pragma once

#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicCreatureBladeKnight.h"
#include "Platformer_DynamicCreatureBomber.h"
#include "Platformer_DynamicCreatureBrontoBurt.h"
#include "Platformer_DynamicCreatureFrosty.h"
#include "Platformer_DynamicCreatureKingDDD.h"
#include "Platformer_DynamicCreatureKracko.h"
#include "Platformer_DynamicCreatureMrShineMrBright.h"
#include "Platformer_DynamicCreatureRocky.h"
#include "Platformer_DynamicCreatureScarfy.h"
#include "Platformer_DynamicCreatureSSTierMetaKnight.h"
#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_DynamicCreatureWhispyWood.h"
#include "Platformer_DynamicMovingPlatform.h"
#include "Platformer_DynamicTeleport.h"
#include "Platformer_DynamicWind.h"

class OneLoneCoder_Platformer;

class cLevel
{
public:
	static OneLoneCoder_Platformer* engine;

	cLevel();

	bool LoadLevel(std::string levelName);
	std::wstring GetLevel();
	int GetWidth();
	int GetHeight();
	int GetInitPlayerPosX();
	int GetInitPlayerPosY();
	bool PopulateEnnemies(std::vector<cDynamicCreature*>& vecDyns, std::string levelName);
	bool PopulateMechanisms(std::string levelName);
	bool PopulateBoss(std::vector<cDynamicCreature*>& vecDyns);
	void DrawTiles(int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY);
	void SelectTile(int startX, int endX, int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY);
	void DrawGroundTile(int x, int y, float fTileOffsetX, float fTileOffsetY, float fCamOffsetX, float fCamOffsetY, olc::Decal* decalTiles, wchar_t tile);
	wchar_t GetTile(int x, int y);
	void SetTile(int x, int y, wchar_t c);

	void LoadLevelsList();
	void LoadBossesList();
	void LoadEnnemiesList();
	void LoadMechanismsList();
	void LoadBossMechanismsList();
	void LoadSpecialTilesList();
	void LoadGroundTilesList();
	void LoadBackGroundsList();
	void LoadBossesBackGroundsList();
	void LoadMusicsList();
	void LoadBossesMusicsList();

	std::string GetName();
	std::string GetBoss();
	std::string GetEnnemies();
	std::string GetMechanisms();
	std::string GetBossMechanisms();
	std::string GetSpecialTilesSpritesheet();
	std::string GetGroundTilesSpritesheet();
	std::string GetBackGroundSpritesheet();
	std::string GetParallax1Spritesheet();
	std::string GetParallax2Spritesheet();
	std::string GetParallax3Spritesheet();
	std::string GetBossBackGroundSpritesheet();
	std::string GetMusic();
	std::string GetBossMusic();

	int GetCurrentLvl();
	void SetCurrentLvl(int selectedLvl);
	int GetUnlockedLvl();
	void UnlockNewLvl();

	bool IsLastOfGame();
	bool IsLastUnlocked();

private:
	const float cfWhispyWoodPX = 14.0f;
	const float cfWhispyWoodPY = 5.0f;

	const float cfMrShineMrBrightPX = 7.5f;
	const float cfMrShineMrBrightPY = 3.0f;

	const float cfFrostyPX = 13.0f;
	const float cfFrostyPY = 7.0f;

	const float cfKrackoPX = 10.0f;
	const float cfKrackoPY = 4.0f;

	const float cfSSTierMetaKnightPX = 10.0f;
	const float cfSSTierMetaKnightPY = 7.0f;

	const float cfKingDDDPX = 13.0f;
	const float cfKingDDDPY = 7.0f;

	std::wstring sLevel;
	int nLevelWidth = 0;
	int nLevelHeight = 0;
	int nInitPlayerPosX = 0;
	int nInitPlayerPosY = 0;
	int nCurrentLevel = 0;
	int nUnlockedLevel = 1;

	std::vector<std::string> levels;
	std::vector<std::string> ennemies;
	std::vector<std::string> mechanisms;
	std::vector<std::string> bossMechanisms;
	std::vector<std::string> specialTiles;
	std::vector<std::string> musics;
	std::vector<std::string> groundTiles;
	std::vector<std::string> backgrounds;
	std::vector<std::string> parallaxLayer1;
	std::vector<std::string> parallaxLayer2;
	std::vector<std::string> parallaxLayer3;
	std::vector<std::string> bosses;
	std::vector<std::string> bossesMusics;
	std::vector<std::string> bossesBackgrounds;
};

#endif // !DEF_LEVEL
