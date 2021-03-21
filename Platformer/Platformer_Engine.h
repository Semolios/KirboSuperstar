#ifndef DEF_ENGINE
#define DEF_ENGINE

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "olcPGEX_Sound.h"

#include "Platformer_Animator.h"
#include "Platformer_Assets.h"
#include "Platformer_Camera.h"
#include "Platformer_ControlsMenu.h"
#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicProjectile.h"
#include "Platformer_DynamicProjectileBoomerang.h"
#include "Platformer_EndScreen.h"
#include "Platformer_Hitbox.h"
#include "Platformer_HUD.h"
#include "Platformer_Level.h"
#include "Platformer_PauseMenu.h"
#include "Platformer_Player.h"
#include "Platformer_SelectMenu.h"
#include "Platformer_TitleScreen.h"
#include "Platformer_Transition.h"
#include "Platformer_WorldMap.h"

#include <cTime> 

class OneLoneCoder_Platformer : public olc::PixelGameEngine
{
public:
	OneLoneCoder_Platformer();

private:
	// Constant values
	const float cfGravity = 20.0f;						// Gravity
	const float cfDrag = -3.0f;							// Drag
	const float cfStopTimebeforeDeadAnim = 1.5f;		// Stop time before death animation
	const float cfTimeBeforeWinAnimation = 2.0f;		// Time before animation begins when kirbo kills the boss
	const float cfBossKilledAnimation = 4.0f;			// Boss killed animation duration
	const float cfGrdDynamicOverlay = 0.08f;			// little gap to give impression the player lightly overlap the platform
	const float cfGameCloseTime = 2.0f;					// Application wait a little time before closing because sound take a little time to be shut off
	const int cnSpriteOffsetX = 64;						// Sprite offset X because kirbo sprites are 192 * 192 pixels
	const int cnSpriteOffsetY = 64;						// Sprite offset Y because kirbo sprites are 192 * 192 pixels

	// Engine properties

	// Level storage
	cLevel* level;
	olc::Sprite* sprBackground;
	olc::Sprite* spriteTiles;
	olc::Sprite* sprGrdTiles;
	olc::Sprite* sprDoor;

	// Sounds
	int sndTitleScreen;
	int sndWorldMap;
	int sndLevelMusic;
	int sndBossLevelMusic;
	int sndBossKilled;
	int sndWind;

	// Sounds shared with other classes
	std::map<std::string, int> sharedSounds;
	int sndWhispyScream;
	int sndLoseLife;
	int sndExplosion;
	int sndPunch;
	int sndSlap;
	int sndKirboWalk;
	int sndKirboJump;
	int sndKirboFly;
	int sndKirboHit;
	int sndKirboBeginVacuum;
	int sndKirboVacuum;
	int sndKirboSwallow;
	int sndWetSlap;
	int sndEarthQuake;
	int sndBlow;
	int sndInhale;
	int sndPoyo01;
	int sndPoyo02;
	int sndSunShootingMoon;
	int sndSunShootUp;
	int sndBeamCharge;
	int sndBeam;
	int sndVolcanoBoom;

	// engine Properties
	cPlayer* player;
	bool bBreakLoop = false;
	float fStopTimebeforeDeadAnim = 0.0f;
	float fWindDirection = 1.0f;
	float fWindPower = 25.0f;
	bool bBossKilled = false;
	bool bInBossLvl = false;
	bool bWind = false;
	float fWaitBeforeWinAnimation = 0.0f;
	float fWinTimer = 0.0f;

	// Camera Properties
	cCamera* camera;

	// Title screen
	olc::Sprite* sprTitleScreen;
	cTitleScreen* titleScreen;

	// World Map
	olc::Sprite* sprWorldMap;
	cWorldMap* worldMap;

	// Transition Screen
	olc::Sprite* sprTransition;
	cTransition* transition;
	int transitionAnim = 0;

	// End Screen
	olc::Sprite* sprEndScreen;
	cEndScreen* endScreen;

	// Pause Screen
	olc::Sprite* sprPauseMenu;
	olc::Sprite* sprCursor;
	cPauseMenu* pauseMenu;

	// Select Menu
	olc::Sprite* sprSelectMenu;
	cSelectMenu* selectMenu;

	// Controls menu
	olc::Sprite* sprControlsMenu;
	cControlsMenu* controlsMenu;

	// Sprites
	const int nTileWidth = 64;
	const int nTileHeight = 64;
	cAnimator animPlayer;

	// Ennemies
	std::vector<cDynamicCreature*> vecEnnemies;

	// Projectiles
	std::vector<cDynamicProjectile*> vecProjectiles;
	std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles;

	// HUD
	cHUD* HUD;
	olc::Sprite* sprHealthBar;
	olc::Sprite* sprHealthPoint;
	olc::Sprite* sprBossHealthBar;

	enum
	{
		GS_LOADING,
		GS_TITLE,
		GS_MAIN,
		GS_TRANSITION,
		GS_LOADLEVEL,
		GS_WORLDMAP,
		GS_ENDSCREEN,
		GS_PAUSE,
		GS_LOADBOSSLEVEL,
		GS_SELECTMENU,
		GS_CONTROLS,
		GS_CLOSE,
	} nGameState = GS_LOADING;

protected:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	bool GameState_Transition(float fElapsedTime);
	bool GameState_Loading(float fElapsedTime);
	bool GameState_LoadLevel(float fElapsedTime);
	bool GameState_Title(float fElapsedTime);
	bool GameState_Main(float fElapsedTime);
	bool GameState_WorldMap(float fElapsedTime);
	bool GameState_EndScreen(float fElapsedTime);
	bool GameState_PauseMenu(float fElapsedTime);
	bool GameState_LoadBossLevel(float fElapsedTime);
	bool GameState_SelectMenu(float fElapsedTime);
	bool GameState_Controls(float fElapsedTime);
	bool GameState_Close(float fElapsedTime);

	void LoadLevelProperties();

public:
	bool IsSolidTile(wchar_t tile);
	bool IsSemiSolidTile(wchar_t tile);
	cDynamicProjectile* CreateProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit = true, int corner = 0);
	cDynamicProjectile* CreateBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit = true, int corner = 0);
	void AddProjectile(cDynamicProjectile* proj);
	float GetTileWidth();
	float GetTileHeight();
	float GetGravityValue();
	float GetGroundDynamicOverlay();
	bool IsInBossLevel();
	void SetbInBossLevel(bool inBossLevel);
	bool IsBossKilled();
	void SetbBossKilled(bool bossKilled);
	void SetGameState(std::string gameState);
	float GetDragValue();
	void SetPlayerChoice(int choice);
	float GetGrdDynamicOverlay();
	void UpdateWinTimer(float fElapsedTime);
	float GetWinTimer();
	void ResetVariables();
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
	void WindEffect(float direction, float windPower, bool activate);
	void BreakLoop();
	olc::Sprite* GetBackGround();
	olc::Sprite* GetTilesSprites();
	olc::Sprite* GetGroundTiles();
	olc::Sprite* GetDoorSpr();
	void ReturnToWorldMap();
	void AddSharedSound(std::string name, int sound, std::string fileName);
	int GetSound(std::string name);
};

#endif // !DEF_ENGINE