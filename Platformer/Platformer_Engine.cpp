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
		case GS_WORLDMAP: GameState_WorldMap(fElapsedTime); break;
		case GS_ENDSCREEN: GameState_EndScreen(fElapsedTime); break;
		case GS_PAUSE: GameState_PauseMenu(fElapsedTime); break;
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
		case 3: animPlayer.ChangeState("riding_star"); break;
	}

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_MAIN;

	return false;
}

bool OneLoneCoder_Platformer::GameState_Loading(float fElapsedTime)
{
	// Assets
	cAssets::get().LoadSprites();
	cAssets::get().LoadItems();

	fFaceDir = 1.0f;

	levels.push_back("assets/lvls/lvl1.txt");
	levels.push_back("assets/lvls/lvl2.txt");
	levels.push_back("assets/lvls/lvl3.txt");
	levels.push_back("assets/lvls/lvl4.txt");
	levels.push_back("assets/lvls/lvl5.txt");
	levels.push_back("assets/lvls/lvl6.txt");

	levelsEnnemies.push_back("assets/lvls/ennemiesLvl1.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl2.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl3.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl4.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl5.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl6.txt");

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

	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star00.png"));
	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star01.png"));
	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star02.png"));
	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star03.png"));
	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star04.png"));
	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star05.png"));
	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star06.png"));
	animPlayer.mapStates["riding_star"].push_back(new olc::Sprite("assets/gfx/riding_star07.png"));

	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning00.png"));
	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning01.png"));
	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning02.png"));
	animPlayer.mapStates["run"].push_back(new olc::Sprite("assets/gfx/kirboRunning03.png"));

	animPlayer.mapStates["slap"].push_back(new olc::Sprite("assets/gfx/slap00.png"));
	animPlayer.mapStates["slap"].push_back(new olc::Sprite("assets/gfx/slap01.png"));
	animPlayer.mapStates["slap"].push_back(new olc::Sprite("assets/gfx/slap02.png"));
	animPlayer.mapStates["slap"].push_back(new olc::Sprite("assets/gfx/slap03.png"));

	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump00.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump01.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump02.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump03.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump04.png"));
	animPlayer.mapStates["jump"].push_back(new olc::Sprite("assets/gfx/kirboJump05.png"));

	animPlayer.mapStates["fall"].push_back(new olc::Sprite("assets/gfx/kirboFall.png"));

	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged00.png"));
	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged01.png"));
	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged02.png"));
	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged03.png"));
	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged04.png"));
	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged05.png"));
	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged06.png"));
	animPlayer.mapStates["damaged"].push_back(new olc::Sprite("assets/gfx/kirboDamaged07.png"));

	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying00.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying01.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying02.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying03.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying02.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying01.png"));

	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("TODO"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("TODO"));

	// title screen
	sprTitleScreen = new olc::Sprite("assets/gfx/title screen.png");
	titleScreen = new cTitleScreen(this, sprTitleScreen);

	// World Map
	sprWorldMap = new olc::Sprite("assets/gfx/WorldMap.png");
	worldMap = new cWorldMap(this, sprWorldMap);
	cWorldMap::animPlayer = &animPlayer;
	worldMap->SetUnlockedLevel(nUnlockedLevel);

	// Transition
	cTransition::animPlayer = &animPlayer;

	// End Screen
	sprEndScreen = new olc::Sprite("assets/gfx/endScreen.png");
	endScreen = new cEndScreen(this, sprEndScreen);

	// Pause Menu
	sprPauseMenu = new olc::Sprite("assets/gfx/PauseMenu.png");
	pauseMenu = new cPauseMenu(this, sprPauseMenu);

	// HUD
	sprHealthBar = new olc::Sprite("assets/gfx/emptyHealthBar.png");
	sprHealthPoint = new olc::Sprite("assets/gfx/healthPoint.png");

	nGameState = GS_TITLE;

	return true;
}

bool OneLoneCoder_Platformer::GameState_LoadLevel(float fElapsedTime)
{
	// Destroy all dynamics
	vecEnnemies.clear();

	nCurrentLevel = worldMap->GetSelectedLevel();
	if (currentLvl->LoadLevel(levels[nCurrentLevel]))
	{
		nLevelWidth = currentLvl->GetWidth();
		nLevelHeight = currentLvl->GetHeight();
		fPlayerPosX = currentLvl->GetInitPlayerPosX();
		fPlayerPosY = currentLvl->GetInitPlayerPoxY();
		sLevel = currentLvl->GetLevel();

		currentLvl->PopulateEnnemies(vecEnnemies, levelsEnnemies[nCurrentLevel]);
	}

	// Reset variables when level is loading
	fPlayerVelX = 0.0f;
	fPlayerVelY = 0.0f;
	fHealth = cfMaxHealth;
	fInvulnerabilityTimer = 0.0f;

	srand(time(NULL));
	transitionAnim = rand() % 4;

	nGameState = GS_TRANSITION;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Title(float fElapsedTime)
{
	titleScreen->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
	{
		animPlayer.ChangeState("riding_star");
		nGameState = GS_WORLDMAP;
	}

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
	if (IsFocused() && !bPlayerDamaged)
	{
		// Fly, enter a door
		if (GetKey(olc::Key::UP).bHeld)
		{
			if (GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f) == L'w' && bPlayerOnGround)
			{
				if (nCurrentLevel + 1 == levels.size())
				{
					// If Player finishes the last level, the game is over
					nGameState = GS_ENDSCREEN;

					return true;
				}
				else if (nCurrentLevel + 1 == nUnlockedLevel)
				{
					// If Player finishes the last level unlocked, another one is unlocked
					nUnlockedLevel++;
					worldMap->SetUnlockedLevel(nUnlockedLevel);
				}
				animPlayer.ChangeState("riding_star");
				nGameState = GS_WORLDMAP;

				return true;
			}
			fPlayerVelY = -cfPlayerVelY;
			bFlying = true;
			animPlayer.ChangeState("flying");
		}

		// Go down when flying
		if (GetKey(olc::Key::DOWN).bHeld)
		{
			if (bFlying)
				fPlayerVelY = cfPlayerVelY;
		}

		// Go left
		if (GetKey(olc::Key::LEFT).bHeld)
		{
			// Init speed by cfDrag + 0.05 or the player won't move when on ground
			if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX -= (cfMinPlayerVelX + 0.05f);

			fPlayerVelX += (bPlayerOnGround ? -cfPlayerAccGrdX : -cfPlayerAccAirX) * fElapsedTime;
			fFaceDir = -1.0f;
		}

		// Go right
		if (GetKey(olc::Key::RIGHT).bHeld)
		{
			// Init speed by cfDrag + 0.05 or the player won't move when on ground
			if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX += (cfMinPlayerVelX + 0.05f);

			fPlayerVelX += (bPlayerOnGround ? cfPlayerAccGrdX : cfPlayerAccAirX) * fElapsedTime;
			fFaceDir = 1.0f;
		}

		// Jump, double jump, stop flying
		if (GetKey(olc::Key::SPACE).bPressed)
		{
			if (bFlying)
			{
				bFlying = false;
			}
			else if (bPlayerOnGround)
			{
				fPlayerVelY = -cfPlayerJumpAcc;
			}
			else if (bDoubleJump && fPlayerVelY > 0)
			{
				bDoubleJump = false;
				fPlayerVelY = -cfPlayerDblJumpAcc;
			}
		}

		// Slap attack
		if (GetKey(olc::Key::F).bPressed)
		{
			// Can't spam slap, can't slap when player is damaged or when he's flying
			if (!bAttacking && !bPlayerDamaged && !bFlying)
			{
				animPlayer.ChangeState("slap");
				bAttacking = true;
				fAnimationTimer = 0.0f;
			}
		}

		// Pause
		if (GetKey(olc::Key::P).bPressed)
		{
			// You can't use pause when you are hit.
			// like you can't pause when you jump in mario sunshine so you can't leave the level when you are falling
			nGameState = GS_PAUSE;
			return true;
		}
	}

	// Gravity
	fPlayerVelY += cfGravity * fElapsedTime;

	if (bAttacking)
	{
		fPlayerVelX = 0.0f;
		fPlayerVelY = 0.0f;
	}
	else
	{
		if (bPlayerOnGround)
		{
			bFlying = false;

			fPlayerVelX += cfDrag * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < cfMinPlayerVelX)
			{
				fPlayerVelX = 0.0f;

				if (!bPlayerDamaged)
					animPlayer.ChangeState("idle");
			}
			else
			{
				if (!bPlayerDamaged)
					animPlayer.ChangeState("run");
			}
		}
		else
		{
			if (!bPlayerDamaged)
			{
				if (!bFlying)
				{
					if (fPlayerVelY < 0)
						animPlayer.ChangeState("jump");
					else
						animPlayer.ChangeState("fall");
				}
			}
		}
	}

	// Draw Player
	olc::GFX2D::Transform2D t;
	t.Translate((float)-nTileWidth / 2.0f, (float)-nTileWidth / 2.0f);
	t.Scale(fFaceDir * 1.0f, 1.0f);

#pragma region ONE CYCLE ANIMATIONS

	if (bAttacking)
	{
		// calculate elapsed time during attack
		fAnimationTimer += fElapsedTime;

		// offset sprite so kirbo is centered
		t.Translate(0.0f, (float)-nTileWidth);

		// Hit frame
		if (fAnimationTimer >= 0.1f && fAnimationTimer <= 0.2f)
		{
			// Create an AOE where the player is facing and check for ennemies and AOE overlap
			polygon sAOE;
			sAOE.angle = 0.0f;

			if (fFaceDir == -1) // West
				sAOE.pos = { (fPlayerPosX - 0.5f) * nTileWidth , (fPlayerPosY + 0.5f) * nTileHeight };

			if (fFaceDir == 1) // East
				sAOE.pos = { (fPlayerPosX + 1.5f) * nTileWidth , (fPlayerPosY + 0.5f) * nTileHeight };

			sAOE.o.push_back({ -(float)nTileWidth / 2.0f, -3.0f * (float)nTileHeight / 2.0f });
			sAOE.o.push_back({ -(float)nTileWidth / 2.0f, +3.0f * (float)nTileHeight / 2.0f });
			sAOE.o.push_back({ +(float)nTileWidth / 2.0f, +3.0f * (float)nTileHeight / 2.0f });
			sAOE.o.push_back({ +(float)nTileWidth / 2.0f, -3.0f * (float)nTileHeight / 2.0f });
			sAOE.p.resize(4);

			for (int i = 0; i < sAOE.o.size(); i++)
			{
				sAOE.p[i] =
				{	// 2D Rotation Transform + 2D Translation (angle is always 0 here, no rotation allowed)
					(sAOE.o[i].x * cosf(sAOE.angle)) - (sAOE.o[i].y * sinf(sAOE.angle)) + sAOE.pos.x,
					(sAOE.o[i].x * sinf(sAOE.angle)) + (sAOE.o[i].y * cosf(sAOE.angle)) + sAOE.pos.y,
				};
			}

			// Check if an ennemy take the attack
			for (auto& dyn : vecEnnemies)
			{
				polygon sEnnemy;
				sEnnemy.pos = { (float)dyn->px * nTileWidth + (float)nTileWidth / 2.0f, (float)dyn->py * nTileHeight + (float)nTileHeight / 2.0f }; // Center of the ennemy
				sEnnemy.angle = 0.0f;
				sEnnemy.o.push_back({ -(float)nTileWidth / 2.0f, -(float)nTileHeight / 2.0f });
				sEnnemy.o.push_back({ -(float)nTileWidth / 2.0f, +(float)nTileHeight / 2.0f });
				sEnnemy.o.push_back({ +(float)nTileWidth / 2.0f, +(float)nTileHeight / 2.0f });
				sEnnemy.o.push_back({ +(float)nTileWidth / 2.0f, -(float)nTileHeight / 2.0f });
				sEnnemy.p.resize(4);

				for (int i = 0; i < sEnnemy.o.size(); i++)
				{
					sEnnemy.p[i] =
					{	// 2D Rotation Transform + 2D Translation (angle is always 0 here, no rotation allowed)
						(sEnnemy.o[i].x * cosf(sEnnemy.angle)) - (sEnnemy.o[i].y * sinf(sEnnemy.angle)) + sEnnemy.pos.x,
						(sEnnemy.o[i].x * sinf(sEnnemy.angle)) + (sEnnemy.o[i].y * cosf(sEnnemy.angle)) + sEnnemy.pos.y,
					};
				}

				if (ShapeOverlap_DIAG(sAOE, sEnnemy))
				{
					if (dyn->bIsAttackable)
						SlapAttack((cDynamicCreature*)dyn);
				}
			}
		}

		if (fAnimationTimer >= animPlayer.mapStates["slap"].size() * animPlayer.fTimeBetweenFrames)
		{
			fAnimationTimer = 0.0f;
			bAttacking = false;
		}
	}

	if (bPlayerDamaged)
	{
		// calculate elapsed time after damage
		fAnimationTimer += fElapsedTime;
		bFlying = false;

		if (fAnimationTimer >= animPlayer.mapStates["damaged"].size() * animPlayer.fTimeBetweenFrames)
		{
			fAnimationTimer = 0.0f;
			bPlayerDamaged = false;

			// If Player health = 0 return to the map
			if (fHealth <= 0)
			{
				nGameState = GS_WORLDMAP;
				animPlayer.ChangeState("riding_star");
				fInvulnerabilityTimer = 0.0f;
				return true;
			}
		}
	}

#pragma endregion

	// Clamp velocities
	if (fPlayerVelX > cfMaxPlayerVelX)
		fPlayerVelX = cfMaxPlayerVelX;

	if (fPlayerVelX < -cfMaxPlayerVelX)
		fPlayerVelX = -cfMaxPlayerVelX;

	if (fPlayerVelY > cfMaxPlayerVelY)
		fPlayerVelY = cfMaxPlayerVelY;

	if (fPlayerVelY < -cfMaxPlayerVelY)
		fPlayerVelY = -cfMaxPlayerVelY;

	if (bFlying)
	{
		if (fPlayerVelX > cfMaxPlayerFlyingVelX)
			fPlayerVelX = cfMaxPlayerFlyingVelX;

		if (fPlayerVelX < -cfMaxPlayerFlyingVelX)
			fPlayerVelX = -cfMaxPlayerFlyingVelX;

		if (fPlayerVelY > cfMaxPlayerFlyingVelY)
			fPlayerVelY = cfMaxPlayerFlyingVelY;
	}

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
		if (IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f)) || IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f)))
		{
			fNewPlayerPosX = (int)fNewPlayerPosX + 1;
			fPlayerVelX = 0;
		}
	}
	else // Moving Right
	{
		if (IsSolidTile(GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f)) || IsSolidTile(GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f)))
		{
			fNewPlayerPosX = (int)fNewPlayerPosX;
			fPlayerVelX = 0;
		}
	}

	bPlayerOnGround = false;
	if (fPlayerVelY <= 0) // Moving Up
	{
		if (IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY)) || IsSolidTile(GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY)))
		{
			fNewPlayerPosY = (int)fNewPlayerPosY + 1;
			fPlayerVelY = 0;
		}
	}
	else // Moving Down
	{
		// This little trick (fPlayerPosY + 1.0f < (float)((int)fNewPlayerPosY + 1.0f) + 0.1f) checks if the player's feets are above the top of the semi-solid Block.
		// Otherwise the player is moved to the top of the block when his feets reach the bottom of the block
		// "fPlayerPosY + 1.0f" is the feets Y position, "(float)((int)fNewPlayerPosY + 1.0f) + 0.1f" takes the top of the block at the feets position and add a 0.1 delta, if the feets are above this delta, the player is moved on top of the block.
		if (IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) || IsSolidTile(GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f)) ||
			((IsSemiSolidTile(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) || IsSemiSolidTile(GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f))) && fPlayerPosY + 1.0f < (float)((int)fNewPlayerPosY + 1.0f) + 0.1f))
		{
			fNewPlayerPosY = (int)fNewPlayerPosY; // Remove this line to create shifting sand
			fPlayerVelY = 0;
			bPlayerOnGround = true;
			bDoubleJump = true;
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

	// Draw Level background
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
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, olc::CYAN);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					SetPixelMode(olc::Pixel::NORMAL);
					break;
				case L'w': // Door
					SetPixelMode(olc::Pixel::ALPHA);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
					SetPixelMode(olc::Pixel::NORMAL);
					break;
			}
		}
	}

	// Ennemies
	for (auto& object : vecEnnemies)
	{
		float fNewObjectPosX = object->px + object->vx * fElapsedTime;
		float fNewObjectPosY = object->py + object->vy * fElapsedTime;

		// Collision
		float fBorder = 0.1f;
		bool bCollisionWithMap = false;

		// Gravity
		object->vy += cfGravity * fElapsedTime;

		if (object->vy > cfMaxPlayerVelY)
			object->vy = cfMaxPlayerVelY;

		if (object->vx <= 0) // Moving Left
		{
			if (IsSolidTile(GetTile(fNewObjectPosX + fBorder, object->py + 0.0f)) || IsSolidTile(GetTile(fNewObjectPosX + fBorder, object->py + 0.9f)))
			{
				fNewObjectPosX = (int)fNewObjectPosX + 1;
				object->vx = 0;
				bCollisionWithMap = true;
			}
		}
		else // Moving Right
		{
			if (IsSolidTile(GetTile(fNewObjectPosX + (1.0f - fBorder), object->py + 0.0f)) || IsSolidTile(GetTile(fNewObjectPosX + (1.0f - fBorder), object->py + 0.9f)))
			{
				fNewObjectPosX = (int)fNewObjectPosX;
				object->vx = 0;
				bCollisionWithMap = true;
			}
		}

		if (object->vy <= 0) // Moving Up
		{
			if (IsSolidTile(GetTile(fNewObjectPosX + fBorder + 0.0f, fNewObjectPosY)) || IsSolidTile(GetTile(fNewObjectPosX + (1.0f - fBorder), fNewObjectPosY)))
			{
				fNewObjectPosY = (int)fNewObjectPosY + 1;
				object->vy = 0;
				bCollisionWithMap = true;
			}
		}
		else // Moving Down
		{
			if (IsSolidTile(GetTile(fNewObjectPosX + fBorder + 0.0f, fNewObjectPosY + 1.0f)) || IsSolidTile(GetTile(fNewObjectPosX + (1.0f - fBorder), fNewObjectPosY + 1.0f)) ||
				IsSemiSolidTile(GetTile(fNewObjectPosX + fBorder + 0.0f, fNewObjectPosY + 1.0f)) || IsSemiSolidTile(GetTile(fNewObjectPosX + (1.0f - fBorder), fNewObjectPosY + 1.0f)))
			{
				fNewObjectPosY = (int)fNewObjectPosY;
				object->vy = 0;
				bCollisionWithMap = true;
			}
		}

		float fDynObjectPosX = fNewObjectPosX;
		float fDynObjectPosY = fNewObjectPosY;

		// Object vs Object collisions
		for (auto& dyn : vecEnnemies)
		{
			if (dyn != object)
			{
				// If the objects are solid then they must not overlap
				if (dyn->bSolidVsDyn && object->bSolidVsDyn)
				{
					// Check if bounding rectangles overlap
					if (fDynObjectPosX < (dyn->px + 1.0f) && (fDynObjectPosX + 1.0f) > dyn->px &&
						object->py < (dyn->py + 1.0f) && (object->py + 1.0f) > dyn->py)
					{
						// First Check Horizontally - Check Left
						if (object->vx <= 0)
							fDynObjectPosX = dyn->px + 1.0f;
						else
							fDynObjectPosX = dyn->px - 1.0f;

						object->TurnAround();
					}
				}
			}
		}

		// Check collision with player to damage him
		if (bIsPlayerAttackable)
		{
			// Check for the four corners of the player
			if ((fDynObjectPosX + 0.1f <= fPlayerPosX + 0.9f && fDynObjectPosX + 0.1f >= fPlayerPosX && fDynObjectPosY + 0.1f <= fPlayerPosY + 0.9f && fDynObjectPosY + 0.1f >= fPlayerPosY) ||
				(fDynObjectPosX + 0.9f <= fPlayerPosX + 0.9f && fDynObjectPosX + 0.9f >= fPlayerPosX && fDynObjectPosY + 0.1f <= fPlayerPosY + 0.9f && fDynObjectPosY + 0.1f >= fPlayerPosY) ||
				(fDynObjectPosX + 0.1f <= fPlayerPosX + 0.9f && fDynObjectPosX + 0.1f >= fPlayerPosX && fDynObjectPosY + 0.9f <= fPlayerPosY + 0.9f && fDynObjectPosY + 0.9f >= fPlayerPosY) ||
				(fDynObjectPosX + 0.9f <= fPlayerPosX + 0.9f && fDynObjectPosX + 0.9f >= fPlayerPosX && fDynObjectPosY + 0.9f <= fPlayerPosY + 0.9f && fDynObjectPosY + 0.9f >= fPlayerPosY))
			{
				animPlayer.ChangeState("damaged");
				fInvulnerabilityTimer = cfInvulnerabilityFrame;
				bPlayerDamaged = true;
				bIsPlayerAttackable = false;
				fHealth -= 5;

				// Knockback the player out of the ennemy
				if (fDynObjectPosX < fPlayerPosX)
				{
					fPlayerVelX = cfDamageEjectionVelX;
					fPlayerVelY = cfDamageEjectionVelY;
				}
				else
				{
					fPlayerVelX = -cfDamageEjectionVelX;
					fPlayerVelY = -cfDamageEjectionVelY;
				}
			}
		}

		object->px = fDynObjectPosX;
		object->py = fDynObjectPosY;
	}

	for (auto& object : vecEnnemies)
	{
		object->Update(fElapsedTime, fPlayerPosX, fPlayerPosY);
	}

	// Remove dead ennemies
	auto i = std::remove_if(vecEnnemies.begin(), vecEnnemies.end(), [](const cDynamicCreature* d)
	{
		return d->bDead;
	});
	if (i != vecEnnemies.end())
		vecEnnemies.erase(i);

	// Draw Ennemies
	for (auto& object : vecEnnemies)
	{
		object->DrawSelf(this, fOffsetX, fOffsetY);
	}

	// Check invulnerability frame
	fInvulnerabilityTimer -= fElapsedTime;
	if (fInvulnerabilityTimer <= 0.0f)
	{
		fInvulnerabilityTimer = 0.0f;
		bShowKirby = true;
		bIsPlayerAttackable = true;
	}
	else
	{
		fInvulnerabilityTickingTimer += fElapsedTime;
		if (fInvulnerabilityTickingTimer >= cfInvulnerabilityTickingSpeed)
		{
			fInvulnerabilityTickingTimer -= cfInvulnerabilityTickingSpeed;

			// Start ticking only after damage animation
			if (!bPlayerDamaged)
				bShowKirby = !bShowKirby;
		}
	}

	t.Translate((fPlayerPosX - fOffsetX) * nTileWidth + (nTileWidth / 2), (fPlayerPosY - fOffsetY) * nTileHeight + (nTileHeight / 2));

	if (bShowKirby)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		animPlayer.DrawSelf(this, t);
		SetPixelMode(olc::Pixel::NORMAL);
	}

#pragma region HUD

	// Health bar
	SetPixelMode(olc::Pixel::ALPHA);
	DrawSprite(0, 0, sprHealthBar);
	SetPixelMode(olc::Pixel::NORMAL);

	// Health points
	for (int i = 0; i < fHealth; i++)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawSprite(13 + i * 10, 14, sprHealthPoint);
		SetPixelMode(olc::Pixel::NORMAL);
	}

#pragma endregion

	return true;
}

bool OneLoneCoder_Platformer::GameState_WorldMap(float fElapsedTime)
{
	worldMap->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_LOADLEVEL;

	return false;
}

bool OneLoneCoder_Platformer::GameState_EndScreen(float fElapsedTime)
{
	endScreen->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_TITLE;

	return false;
}

bool OneLoneCoder_Platformer::GameState_PauseMenu(float fElapsedTime)
{
	pauseMenu->Update(this, fElapsedTime);

	if (GetKey(olc::Key::P).bPressed)
	{
		nGameState = GS_MAIN;
	}

	return true;
}

bool OneLoneCoder_Platformer::IsSolidTile(wchar_t tile)
{
	// List Here all the tiles that are not solid (if there are less non solid tile than solid ones)
	return tile != '.' && tile != 'o' && tile != 'w' && tile != '?';
}

bool OneLoneCoder_Platformer::IsSemiSolidTile(wchar_t tile)
{
	// List Here all the tiles that are semi solid
	return tile == '?';
}

bool OneLoneCoder_Platformer::ShapeOverlap_DIAG(polygon& r1, polygon& r2)
{
	polygon* poly1 = &r1;
	polygon* poly2 = &r2;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			poly1 = &r2;
			poly2 = &r1;
		}

		// Check diagonals of polygon...
		for (int p = 0; p < poly1->p.size(); p++)
		{
			vec2d line_r1s = poly1->pos;
			vec2d line_r1e = poly1->p[p];

			// ...against edges of the other
			for (int q = 0; q < poly2->p.size(); q++)
			{
				vec2d line_r2s = poly2->p[q];
				vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];

				// Standard "off the shelf" line segment intersection
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void OneLoneCoder_Platformer::SlapAttack(cDynamicCreature* victim)
{
	if (victim != nullptr)
	{
		// Attack victim with damage
		victim->nHealth -= 5;

		// Knock victim back
		float tx = victim->px - fPlayerPosX;
		float ty = victim->py - fPlayerPosY;
		float d = sqrtf(tx * tx + ty * ty);
		if (d < 1) d = 1.0f;

		// After a hit, the object experiences knock back, where it is temporarily
		// under system control. This delivers two functions, the first being
		// a visual indicator to the player that something has happened, and the second
		// it stops the ability to spam attacks on a single creature
		victim->KnockBack(tx / d, ty / d, 0.2f);
	}
}
