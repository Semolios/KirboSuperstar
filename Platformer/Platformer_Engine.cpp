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
		case GS_TRANSITION: GameState_Transition(fElapsedTime); break;
		case GS_LOADLEVEL: GameState_LoadLevel(fElapsedTime); break;
	}

	return true;
}

bool OneLoneCoder_Platformer::GameState_Transition(float fElapsedTime)
{
	transition->Update(this, fElapsedTime);

	switch (transitionAnim)
	{
		case 0: animPlayer.ChangeState("idle"); break;
		case 1: animPlayer.ChangeState("run"); break;
		case 2: animPlayer.ChangeState("jump"); break;
	}

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_MAIN;

	return false;
}

bool OneLoneCoder_Platformer::GameState_Loading(float fElapsedTime)
{
	fFaceDir = 1.0f;

	levels.push_back("assets/lvls/lvl1.txt");
	levels.push_back("assets/lvls/lvl2.txt");

	currentLvl = new cLevel();

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

	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump00.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump01.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump02.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump03.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump04.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump05.png"));

	animPlayer.mapStates["fall"].push_back(new olc::Sprite("assets/gfx/kirboFall.png"));

	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("TODO"));

	// title screen
	sprTitleScreen = new olc::Sprite("assets/gfx/title screen.png");
	titleScreen = new cTitleScreen(this, sprTitleScreen);

	// Transition
	cTransition::animPlayer = &animPlayer;

	nGameState = GS_TITLE;

	return true;
}

bool OneLoneCoder_Platformer::GameState_LoadLevel(float fElapsedTime)
{
	if (currentLvl->LoadLevel(levels[nCurrentLevel]))
	{
		nLevelWidth = currentLvl->GetWidth();
		nLevelHeight = currentLvl->GetHeight();
		sLevel = currentLvl->GetLevel();
	}

	srand(time(NULL));
	transitionAnim = rand() % 3;

	nGameState = GS_TRANSITION;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Title(float fElapsedTime)
{
	titleScreen->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_LOADLEVEL;

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
			fPlayerVelY = -cfPlayerVelY;
		}

		if (GetKey(olc::Key::DOWN).bHeld)
		{
			fPlayerVelY = cfPlayerVelY;
		}

		if (GetKey(olc::Key::LEFT).bHeld)
		{
			if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX -= (cfMinPlayerVelX + 0.05f);

			fPlayerVelX += (bPlayerOnGround ? -cfPlayerAccGrdX : -cfPlayerAccAirX) * fElapsedTime;
			fFaceDir = -1.0f;
		}

		if (GetKey(olc::Key::RIGHT).bHeld)
		{
			if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX += (cfMinPlayerVelX + 0.05f);

			fPlayerVelX += (bPlayerOnGround ? cfPlayerAccGrdX : cfPlayerAccAirX) * fElapsedTime;
			fFaceDir = 1.0f;
		}

		if (GetKey(olc::Key::SPACE).bPressed)
		{
			if (bPlayerOnGround)
			{
				fPlayerVelY = -cfPlayerJumpAcc;
			}
		}
	}

	// Gravity
	fPlayerVelY += cfGravity * fElapsedTime;

	if (bPlayerOnGround)
	{
		fPlayerVelX += cfDrag * fPlayerVelX * fElapsedTime;
		if (fabs(fPlayerVelX) < cfMinPlayerVelX)
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
	if (fPlayerVelX > cfMaxPlayerVelX)
		fPlayerVelX = cfMaxPlayerVelX;

	if (fPlayerVelX < -cfMaxPlayerVelX)
		fPlayerVelX = -cfMaxPlayerVelX;

	if (fPlayerVelY > cfMaxPlayerVelY)
		fPlayerVelY = cfMaxPlayerVelY;

	if (fPlayerVelY < -cfMaxPlayerVelY)
		fPlayerVelY = -cfMaxPlayerVelY;

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
				case L'w':
					// Ce n'est pas ici qu'il faut le mettre, sinon d�s qu'il apparait � l'�cran on change de niveau, c'est pas ce qu'on veut
					nCurrentLevel++;
					nGameState = GS_LOADLEVEL;
					fPlayerPosX = 1;
					fPlayerPosY = 7;
					break;
			}
		}
	}

	// Draw Player
	olc::GFX2D::Transform2D t;
	t.Translate((float)-nTileWidth / 2.0f, (float)-nTileWidth / 2.0f);
	t.Scale(fFaceDir * 1.0f, 1.0f);

	t.Translate((fPlayerPosX - fOffsetX) * nTileWidth + (nTileWidth / 2), (fPlayerPosY - fOffsetY) * nTileHeight + (nTileWidth / 2));

	SetPixelMode(olc::Pixel::ALPHA);
	animPlayer.DrawSelf(this, t);
	SetPixelMode(olc::Pixel::NORMAL);

	return true;
}
