#ifndef DEF_ENGINE
#define DEF_ENGINE

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "olcPGEX_Sound.h"
#include "Zix_PGE_Controller.h"

#include "Platformer_Animator.h"
#include "Platformer_Assets.h"
#include "Platformer_Camera.h"
#include "Platformer_ControlsMenu.h"
#include "Platformer_CreditsMenu.h"
#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCeiling.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicHarmfulBloc.h"
#include "Platformer_DynamicHorizontalCrusher.h"
#include "Platformer_DynamicMovingPlatform.h"
#include "Platformer_DynamicMovingPlatformHorSine.h"
#include "Platformer_DynamicMovingPlatformHorSingle.h"
#include "Platformer_DynamicMovingPlatformVerSine.h"
#include "Platformer_DynamicMovingPlatformVerSingle.h"
#include "Platformer_DynamicProjectile.h"
#include "Platformer_DynamicProjectileBoomerang.h"
#include "Platformer_DynamicProjectileOrbital.h"
#include "Platformer_DynamicTeleport.h"
#include "Platformer_DynamicVerticalCrusher.h"
#include "Platformer_DynamicWall.h"
#include "Platformer_DynamicWind.h"
#include "Platformer_EndScreen.h"
#include "Platformer_Hitbox.h"
#include "Platformer_HUD.h"
#include "Platformer_Item.h"
#include "Platformer_Level.h"
#include "Platformer_LevelStart.h"
#include "Platformer_PauseMenu.h"
#include "Platformer_Player.h"
#include "Platformer_SelectMenu.h"
#include "Platformer_TitleScreen.h"
#include "Platformer_WorldMap.h"

#include <cTime>

class OneLoneCoder_Platformer : public olc::PixelGameEngine
{
public:
	OneLoneCoder_Platformer();

private:
	// Constant values
	const float cfGravity = 20.0f;						// Gravity
	const float cfStopTimebeforeDeadAnim = 1.5f;		// Stop time before death animation
	const float cfTimeBeforeWinAnimation = 2.0f;		// Time before animation begins when kirbo kills the boss
	const float cfBossKilledAnimation = 4.0f;			// Boss killed animation duration
	const float cfGrdDynamicOverlay = 0.08f;			// little gap to give impression the player lightly overlap the platform
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
	int sndInvincibility;
	int sndMenu;
	int sndTransition;
	int sndEndScreen;

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
	int sndIceBlow;
	int sndpatpat;
	int sndItemLaunch;
	int sndSuperstar;
	int sndFrostyJump;
	int sndFrostyGroundPound;
	int sndLightning;
	int sndElectricity;
	int sndElectricity2;
	int sndMeleeControllerRape;
	int sndDash;
	int sndHiyayayaya;
	int sndSingleSwordStrike;
	int sndMultipleSwordStrike;
	int sndHa;
	int sndTornado;
	int sndLoudScreaming;
	int sndBehold;
	int sndKingDDDJump;
	int sndKingDDDUpSmash;
	int sndKingDDDUpSmashHammer;
	int sndKingDDDSideSmash;
	int sndKingDDDDownSmashHammer;
	int sndKingDDDSideB;
	int sndSpikeBallBounce;
	int sndKingDDDDownBCharge;
	int sndKingDDDDownBPrepare;
	int sndKingDDDDownBAttack;
	int sndKingDDDUpBJump;
	int sndKingDDDUpBLanding;
	int sndKingDDDVacuum;
	int sndKingDDDUpAir;
	int sndKingDDDAirAtkVoice;
	int sndKingDDDAirAtkHammer;
	int sndItemPicked;
	int sndRockyFall;
	int sndBoom;
	int sndEnterDoor;
	int sndMenuBip;

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
	olc::Sprite* sprLock;
	cWorldMap* worldMap;

	// Level Start Screen
	olc::Sprite* sprLevelStart;
	cLevelStart* levelStart;
	int levelStartAnim = 0;

	// End Screen
	olc::Sprite* sprEndScreen;
	cEndScreen* endScreen;
	bool bPlayMusic = true;

	// Pause Menu
	olc::Sprite* sprPauseMenu;
	olc::Sprite* sprCursor;
	cPauseMenu* pauseMenu;

	// Select Menu
	olc::Sprite* sprSelectMenu;
	cSelectMenu* selectMenu;

	// Controls Menu
	olc::Sprite* sprControlsMenu;
	olc::Sprite* sprMenuBar;
	olc::Sprite* sprUnselQuit;
	olc::Sprite* sprSelQuit;
	olc::Sprite* sprUnselDefault;
	olc::Sprite* sprSelDefault;
	cControlsMenu* controlsMenu;
	std::map<std::string, olc::Key> savedControls;
	// --- Controller Sprites
	olc::Sprite* sprX;
	olc::Sprite* sprY;
	olc::Sprite* sprB;
	olc::Sprite* sprPause;
	olc::Sprite* sprUp;
	olc::Sprite* sprDown;
	olc::Sprite* sprLeft;
	olc::Sprite* sprRight;
	olc::Sprite* sprA;
	olc::Sprite* sprRBump;

	// Credits Menu
	olc::Sprite* sprCreditsMenu;
	cCreditsMenu* creditsMenu;

	// Player Sprites
	const int nTileWidth = 64;
	const int nTileHeight = 64;
	cAnimator animPlayer;

	// Ennemies
	std::vector<cDynamicCreature*> vecEnnemies;

	// Projectiles
	std::vector<cDynamicProjectile*> vecProjectiles;
	std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles;

	// Moving Platforms
	std::vector<cDynamicMovingPlatform*> vecPlatforms;
	std::map<std::string, std::vector<olc::Sprite*>> mapPlatforms;
	olc::Sprite* sprDoorSwitchOff;
	olc::Sprite* sprDoorSwitchOn;

	// Winds
	std::vector<cDynamicWind*> vecWinds;
	std::map<std::string, std::vector<olc::Sprite*>> mapWinds;

	// Teleports
	std::vector<cDynamicTeleport*> vecTeleports;
	std::map<std::string, std::vector<olc::Sprite*>> mapTeleports;

	// HUD
	cHUD* HUD;
	olc::Sprite* sprHealthBar;
	olc::Sprite* sprHealthPoint;
	olc::Sprite* sprBossHealthBar;

	// SplashScreen
	float fSplashScreenTimer;
	float fSplashScreenTime = 5.0f;

	// Controller
	ControllerManager controller;

	// Transition
	float fTransitionTimer;
	float fTransitionTime = 0.5f;
	bool bPlayTransitionSound = false;
	std::string sNextState;

	// Loading Screen
	enum
	{
		LS_CLEARSCREEN,
		LS_LEVELS,
		LS_ANIMATIONS,
		LS_PROJECTILES,
		LS_MECHANISMS,
		LS_TITLE,
		LS_WORLDMAP,
		LS_LEVELSTART,
		LS_ENDSCREEN,
		LS_PAUSEMENU,
		LS_SELECTMENU,
		LS_CONTROLSMENU,
		LS_CREDITSMENU,
		LS_HUD,
		LS_ENGINEPOINTERS,
		LS_PLAYER,
		LS_CAMERA,
		LS_SOUNDS,
		LS_ASSETS,
	} nLoadingState = LS_CLEARSCREEN;
	bool bLoadFinished = false;

	enum
	{
		GS_SPLASHSCREEN,
		GS_LOADING,
		GS_TITLE,
		GS_MAIN,
		GS_LEVELSTART,
		GS_LOADLEVEL,
		GS_WORLDMAP,
		GS_ENDSCREEN,
		GS_PAUSE,
		GS_LOADBOSSLEVEL,
		GS_SELECTMENU,
		GS_CONTROLS,
		GS_CREDITS,
		GS_CLOSE,
		GS_TRANSITION,
	} nGameState = GS_SPLASHSCREEN;

protected:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	bool GameState_LevelStart(float fElapsedTime);
	bool GameState_SplashScreen(float fElapsedTime);
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
	bool GameState_Credits(float fElapsedTime);
	bool GameState_Close(float fElapsedTime);
	bool GameState_Transition(float fElapsedTime);

	void DestroyAllDynamics();
	void LoadLevelProperties();

public:
	// Keyboard function
	bool GetAnyKey();
	olc::Key GetFirstKeyPressed();

	// Level detection functions
	bool IsSolidTile(wchar_t tile);
	bool IsBreakableTile(wchar_t tile);
	bool IsSemiSolidTile(wchar_t tile);

	// Projectiles functions
	void AddProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit = true, int corner = 0, bool breackableAgainstTiles = true, float fDrag = -3.0f, std::string sound = "", bool bouncy = false, std::string bounceSound = "");
	void AddBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit = true, int corner = 0, std::string sound = "");
	void AddOrbital(float ox, float oy, bool bFriend, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit = true, int corner = 0, float cx = 0.0f, float cy = 0.0f, float angrot = 0.0f, std::string sound = "");

	// Platforms functions
	void AddPlatform(float ox, float oy, std::string sprite, std::wstring iced);
	void AddHorizontalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger);
	void AddVerticalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger);
	void AddHorizontalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float tx, float vx, std::wstring trigger);
	void AddVerticalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float ty, float vy, std::wstring trigger);
	void AddWall(float ox, float oy, std::string sprite, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY);
	void AddCeiling(float ox, float oy, std::string sprite, std::wstring topSolid, std::wstring linkToPreviousPtfm);
	void AddHorizontalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime);
	void AddVerticalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime);
	void AddHarmfulBloc(float ox, float oy, std::string sprite, float dmg);
	std::vector<cDynamicMovingPlatform*> GetClosePlatforms(float px, float py);
	olc::Sprite* GetDoorSwitch(bool on);

	// Winds functions
	void AddWind(float ox, float oy, std::string sprite, std::wstring direction, float power);
	std::vector<cDynamicWind*> GetCloseWinds(float px, float py);

	// Teleports functions
	void AddTeleport(float ax, float ay, float bx, float by, std::string sprite);
	std::vector<cDynamicTeleport*> GetCloseTeleport(float px, float py);
	std::vector<cDynamicTeleport*> GetCloseTeleportDest(float px, float py);

	// Engine properties/modifications functions
	float GetTileWidth();
	float GetTileHeight();
	float GetGravityValue();
	float GetGroundDynamicOverlay();
	bool IsInBossLevel();
	void SetbInBossLevel(bool inBossLevel);
	bool IsBossKilled();
	void SetbBossKilled(bool bossKilled);
	void SetGameState(std::string gameState);
	void UpdateWinTimer(float fElapsedTime);
	float GetWinTimer();
	void ResetVariables();
	void BreakLoop();
	olc::Sprite* GetBackGround();
	olc::Sprite* GetTilesSprites();
	olc::Sprite* GetGroundTiles();
	olc::Sprite* GetDoorSpr();
	void ReturnToWorldMap();
	void GoToControlsMenu();

	// Pause menu functions
	void SetPlayerChoice(int choice);

	// Camera functions
	cCamera* GetCamera();
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
	void WindEffect(float direction, float windPower, bool activate);

	// Sound functions
	void AddSharedSound(std::string name, int sound, std::string fileName);
	void PlaySample(std::string name, bool loop = false, bool dontPlayIfAlreadyPlaying = false);
	void StopSample(std::string name);
	bool IsSamplePlaying(std::string name);
	void PlayLevelMusic();
	void StopLevelMusic();

	// Player functions
	void ChangeKirboVelocities(float vx, float vy);
	void SetKirboGrabbed(bool grabbed);
	void ChangeKirboAnimation(std::string animation);
	void SetKirboVisible(bool visible);
	void SetKirboAttackable(bool attackable);
	void SetKirboPositions(float px, float py);
	bool KirboCollisionWithEnnemy(cDynamic* object);
	bool IsKirboAttackable();
	void HealPlayer();
	void PlayerGetCandy(float candyTime);
	void BuffPlayerDamage();
	void BuffPlayerDefense();

	// Controller functions
	ControllerManager* GetController();

	// Loading Screen functions
	void UpdateProgressBar(std::string loadPercent);

	// Controls menu functions
	void ApplyControls();
	olc::Key ToOlcKey(std::string key);
	std::string olcKeyToStr(olc::Key key);
	olc::Key GetSavedControls(std::string control);

	// Transition function
	void TransitionTo(std::string newState, bool playTransitionSound);

	//	Font functions
	void DrawKirboString(int x, int y, std::string text, int scale = 1, bool centered = false);

	// Other
	std::string ToStr(std::wstring str);
};

#endif // !DEF_ENGINE
