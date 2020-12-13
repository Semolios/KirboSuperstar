#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

#include "Platformer_Animator.h"
#include "Platformer_Assets.h"
#include "Platformer_ControlsMenu.h"
#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicCreatureBladeKnight.h"
#include "Platformer_DynamicCreatureBomber.h"
#include "Platformer_DynamicCreatureBrontoBurt.h"
#include "Platformer_DynamicCreatureScarfy.h"
#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_DynamicProjectile.h"
#include "Platformer_EndScreen.h"
#include "Platformer_Level.h"
#include "Platformer_PauseMenu.h"
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
	const float cfPlayerVelY = 5.0f;					// Up and Down velocities
	const float cfMinPlayerVelX = 1.0f;					// Minimum player horizontal velocity
	const float cfMaxPlayerVelX = 10.0f;				// Maximum player horizontal velocity
	const float cfMaxPlayerVelY = 100.0f;				// Maximum player vertical velocity
	const float cfMaxPlayerFlyingVelX = 5.0f;			// Maximum player horizontal velocity while flying
	const float cfMaxPlayerFlyingVelY = 5.0f;			// Maximum player vertical velocity while flying
	const float cfPlayerAccGrdX = 25.0f;				// Acceleration value when on ground
	const float cfPlayerAccAirX = 15.0f;				// Acceleration value when in the air
	const float cfPlayerJumpMaxAcc = -9.5f;				// Jump acceleration
	const float cfPlayerJumpMinAcc = -7.0f;				// Min Jump acceleratin
	const float cfJumpIncrement = -0.25f;				// Jump increment when holding space
	const float cfPlayerDblJumpMaxAcc = -8.0f;			// Double jump acceleration
	const float cfGravity = 20.0f;						// Gravity
	const float cfDrag = -3.0f;							// Drag
	const float cfMaxHealth = 10.0f;					// Max Kirbo health
	const float cfDamageEjectionVelX = 4.0f;			// horizontal velocity when hit by ennemy
	const float cfDamageEjectionVelY = 4.0f;			// vertical velocity when hit by ennemy
	const float cfCameraLowerPos = 1.0f / 4.0f;			// Lower position for the camera (when the player is not pushing down)
	const float cfCameraUpperPos = 2.0f / 3.0f;			// Upper position for the camera (when the player holds down when on ground)
	const float cfInvulnerabilityFrame = 4.0f;			// Invulnerability frame after a hit
	const float cfInvulnerabilityTickingSpeed = 0.2f;	// Ticking Speed when invulnerable
	const float cfStopTimebeforeDeadAnim = 1.5f;		// Stop time before death animation
	const float cfDeadAnimation = 4.0f;					// Duration of the dead animation
	const float cfTimeBeforeWinAnimation = 2.0f;		// Time before animation begins when kirby kills the boss
	const float cfBossKilledAnimation = 4.0f;			// Boss killed animation duration
	const float cfDeadRotationAnimation = 15.0f;		// rotation speed of dead animation
	const float cfGoAwayRotationAnimation = 15.0f;		// rotation speed of Kirbo goes away animation
	const float cfGoAwayTranslationAnimation = 256.0f;	// speed of kirbo in go away animation
	const float cfslapSpawnT = 1.0f;					// Number of frames to spawn slap attack
	const float cfJesusCrossSpawnT = 7.0f;				// Number of frames to spawn jesus cross
	const float cfVacuumAnimT = 2.0f;					// Number of frames when vacuum animation change to begin vacuum to vacuum
	const float cfGrdPlayerOverlay = 0.08f;				// little gap to give impression the player cross the grass and don't walk on the grass
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
	int nLevelWidth;
	int nLevelHeight;
	cLevel* currentLvl;
	int nCurrentLevel = 0;
	int nUnlockedLevel = 1;
	olc::Sprite* sprBackground;
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* sprGrdTiles = nullptr;
	std::vector<std::string> levels;
	std::vector<std::string> levelsEnnemies;
	std::vector<std::string> levelsTiles;
	std::vector<std::string> groundTiles;
	std::vector<std::string> levelsBackgrounds;
	std::vector<std::string> bossLevels;
	std::vector<std::string> bossLevelsBackgrounds;

	// Player Properties
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	float fAnimationTimer = 0.0f;
	float fInvulnerabilityTimer = 0.0f;
	float fFaceDir = 1.0f;
	float fHealth = 10.0f;
	float fCameraLookingDown = 2.0f / 3.0f;
	float fInvulnerabilityTickingTimer = 0.0f;
	float fStopTimebeforeDeadAnim = 0.0f;
	float fDeadAnimation = 0.0f;
	float fWaitBeforeWinAnimation = 0.0f;
	float fWinTimer = 0.0f;
	float fKirboGoesAwayTimer = 0.0f;
	float fWindDirection = 1.0f;
	float fWindPower = 25.0f;
	bool bPlayerOnGround = false;
	bool bDoubleJump = false;
	bool bChargeJump = false;
	bool bChargeDoubleJump = false;
	bool bAttacking = false;
	bool bSlapping = false;
	bool bVacuuming = false;
	bool bLaunchingJesusCross = false;
	bool bCanSpawnProjectile = false;
	bool bPlayerDamaged = false;
	bool bIsPlayerAttackable = true;
	bool bFlying = false;
	bool bShowKirby = true;
	bool bDead = false;
	bool bSwallowing = false;
	bool bInBossLvl = false;
	bool bBossKilled = false;
	bool bWind = false;

	// Camera Properties
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	float fShakeTimerChange = 0.0f;
	float fShakeEffectX;
	float fShakeEffectY;
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
	void StopAnyAttack();
	void CheckIfPlayerIsDamaged(cDynamic* object, float angle, float fOffsetX, float fOffsetY);
	bool ShapeOverlap_DIAG(polygon& r1, polygon& r2);
	void Attack(cDynamicCreature* victim, int damage);
	void ChangeEnnemyProperties(cDynamicCreature* victim, bool vaccumedState);

public:
	bool IsSolidTile(wchar_t tile);
	bool IsSemiSolidTile(wchar_t tile);
	cDynamicProjectile* CreateProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, float spriteWidth, float spriteHeight, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit = true);
	void AddProjectile(cDynamicProjectile* proj);
	float GetTileWidth();
	float GetTileHeight();
	void ResetVariables();
	bool CanInteract();
	void ActivateShakeEffect(bool activate);
	void CameraShakeEffect(float fElapsedTime);
	void WindEffect(float direction, float windPower, bool activate);
	void DrawGroundTile(int x, int y, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY, olc::Sprite* spriteTiles, wchar_t tile);
};