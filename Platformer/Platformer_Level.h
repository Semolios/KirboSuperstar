#ifndef DEF_LEVEL
#define DEF_LEVEL

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicCreatureBladeKnight.h"
#include "Platformer_DynamicCreatureBomber.h"
#include "Platformer_DynamicCreatureBrontoBurt.h"
#include "Platformer_DynamicCreatureFrosty.h"
#include "Platformer_DynamicCreatureMrShineMrBright.h"
#include "Platformer_DynamicCreatureRocky.h"
#include "Platformer_DynamicCreatureScarfy.h"
#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_DynamicCreatureWhispyWood.h"

#include <mutex>

class OneLoneCoder_Platformer;

constexpr int nMaxLvlThreads = 16;

static std::atomic<int> nLvlWorkerComplete;

class cLevel
{
private:
	const float cfWhispyWoodPX = 14.0f;
	const float cfWhispyWoodPY = 5.0f;

	const float cfMrShineMrBrightPX = 7.5f;
	const float cfMrShineMrBrightPY = 3.0f;

	const float cfFrostyPX = 13.0f;
	const float cfFrostyPY = 7.0f;

	std::wstring sLevel;
	int nLevelWidth = 0;
	int nLevelHeight = 0;
	int nInitPlayerPosX = 0;
	int nInitPlayerPosY = 0;
	int nCurrentLevel = 0;
	int nUnlockedLevel = 6; // TODO PENSER A REMETTRE A 1 A LA FIN

	std::vector<std::string> levels;
	std::vector<std::string> levelsEnnemies;
	std::vector<std::string> levelsTiles;
	std::vector<std::string> levelsMusics;
	std::vector<std::string> groundTiles;
	std::vector<std::string> levelsBackgrounds;
	std::vector<std::string> bossLevels;
	std::vector<std::string> bossLevelsMusics;
	std::vector<std::string> bossLevelsBackgrounds;

	struct WorkerThread
	{
		int sx;
		int sy;
		int nVisibleTilesX;
		int nVisibleTilesY;
		float fOffsetX;
		float fOffsetY;
		float fTileOffsetX;
		float fTileOffsetY;

		std::condition_variable cvStart;
		bool alive = true;
		std::mutex mux;
		int screen_width = 0;
		cLevel* lvl;

		std::thread thread;

		void Start(int x, int y, int visibleTilesX, int visibleTilesY, float offsetX, float offsetY, float tileOffsetX, float tileOffsetY, cLevel* level)
		{
			sx = x;
			sy = y;
			nVisibleTilesX = visibleTilesX;
			nVisibleTilesY = visibleTilesY;
			fOffsetX = offsetX;
			fOffsetY = offsetY;
			fTileOffsetX = tileOffsetX;
			fTileOffsetY = tileOffsetY;
			lvl = level;

			std::unique_lock<std::mutex> lm(mux);
			cvStart.notify_one();
		}

		void SelectTile()
		{
			while (alive)
			{
				std::unique_lock<std::mutex> lm(mux);
				cvStart.wait(lm);

				lvl->SelectTile(sx, sy, nVisibleTilesX, nVisibleTilesY, fOffsetX, fOffsetY, fTileOffsetX, fTileOffsetY);

				nLvlWorkerComplete++;
			}
		}
	};

	WorkerThread workers[nMaxLvlThreads];

public:
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
	bool PopulateBoss(std::vector<cDynamicCreature*>& vecDyns);
	void DrawTiles(int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY);
	void SelectTile(int startX, int endX, int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY);
	void InitialiseThreadPool();
	void DrawGroundTile(int x, int y, float fTileOffsetX, float fTileOffsetY, float fCamOffsetX, float fCamOffsetY, olc::Sprite* spriteTiles, wchar_t tile);
	wchar_t GetTile(int x, int y);
	void SetTile(int x, int y, wchar_t c);

	void LoadLevelsList();
	void LoadBossLevelsList();
	void LoadLevelsEnnemiesList();
	void LoadLevelsTilesList();
	void LoadLevelsGrdTilesList();
	void LoadLevelsBackGroundList();
	void LoadLevelsBossBckGrdList();
	void LoadLevelsMusics();
	void LoadBossLevelsMusics();

	std::string GetLevelName();
	std::string GetBossLevelName();
	std::string GetLevelsEnnemiesName();
	std::string GetLevelsTilesName();
	std::string GetLevelsGrdTilesName();
	std::string GetLevelsBackGroundName();
	std::string GetLevelsBossBckGrdName();
	std::string GetLevelsMusicsName();
	std::string GetBossLevelsMusicsName();

	int GetCurrentLvl();
	void SetCurrentLvl(int selectedLvl);
	int GetUnlockedLvl();
	void UnlockNewLvl();

	bool IsLastOfGame();
	bool IsLastUnlocked();
};

#endif // !DEF_LEVEL