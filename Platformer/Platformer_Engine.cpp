#include "Platformer_Engine.h"

OneLoneCoder_Platformer::OneLoneCoder_Platformer()
{
	sAppName = "Tile Based Platform Game";
}

bool OneLoneCoder_Platformer::OnUserCreate()
{
	spriteTiles = new olc::Sprite("assets/gfx/tilemap.png");
	animPlayer.ChangeState("idle");

	return true;
}

bool OneLoneCoder_Platformer::OnUserUpdate(float fElapsedTime)
{
	switch (nGameState)
	{
		case GS_LOADING: GameState_Loading(fElapsedTime); break;
		case GS_TITLE: GameState_Title(fElapsedTime); break;
		case GS_MAIN: GameState_Main(fElapsedTime); break;
	}

	return true;
}

bool OneLoneCoder_Platformer::GameState_Loading(float fElapsedTime)
{
	fFaceDir = 1.0f;

	nLevelWidth = 64;
	nLevelHeight = 16;
	sLevel += L"................................................................";
	sLevel += L"................................................................";
	sLevel += L".......o.o.o....................................................";
	sLevel += L"........o.o.....................................................";
	sLevel += L".......................########.................................";
	sLevel += L".....BB?BBBB?BB.......###..............#.#......................";
	sLevel += L"....................###................#.#......................";
	sLevel += L"...................####.........................................";
	sLevel += L"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG.##############.....########";
	sLevel += L"...................................#.#...............###........";
	sLevel += L"........................############.#............###...........";
	sLevel += L"........................#............#.........###..............";
	sLevel += L"........................#.############......###.................";
	sLevel += L"........................#................###....................";
	sLevel += L"........................#################.......................";
	sLevel += L"................................................................";

	// Player sprites
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo00.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo01.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo02.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo03.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo04.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo05.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo04.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo03.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo02.png"));
	animPlayer.mapStates["idle"].push_back(new olc::Sprite("assets/gfx/kirbo01.png"));

	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("TODO"));

	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning00.png"));
	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning01.png"));
	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning02.png"));
	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning03.png"));

	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("TODO"));

	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump.png"));

	animPlayer.mapStates["fall"].push_back(new olc::Sprite("assets/gfx/kirboFall.png"));

	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));

	// title screen
	sprTitleScreen = new olc::Sprite("assets/gfx/title screen.png");

	nGameState = GS_TITLE;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Title(float fElapsedTime)
{
	DrawSprite(0, 0, sprTitleScreen);

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_MAIN;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Main(float fElapsedTime)
{
	animPlayer.Update(fElapsedTime);

	// Utility Lambdas
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
			return sLevel[y * nLevelWidth + x];
		else
			return L' ';
	};

	auto SetTile = [&](int x, int y, wchar_t c)
	{
		if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
			sLevel[y * nLevelWidth + x] = c;
	};

	// Handle input
	if (IsFocused())
	{
		if (GetKey(olc::Key::UP).bHeld)
		{
			fPlayerVelY = -6.0f;
		}

		if (GetKey(olc::Key::DOWN).bHeld)
		{
			fPlayerVelY = 6.0f;
		}

		if (GetKey(olc::Key::LEFT).bHeld)
		{
			fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
			fFaceDir = -1.0f;
		}

		if (GetKey(olc::Key::RIGHT).bHeld)
		{
			fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
			fFaceDir = 1.0f;
		}

		if (GetKey(olc::Key::SPACE).bPressed)
		{
			if (fPlayerVelY == 0)
			{
				fPlayerVelY = -12.0f;
			}
		}
	}

	// Gravity
	fPlayerVelY += 20.0f * fElapsedTime;

	if (bPlayerOnGround)
	{
		fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
		if (fabs(fPlayerVelX) < 0.15f)
		{
			fPlayerVelX = 0.0f;
			animPlayer.ChangeState("idle");
		}
		else
		{
			animPlayer.ChangeState("run");
		}
	}
	else
	{
		if (fPlayerVelY < 0)
			animPlayer.ChangeState("jump");
		else
			animPlayer.ChangeState("fall");
	}

	// Clamp velocities
	if (fPlayerVelX > 10.0f)
		fPlayerVelX = 10.0f;

	if (fPlayerVelX < -10.0f)
		fPlayerVelX = -10.0f;

	if (fPlayerVelY > 100.0f)
		fPlayerVelY = 100.0f;

	if (fPlayerVelY < -100.0f)
		fPlayerVelY = -100.0f;

	float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
	float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

	// Check for pickups !
	if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'o')
		SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');

	if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == L'o')
		SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');

	if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'o')
		SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');

	if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == L'o')
		SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.');

	// Collision
	if (fPlayerVelX <= 0) // Moving Left
	{
		if (GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'.')
		{
			fNewPlayerPosX = (int)fNewPlayerPosX + 1;
			fPlayerVelX = 0;
		}
	}
	else // Moving Right
	{
		if (GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'.')
		{
			fNewPlayerPosX = (int)fNewPlayerPosX;
			fPlayerVelX = 0;
		}
	}

	bPlayerOnGround = false;
	if (fPlayerVelY <= 0) // Moving Up
	{
		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'.')
		{
			fNewPlayerPosY = (int)fNewPlayerPosY + 1;
			fPlayerVelY = 0;
		}
	}
	else // Moving Down
	{
		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'.')
		{
			fNewPlayerPosY = (int)fNewPlayerPosY;
			fPlayerVelY = 0;
			bPlayerOnGround = true;
		}
	}

	fPlayerPosX = fNewPlayerPosX;
	fPlayerPosY = fNewPlayerPosY;

	fCameraPosX = fPlayerPosX;
	fCameraPosY = fPlayerPosY;

	// Draw level
	int nVisibleTilesX = ScreenWidth() / nTileWidth;
	int nVisibleTilesY = ScreenHeight() / nTileHeight;

	// Calculate Top-Left most visible tile
	float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
	float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;

	// Clamp camera to game boundaries
	if (fOffsetX < 0) fOffsetX = 0;
	if (fOffsetY < 0) fOffsetY = 0;
	if (fOffsetX > nLevelWidth - nVisibleTilesX) fOffsetX = nLevelWidth - nVisibleTilesX;
	if (fOffsetY > nLevelHeight - nVisibleTilesY) fOffsetY = nLevelHeight - nVisibleTilesY;

	// Get offsets for smooth movement
	float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
	float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

	Clear(olc::CYAN);

	// Draw Visible tile map
	for (int x = -2; x < nVisibleTilesX + 2; x++)
	{
		for (int y = -2; y < nVisibleTilesY + 2; y++)
		{
			wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);
			switch (sTileID)
			{
				case L'#': // Solid Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'G': // Ground Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'B': // Brick Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'?': // Question Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'o': // Coin
					SetPixelMode(olc::Pixel::ALPHA);
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::CYAN);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					SetPixelMode(olc::Pixel::NORMAL);
					break;
			}
		}
	}

	// Draw Player
	olc::GFX2D::Transform2D t;
	t.Translate(-32.0f, -32.0f);
	t.Scale(fFaceDir * 1.0f, 1.0f);

	t.Translate((fPlayerPosX - fOffsetX) * nTileWidth + 32, (fPlayerPosY - fOffsetY) * nTileHeight + 32);

	SetPixelMode(olc::Pixel::ALPHA);
	animPlayer.DrawSelf(this, t);
	SetPixelMode(olc::Pixel::NORMAL);

	return true;
}
