#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

#include "Platformer_Animator.h"
#include "Platformer_Assets.h"
#include "Platformer_ControlsMenu.h"
#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicProjectile.h"
#include "Platformer_EndScreen.h"
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
	const float cfCameraLowerPos = 1.0f / 4.0f;			// Lower position for the camera (when the player is not pushing down)
	const float cfCameraUpperPos = 2.0f / 3.0f;			// Upper position for the camera (when the player holds down when on ground)
	const float cfStopTimebeforeDeadAnim = 1.5f;		// Stop time before death animation
	const float cfTimeBeforeWinAnimation = 2.0f;		// Time before animation begins when kirbo kills the boss
	const float cfBossKilledAnimation = 4.0f;			// Boss killed animation duration
	const float cfGrdDynamicOverlay = 0.08f;			// little gap to give impression the player lightly overlap the platform
	const float cfKnockBackDuration = 0.3f;				// Knock back duration
	const int cnSpriteOffsetX = 64;						// Sprite offset X because kirbo sprites are 192 * 192 pixels
	const int cnSpriteOffsetY = 64;						// Sprite offset Y because kirbo sprites are 192 * 192 pixels

	struct vec2d
	{
		float x;
		float y;
	};

	struct polygon
	{
		std::vector<vec2d> p;	// Transformed Points
		vec2d pos;				// Position of shape
		float angle;			// Direction of shape
		std::vector<vec2d> o;	// "Model" of shape							
		bool overlap = false;	// Flag to indicate if overlap has occurred
	};

	// Level storage
	std::wstring sLevel;
	cLevel* level;
	int nCurrentLevel = 0;
	int nUnlockedLevel = 6; // TODO PENSER A REMETTRE A 1 A LA FIN
	olc::Sprite* sprBackground;
	olc::Sprite* spriteTiles;
	olc::Sprite* sprGrdTiles;
	olc::Sprite* sprDoor;
	std::vector<std::string> levels;
	std::vector<std::string> levelsEnnemies;
	std::vector<std::string> levelsTiles;
	std::vector<std::string> groundTiles;
	std::vector<std::string> levelsBackgrounds;
	std::vector<std::string> bossLevels;
	std::vector<std::string> bossLevelsBackgrounds;

	// engine Properties
	bool bBreakLoop = false;
	float fStopTimebeforeDeadAnim = 0.0f;
	float fWindDirection = 1.0f;
	float fWindPower = 25.0f;
	bool bBossKilled = false;
	bool bInBossLvl = false;
	bool bWind = false;
	float fWaitBeforeWinAnimation = 0.0f;
	float fWinTimer = 0.0f;

	// Player Properties
	cPlayer* player;
	//float fPlayerPosX = 0.0f;
	//float fPlayerPosY = 0.0f;
	//float fPlayerVelX = 0.0f;
	//float fPlayerVelY = 0.0f;
	//float fAnimationTimer = 0.0f;
	//float fFaceDir = 1.0f;
	//float fHealth = 10.0f;
	//float fInvulnerabilityTickingTimer = 0.0f;
	//float fDeadAnimation = 0.0f;
	//float fKirboGoesAwayTimer = 0.0f;
	//float fPlayerCollisionUpperLimit = 0.9f;
	//float fPlayerCollisionLowerLimit = 0.1f;
	//float fSwallowDistance = 0.1f;
	//bool bPlayerOnGround = false;
	//bool bDoubleJump = false;
	//bool bChargeJump = false;
	//bool bChargeDoubleJump = false;
	//bool bAttacking = false;
	//bool bSlapping = false;
	//bool bVacuuming = false;
	//bool bLaunchingJesusCross = false;
	//bool bCanSpawnProjectile = false;
	//bool bPlayerDamaged = false;
	//bool bIsPlayerAttackable = true;
	//bool bFlying = false;
	//bool bDead = false;
	//bool bSwallowing = false;

	// Camera Properties
	float fCameraLookingDown = 2.0f / 3.0f;
	const float cfCameraMoveSpeed = 0.02f;
	const float cfShakeAttenuation = 1000.0f;
	const float cfShakeEffectChangeFrequency = 0.07f;
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	float fShakeTimerChange = 0.0f;
	float fShakeEffectX;
	float fShakeEffectY;
	int nShakeAmplitudeX;
	int nShakeAmplitudeY;
	bool bShake = false;

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

	void LoadLevelProperties();
	void CheckIfPlayerIsDamaged(cDynamic* object, float angle, float fOffsetX, float fOffsetY);
	bool ShapeOverlap_DIAG(polygon& r1, polygon& r2);
	void Attack(cDynamicCreature* victim, int damage);
	void ChangeEnnemyProperties(cDynamicCreature* victim, bool vaccumedState);

public:
	bool IsSolidTile(wchar_t tile);
	bool IsSemiSolidTile(wchar_t tile);
	cDynamicProjectile* CreateProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit = true, int corner = 0);
	void AddProjectile(cDynamicProjectile* proj);
	float GetTileWidth();
	float GetTileHeight();
	float GetGravityValue();
	float GetGroundDynamicOverlay();
	wchar_t GetTile(int x, int y);
	void SetTile(int x, int y, wchar_t c);
	bool IsInBossLevel();
	void SetbInBossLevel(bool inBossLevel);
	bool IsBossKilled();
	void SetbBossKilled(bool bossKilled);
	void SetGameState(std::string gameState);
	void LowerCameraPosition();
	void RaiseCameraPosition();
	float GetDragValue();
	void SetPlayerChoice(int choice);
	float GetGrdDynamicOverlay();
	void UpdateWinTimer(float fElapsedTime);
	float GetWinTimer();
	void ResetVariables();
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
	void WindEffect(float direction, float windPower, bool activate);
	void DrawGroundTile(int x, int y, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY, olc::Sprite* spriteTiles, wchar_t tile);
	void BreakLoop();
};