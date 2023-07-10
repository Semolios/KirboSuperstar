#ifndef DEF_ENGINE
#define DEF_ENGINE

#include "olcSoundWaveEngine.h"
#include "Zix_PGE_Controller.h"

#include "Platformer_Animator.h"
#include "Platformer_Assets.h"
#include "Platformer_Camera.h"
#include "Platformer_ControlsMenu.h"
#include "Platformer_SoundMenu.h"
#include "Platformer_ScreenModeMenu.h"
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
#include "Platformer_Level.h"
#include "Platformer_LevelStart.h"
#include "Platformer_PauseMenu.h"
#include "Platformer_Player.h"
#include "Platformer_SelectMenu.h"
#include "Platformer_TitleScreen.h"
#include "Platformer_WorldMap.h"

#include <cTime>
#include <Lmcons.h>

class cCamera;
class cControlsMenu;
class cCreditsMenu;
class cPauseMenu;
class cPlayer;
class cScreenModeMenu;
class cSelectMenu;
class cSoundMenu;

class OneLoneCoder_Platformer : public olc::PixelGameEngine
{
public:
	OneLoneCoder_Platformer();

	// Keyboard function
	bool GetAnyKey();
	olc::Key GetFirstKeyPressed();

	// Level detection functions
	bool IsSolidTile(wchar_t tile);
	bool IsBreakableTile(wchar_t tile);
	bool IsSemiSolidTile(wchar_t tile);

	// Projectiles functions
	void AddProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit = true, int corner = 0, bool breackableAgainstTiles = true, float fDrag = -3.0f, std::string sound = "", bool bouncy = false, std::string bounceSound = "", bool scenery = false, std::string effect = "", float effectDuration = 0.0f, bool pickable = false);
	void AddBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit = true, int corner = 0, std::string sound = "", bool scenery = false);
	void AddOrbital(float ox, float oy, bool bFriend, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit = true, int corner = 0, float cx = 0.0f, float cy = 0.0f, float angrot = 0.0f, std::string sound = "", bool scenery = false);
	void AddPreparedProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit = true, int corner = 0, bool breackableAgainstTiles = true, float fDrag = -3.0f, std::string sound = "", bool bouncy = false, std::string bounceSound = "", bool scenery = true);

	// Platforms functions
	void AddPlatform(float ox, float oy, std::string sprite, std::wstring iced);
	void AddScenery(float ox, float oy, std::string sprite);
	void AddHorizontalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger);
	void AddVerticalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger);
	void AddHorizontalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float tx, float vx, std::wstring trigger);
	void AddVerticalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float ty, float vy, std::wstring trigger);
	void AddWall(float ox, float oy, std::string sprite, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY);
	void AddCeiling(float ox, float oy, std::string sprite, std::wstring topSolid, std::wstring linkToPreviousPtfm);
	void AddHorizontalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime);
	void AddVerticalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime);
	void AddHarmfulBloc(float ox, float oy, std::string sprite, float dmg, std::wstring tangible);
	std::vector<cDynamicMovingPlatform*> GetClosePlatforms(float px, float py);
	olc::Decal* GetDoorSwitch(bool on);

	// Winds functions
	void AddWind(float ox, float oy, std::string sprite, std::wstring direction, float power);
	std::vector<cDynamicWind*> GetCloseWinds(float px, float py);

	// Teleports functions
	void AddTeleport(float ax, float ay, float bx, float by, std::string sprite);
	std::vector<cDynamicTeleport*> GetCloseTeleport(float px, float py);
	std::vector<cDynamicTeleport*> GetCloseTeleportDest(float px, float py);
	void RespawnEnnemies(cLevel* level);

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
	olc::Decal* GetBackGroundDecal();
	olc::Decal* GetParallax1Decal();
	olc::Decal* GetParallax2Decal();
	olc::Decal* GetSpecialTilesDecal();
	olc::Decal* GetGroundTilesDecal();
	olc::Decal* GetDoorDecal();
	void ReturnToWorldMap(bool drawGame);
	void GoToControlsMenu();
	void GoToSoundMenu();
	void GoToScreenModeMenu();
	void HitStop();
	void BossHitStop();

	// Pause menu functions
	void SetPlayerChoice(int choice);

	// Camera functions
	cCamera* GetCamera();
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
	void WindEffect(float direction, float windPower, bool activate);

	// Sound functions
	void AddSharedSound(std::string name, olc::sound::Wave* sound, std::string fileName);
	void PlaySample(std::string name, bool loop = false, bool dontPlayIfAlreadyPlaying = false);
	void StopSample(std::string name);
	bool IsSamplePlaying(std::string name);
	void PlayLevelMusic();
	void StopLevelMusic();
	void UpdateVolume(int volume);

	// Player functions
	void ChangeKirboVelocities(float vx, float vy);
	void SetKirboGrabbed(bool grabbed);
	void ChangeKirboAnimation(std::string animation);
	void SetKirboVisible(bool visible);
	void SetKirboAttackable(bool attackable);
	void SetKirboPositions(float px, float py);
	bool KirboCollisionWithEnnemy(cDynamic* object);
	bool IsKirboAttackable();
	void MaxHealPlayer();
	void HealPlayer(int hp);
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
	void TransitionTo(std::string newState, bool playTransitionSound, bool drawGame);

	//	Font functions
	void DrawKirboString(int x, int y, std::string text, int scale = 1, bool centered = false);

	// Ennemies
	std::vector<cDynamicCreature*> GetCloseEnnemies(float px, float py);

	// Other
	std::string ToStr(std::wstring str);
	void UpdateGame(float fElapsedTime, float* angle, float* offsetX, float* offsetY);
	void DrawGame(float fElapsedTime, float angle, float offsetX, float offsetY);

private:
	// Constant values
	const float cfGravity = 20.0f;						// Gravity
	const float cfStopTimebeforeDeadAnim = 1.5f;		// Stop time before death animation
	const float cfTimeBeforeWinAnimation = 2.0f;		// Time before animation begins when kirbo kills the boss
	const float cfBossKilledAnimation = 4.0f;			// Boss killed animation duration
	const float cfGrdDynamicOverlay = 0.08f;			// little gap to give impression the player lightly overlap the platform
	const float cfHitStopDuration = 0.1f;				// Little hit stop when hitting an ennemy
	const float cfBossHitStopDuration = 0.5;			// Little hit stop when boss killed
	const int cnSpriteOffsetX = 64;						// Sprite offset X because kirbo sprites are 192 * 192 pixels
	const int cnSpriteOffsetY = 64;						// Sprite offset Y because kirbo sprites are 192 * 192 pixels

	// Engine properties

	// Level storage
	cLevel* level;
	olc::Sprite sprBackground;
	olc::Sprite sprParallax1;
	olc::Sprite sprParallax2;
	olc::Sprite sprSpecialTiles;
	olc::Sprite sprGrdTiles;
	olc::Sprite sprDoor;
	olc::Decal* decBackground;
	olc::Decal* decParallax1;
	olc::Decal* decParallax2;
	olc::Decal* decSpecialTiles;
	olc::Decal* decGrdTiles;
	olc::Decal* decDoor;

	// Sounds
	olc::sound::WaveEngine waveEngine;
	olc::sound::Wave sndTitleScreen;
	olc::sound::Wave sndWorldMap;
	olc::sound::Wave sndLevelMusic;
	olc::sound::Wave sndBossLevelMusic;
	olc::sound::Wave sndBossKilled;
	olc::sound::Wave sndWind;
	olc::sound::Wave sndInvincibility;
	olc::sound::Wave sndMenu;
	olc::sound::Wave sndTransition;
	olc::sound::Wave sndEndScreen;

	olc::sound::PlayingWave pwTitleScreen;
	olc::sound::PlayingWave pwWorldMap;
	olc::sound::PlayingWave pwLevelMusic;
	olc::sound::PlayingWave pwBossLevelMusic;
	olc::sound::PlayingWave pwBossKilled;
	olc::sound::PlayingWave pwWind;
	olc::sound::PlayingWave pwInvincibility;
	olc::sound::PlayingWave pwMenu;
	olc::sound::PlayingWave pwTransition;
	olc::sound::PlayingWave pwEndScreen;

	// Sounds shared with other classes
	std::map<std::string, olc::sound::Wave*> sharedSounds;
	std::map<std::string, olc::sound::PlayingWave> sharedPlaying;
	olc::sound::Wave sndWhispyScream;
	olc::sound::Wave sndLoseLife;
	olc::sound::Wave sndExplosion;
	olc::sound::Wave sndPunch;
	olc::sound::Wave sndSlap;
	olc::sound::Wave sndKirboWalk;
	olc::sound::Wave sndKirboJump;
	olc::sound::Wave sndKirboFly;
	olc::sound::Wave sndKirboHit;
	olc::sound::Wave sndKirboBeginVacuum;
	olc::sound::Wave sndKirboVacuum;
	olc::sound::Wave sndKirboSwallow;
	olc::sound::Wave sndWetSlap;
	olc::sound::Wave sndEarthQuake;
	olc::sound::Wave sndBlow;
	olc::sound::Wave sndInhale;
	olc::sound::Wave sndPoyo01;
	olc::sound::Wave sndPoyo02;
	olc::sound::Wave sndSunShootingMoon;
	olc::sound::Wave sndSunShootUp;
	olc::sound::Wave sndBeamCharge;
	olc::sound::Wave sndBeam;
	olc::sound::Wave sndVolcanoBoom;
	olc::sound::Wave sndIceBlow;
	olc::sound::Wave sndpatpat;
	olc::sound::Wave sndItemLaunch;
	olc::sound::Wave sndSuperstar;
	olc::sound::Wave sndFrostyJump;
	olc::sound::Wave sndFrostyGroundPound;
	olc::sound::Wave sndLightning;
	olc::sound::Wave sndElectricity;
	olc::sound::Wave sndElectricity2;
	olc::sound::Wave sndMeleeControllerRape;
	olc::sound::Wave sndDash;
	olc::sound::Wave sndHiyayayaya;
	olc::sound::Wave sndSingleSwordStrike;
	olc::sound::Wave sndMultipleSwordStrike;
	olc::sound::Wave sndHa;
	olc::sound::Wave sndTornado;
	olc::sound::Wave sndLoudScreaming;
	olc::sound::Wave sndBehold;
	olc::sound::Wave sndKingDDDJump;
	olc::sound::Wave sndKingDDDUpSmash;
	olc::sound::Wave sndKingDDDUpSmashHammer;
	olc::sound::Wave sndKingDDDSideSmash;
	olc::sound::Wave sndKingDDDDownSmashHammer;
	olc::sound::Wave sndKingDDDSideB;
	olc::sound::Wave sndSpikeBallBounce;
	olc::sound::Wave sndKingDDDDownBCharge;
	olc::sound::Wave sndKingDDDDownBPrepare;
	olc::sound::Wave sndKingDDDDownBAttack;
	olc::sound::Wave sndKingDDDUpBJump;
	olc::sound::Wave sndKingDDDUpBLanding;
	olc::sound::Wave sndKingDDDVacuum;
	olc::sound::Wave sndKingDDDUpAir;
	olc::sound::Wave sndKingDDDAirAtkVoice;
	olc::sound::Wave sndKingDDDAirAtkHammer;
	olc::sound::Wave sndItemPicked;
	olc::sound::Wave sndRockyFall;
	olc::sound::Wave sndBoom;
	olc::sound::Wave sndEnterDoor;
	olc::sound::Wave sndMenuBip;
	olc::sound::Wave sndGateOpening;
	olc::sound::Wave sndGateOpened;
	olc::sound::Wave sndCreditMenu;

	// engine Properties
	cPlayer* player;
	float fStopTimebeforeDeadAnim = 0.0f;
	float fWindDirection = 1.0f;
	float fWindPower = 25.0f;
	float fWaitBeforeWinAnimation = 0.0f;
	float fWinTimer = 0.0f;
	bool bBreakLoop = false;
	bool bBossKilled = false;
	bool bInBossLvl = false;
	bool bWind = false;

	// Camera Properties
	cCamera* camera;

	// Title screen
	olc::Sprite	  sprTitleScreen;
	cTitleScreen* titleScreen;

	// World Map
	olc::Sprite sprWorldMap;
	olc::Sprite sprLock;
	cWorldMap*  worldMap;

	// Level Start Screen
	olc::Sprite  sprLevelStart;
	cLevelStart* levelStart;
	int levelStartAnim = 0;

	// End Screen
	olc::Sprite sprEndScreen;
	cEndScreen* endScreen;
	bool bPlayMusic = true;

	// Pause Menu
	olc::Sprite sprPauseMenu;
	olc::Sprite sprCursor;
	cPauseMenu* pauseMenu;

	// Select Menu
	olc::Sprite  sprSelectMenu;
	cSelectMenu* selectMenu;

	// Controls Menu
	olc::Sprite	   sprControlsMenu;
	olc::Sprite	   sprMenuBar;
	olc::Sprite	   sprUnselQuit;
	olc::Sprite	   sprSelQuit;
	olc::Sprite	   sprUnselDefault;
	olc::Sprite	   sprSelDefault;
	cControlsMenu* controlsMenu;
	std::map<std::string, olc::Key> savedControls;
	// --- Controller Sprites
	olc::Sprite sprX;
	olc::Sprite sprY;
	olc::Sprite sprB;
	olc::Sprite sprPause;
	olc::Sprite sprUp;
	olc::Sprite sprDown;
	olc::Sprite sprLeft;
	olc::Sprite sprRight;
	olc::Sprite sprA;
	olc::Sprite sprRBump;

	// Sound Menu
	olc::Sprite sprSoundMenuNoSound;
	olc::Sprite sprSoundMenuLow;
	olc::Sprite sprSoundMenuMid;
	olc::Sprite sprSoundMenuHigh;
	olc::Sprite sprSoundMenuMax;
	olc::Sprite sprSoundMenuRightArrow;
	olc::Sprite sprSoundMenuLeftArrow;
	cSoundMenu* soundMenu;

	// Screen mode menu
	olc::Sprite		 sprScreenModeMenu;
	cScreenModeMenu* screenModeMenu;

	// Credits Menu
	olc::Sprite	  sprCreditsMenu;
	cCreditsMenu* creditsMenu;

	// Player Sprites
	const int	nTileWidth = 64;
	const int	nTileHeight = 64;
	cAnimator	animPlayer;
	olc::Sprite sprPlayer;
	olc::Sprite sprSlap;
	olc::Decal* decPlayer;

	// Ennemies
	std::vector<cDynamicCreature*> vecEnnemies;
	float fHitStop = 0.0f;

	// Projectiles
	std::vector<cDynamicProjectile*> vecProjectiles;
	std::vector<cDynamicProjectile*> vecPreparedProjectiles;
	std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles;
	std::map<std::string, std::vector<olc::Decal*>>  mapDecProjectiles;

	// Moving Platforms
	std::vector<cDynamicMovingPlatform*> vecPlatforms;
	std::map<std::string, std::vector<olc::Sprite*>> mapPlatforms;
	std::map<std::string, std::vector<olc::Decal*>>  mapDecPlatforms;
	olc::Sprite sprDoorSwitchOff;
	olc::Sprite sprDoorSwitchOn;
	olc::Decal* decDoorSwitchOff;
	olc::Decal* decDoorSwitchOn;

	// Winds
	std::vector<cDynamicWind*> vecWinds;
	std::map<std::string, std::vector<olc::Sprite*>> mapWinds;
	std::map<std::string, std::vector<olc::Decal*>>  mapDecWinds;

	// Teleports
	std::vector<cDynamicTeleport*> vecTeleports;
	std::map<std::string, std::vector<olc::Sprite*>> mapTeleports;
	std::map<std::string, std::vector<olc::Decal*>>  mapDecTeleports;

	// HUD
	cHUD* HUD;
	olc::Sprite sprHealthBar;
	olc::Sprite sprHealthPoint;
	olc::Sprite sprBossHealthBar;
	olc::Decal* decHealthBar;
	olc::Decal* decHealthPoint;
	olc::Decal* decBossHealthBar;

	// SplashScreen
	float fSplashScreenTimer;
	float fSplashScreenTime = 5.0f;

	// Controller
	ControllerManager controller;

	// Transition
	float fTransitionTimer;
	float fTransitionTime = 0.25f;
	bool bPlayTransitionSound = false;
	bool bDrawGame = false;
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
		LS_SOUNDMENU,
		LS_SCREENMODEMENU,
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
		GS_SOUNDS,
		GS_SCREENMODE,
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
	bool GameState_Sounds(float fElapsedTime);
	bool GameState_ScreenMode(float fElapsedTime);
	bool GameState_Credits(float fElapsedTime);
	bool GameState_Close(float fElapsedTime);
	bool GameState_Transition(float fElapsedTime);

	void DestroyAllDynamics();
};

#endif // !DEF_ENGINE
