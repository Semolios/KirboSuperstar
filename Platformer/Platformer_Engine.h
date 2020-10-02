#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

#include "Platformer_Animator.h"
#include "Platformer_Assets.h"
#include "Platformer_Dynamic.h"
#include "Platformer_DynamicCreature.h"
#include "Platformer_DynamicCreatureWaddleDee.h"
#include "Platformer_EndScreen.h"
#include "Platformer_Level.h"
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
	const float cfPlayerVelY = 6.0f;		// Up and Down velocities
	const float cfMinPlayerVelX = 1.0f;		// Minimum player horizontal velocity
	const float cfMaxPlayerVelX = 10.0f;	// Maximum player horizontal velocity
	const float cfMaxPlayerVelY = 100.0f;	// Maximum player vertical velocity
	const float cfPlayerAccGrdX = 25.0f;	// Acceleration value when on ground
	const float cfPlayerAccAirX = 15.0f;	// Acceleration value when in the air
	const float cfPlayerJumpAcc = 12.0f;	// Jump acceleration
	const float cfPlayerDblJumpAcc = 10.0f;	// Double jump acceleration
	const float cfGravity = 20.0f;			// Gravity
	const float cfDrag = -3.0f;				// Drag

	// Level storage
	std::wstring sLevel;
	int nLevelWidth;
	int nLevelHeight;

	cLevel* currentLvl;
	int nCurrentLevel = 0;
	int nUnlockedLevel = 1;
	std::vector<std::string> levels;

	// Player Properties
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	float fAnimationTimer = 0.0f;
	bool bPlayerOnGround = false;
	bool bDoubleJump = false;
	bool bAttacking = false;

	// Camera Properties
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	// Title screen
	olc::Sprite* sprTitleScreen;
	cTitleScreen* titleScreen;

	// World Map
	olc::Sprite* sprWorldMap;
	cWorldMap* worldMap;

	// Transition Screen
	cTransition* transition;
	int transitionAnim = 0;

	// End Screen
	olc::Sprite* sprEndScreen;
	cEndScreen* endScreen;

	// Sprites
	const int nTileWidth = 64;
	const int nTileHeight = 64;
	olc::Sprite* spriteTiles = nullptr;
	cAnimator animPlayer;

	// Ennemies
	std::vector<cDynamic*> vecDynamics;

	// Face direction
	float fFaceDir = 1.0f;

	bool bOnPause = false;

	enum
	{
		GS_LOADING,
		GS_TITLE,
		GS_MAIN,
		GS_TRANSITION,
		GS_LOADLEVEL,
		GS_WORLDMAP,
		GS_ENDSCREEN,
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

	bool IsSolidTile(wchar_t tile);
};