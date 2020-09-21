#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Animator.h"

class OneLoneCoder_Platformer : public olc::PixelGameEngine
{
public:
	OneLoneCoder_Platformer();

private:
	// Level storage
	std::wstring sLevel;
	int nLevelWidth;
	int nLevelHeight;

	// Player Properties
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	bool bPlayerOnGround = false;

	// Camera Properties
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	// Title screen
	olc::Sprite* sprTitleScreen;

	// Sprites
	int nTileWidth = 64;
	int nTileHeight = 64;
	olc::Sprite* spriteTiles = nullptr;
	cAnimator animPlayer;

	// Face direction
	float fFaceDir = 1.0f;

	enum
	{
		GS_LOADING,
		GS_TITLE,
		GS_MAIN,
	} nGameState = GS_LOADING;

protected:

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	bool GameState_Loading(float fElapsedTime);
	bool GameState_Title(float fElapsedTime);
	bool GameState_Main(float fElapsedTime);
};