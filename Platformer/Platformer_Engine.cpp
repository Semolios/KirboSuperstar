#include "Platformer_Engine.h"

OneLoneCoder_Platformer::OneLoneCoder_Platformer()
{
	sAppName = "Tile Based Platform Game";
}

bool OneLoneCoder_Platformer::OnUserCreate()
{
	return true;
}

bool OneLoneCoder_Platformer::OnUserUpdate(float fElapsedTime)
{
	switch (nGameState)
	{
		case GS_LOADING:		GameState_Loading(fElapsedTime); break;
		case GS_TITLE:			GameState_Title(fElapsedTime); break;
		case GS_MAIN:			GameState_Main(fElapsedTime); break;
		case GS_TRANSITION:		GameState_Transition(fElapsedTime); break;
		case GS_LOADLEVEL:		GameState_LoadLevel(fElapsedTime); break;
		case GS_WORLDMAP:		GameState_WorldMap(fElapsedTime); break;
		case GS_ENDSCREEN:		GameState_EndScreen(fElapsedTime); break;
		case GS_PAUSE:			GameState_PauseMenu(fElapsedTime); break;
		case GS_LOADBOSSLEVEL:	GameState_LoadBossLevel(fElapsedTime); break;
		case GS_SELECTMENU:		GameState_SelectMenu(fElapsedTime); break;
		case GS_CONTROLS:		GameState_Controls(fElapsedTime); break;
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
#pragma region Assets

	cAssets::get().LoadSprites();
	cAssets::get().LoadItems();

#pragma endregion

#pragma region Loading Levels

	level = new cLevel();
	cLevel::engine = this;
	sprDoor = new olc::Sprite("assets/gfx/door.png");

	levels = level->LoadLevelsList();
	bossLevels = level->LoadBossLevelsList();
	levelsEnnemies = level->LoadLevelsEnnemiesList();
	levelsTiles = level->LoadLevelsTilesList();
	groundTiles = level->LoadLevelsGrdTilesList();
	levelsBackgrounds = level->LoadLevelsBackGroundList();
	bossLevelsBackgrounds = level->LoadLevelsBossBckGrdList();

#pragma endregion

#pragma region Load Animations

	animPlayer.LoadAnimations();

#pragma endregion

#pragma region Projectiles sprites

	mapProjectiles = cDynamicProjectile::LoadProjectilesSprites();

#pragma endregion

#pragma region Title Screen

	sprTitleScreen = new olc::Sprite("assets/gfx/title screen.png");
	titleScreen = new cTitleScreen(this, sprTitleScreen);

#pragma endregion

#pragma region World Map

	sprWorldMap = new olc::Sprite("assets/gfx/WorldMap.png");
	worldMap = new cWorldMap(this, sprWorldMap);
	cWorldMap::animPlayer = &animPlayer;
	worldMap->SetUnlockedLevel(nUnlockedLevel);

#pragma endregion

#pragma region Transition

	sprTransition = new olc::Sprite("assets/gfx/transitionScreen.png");
	transition = new cTransition(this, sprTransition);
	cTransition::animPlayer = &animPlayer;

#pragma endregion

#pragma region End Screen

	sprEndScreen = new olc::Sprite("assets/gfx/endScreen.png");
	endScreen = new cEndScreen(this, sprEndScreen);

#pragma endregion

#pragma region Pause Menu

	sprPauseMenu = new olc::Sprite("assets/gfx/PauseMenu.png");
	sprCursor = new olc::Sprite("assets/gfx/cursor.png");
	pauseMenu = new cPauseMenu(this, sprPauseMenu, sprCursor);

#pragma endregion

#pragma region Select Menu

	sprSelectMenu = new olc::Sprite("assets/gfx/SelectMenu.png");
	selectMenu = new cSelectMenu(this, sprSelectMenu, sprCursor);

#pragma endregion

#pragma region Controls Menu

	sprControlsMenu = new olc::Sprite("assets/gfx/ControlsMenu.png");
	controlsMenu = new cControlsMenu(this, sprControlsMenu);

#pragma endregion

#pragma region HUD

	HUD = new cHUD();
	sprHealthBar = new olc::Sprite("assets/gfx/emptyHealthBar.png");
	sprHealthPoint = new olc::Sprite("assets/gfx/healthPoint.png");
	sprBossHealthBar = new olc::Sprite("assets/gfx/emptyBossHealthBar.png");

#pragma endregion

#pragma region Static Classes engine initialisation

	cDynamicCreature::engine = this;
	cDynamicCreatureBladeKnight::engine = this;
	cDynamicCreatureBomber::engine = this;
	cDynamicCreatureMrShineMrBright::engine = this;
	cDynamicCreatureRocky::engine = this;
	cDynamicCreatureWaddleDee::engine = this;
	cDynamicCreatureWhispyWood::engine = this;
	cDynamicProjectile::engine = this;
	cPlayer::engine = this;

#pragma endregion

#pragma region Player

	player = new cPlayer(&animPlayer);

#pragma endregion

	nGameState = GS_TITLE;

	return true;
}

bool OneLoneCoder_Platformer::GameState_LoadLevel(float fElapsedTime)
{
	// Destroy all dynamics
	vecEnnemies.clear();
	vecProjectiles.clear();

	nCurrentLevel = worldMap->GetSelectedLevel();
	if (level->LoadLevel(levels[nCurrentLevel]))
	{
		LoadLevelProperties();

		level->PopulateEnnemies(vecEnnemies, levelsEnnemies[nCurrentLevel]);

		spriteTiles = new olc::Sprite(levelsTiles[nCurrentLevel]);
		sprGrdTiles = new olc::Sprite(groundTiles[nCurrentLevel]);
		sprBackground = new olc::Sprite(levelsBackgrounds[nCurrentLevel]);
	}

	// Reset variables when level is loading
	player->InitialiseKirboHealth();
	ResetVariables();

	srand(time(NULL));
	transitionAnim = rand() % 4;

	bInBossLvl = false;

	nGameState = GS_TRANSITION;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Title(float fElapsedTime)
{
	titleScreen->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_SELECTMENU;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Main(float fElapsedTime)
{
	// Stop time a while before dead animation
	if (player->IsDead())
	{
		fStopTimebeforeDeadAnim += fElapsedTime;

		if (fStopTimebeforeDeadAnim < cfStopTimebeforeDeadAnim)
			return true;
	}

	animPlayer.Update(fElapsedTime);

	player->HandleInput(this, fElapsedTime);
	// Handle pause button pressed
	if (bBreakLoop)
	{
		bBreakLoop = false;
		return true;
	}

	// Clamp camera offset
	if (fCameraLookingDown <= cfCameraLowerPos) fCameraLookingDown = cfCameraLowerPos;
	if (fCameraLookingDown >= cfCameraUpperPos) fCameraLookingDown = cfCameraUpperPos;

	player->ApplyGravity(fElapsedTime);

	player->Update(fElapsedTime);

	// Draw Player
	olc::GFX2D::Transform2D t;
	t.Translate(((float)-nTileWidth / 2.0f) - cnSpriteOffsetX, ((float)-nTileWidth / 2.0f) - cnSpriteOffsetY);
	t.Scale(player->GetFaceDir() * 1.0f, 1.0f);

	player->OneCycleAnimations(fElapsedTime, &t, mapProjectiles);
	// Handle State Change
	if (bBreakLoop)
	{
		bBreakLoop = false;
		return true;
	}

	player->ClampVelocities();

	// Wind effect
	if (bWind)
		player->IncreaseVelocities(fWindDirection * fWindPower * fElapsedTime, 0);

	player->Collisions(fElapsedTime, level);

	fCameraPosX = player->GetPlayerPosX();
	fCameraPosY = player->GetPlayerPosY();

	// Draw level
	int nVisibleTilesX = ScreenWidth() / nTileWidth;
	int nVisibleTilesY = ScreenHeight() / nTileHeight;

	// Calculate Top-Left most visible tile
	float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
	float fOffsetY = fCameraPosY - (float)nVisibleTilesY * fCameraLookingDown;

	// Clamp camera to game boundaries
	if (fOffsetX < 1) fOffsetX = 1;
	if (fOffsetY < 1) fOffsetY = 1;
	if (fOffsetX > level->GetWidth() - nVisibleTilesX - 1) fOffsetX = level->GetWidth() - nVisibleTilesX - 1;
	if (fOffsetY > level->GetHeight() - nVisibleTilesY - 1) fOffsetY = level->GetHeight() - nVisibleTilesY - 1;

	if (bShake)
	{
		fShakeTimerChange += fElapsedTime;

		if (fShakeTimerChange >= cfShakeEffectChangeFrequency)
		{
			fShakeEffectX = ((float)(rand() % nShakeAmplitudeX) - 100.0f) / cfShakeAttenuation;
			fShakeEffectY = ((float)(rand() % nShakeAmplitudeY) - 100.0f) / cfShakeAttenuation;
		}

		fOffsetX += fShakeEffectX;
		fOffsetY += fShakeEffectY;
	}

	// Get offsets for smooth movement
	float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
	float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

	// Draw Level background
	float fBackgroundOffsetX = fOffsetX * (float)nTileWidth * ((float)(sprBackground->width - ScreenWidth()) / (float)(level->GetWidth() * nTileWidth - ScreenWidth()));
	float fBackgroundOffsetY = fOffsetY * (float)nTileHeight * ((float)(sprBackground->height - ScreenHeight()) / (float)(level->GetWidth() * nTileHeight - ScreenHeight()));
	DrawPartialSprite(0, 0, sprBackground, fBackgroundOffsetX, fBackgroundOffsetY, ScreenWidth(), ScreenHeight());

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
					DrawGroundTile(x, y, fOffsetX, fOffsetY, fTileOffsetX, fTileOffsetY, sprGrdTiles, sTileID);
					break;
				case L'B': // Brick Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'?': // Question Block
					SetPixelMode(olc::Pixel::ALPHA);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					SetPixelMode(olc::Pixel::NORMAL);
					break;
				case L'o': // Coin
					SetPixelMode(olc::Pixel::ALPHA);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					SetPixelMode(olc::Pixel::NORMAL);
					break;
				case L'w': // Door
					SetPixelMode(olc::Pixel::ALPHA);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY + (nTileHeight - sprDoor->height) + 5.0f, sprDoor, 0, 0, sprDoor->width, sprDoor->height);
					SetPixelMode(olc::Pixel::NORMAL);
					break;
					// TODO
					/*ADD HERE THE NEW TILES*/
			}
		}
	}

	// Ennemies
	for (auto& object : vecEnnemies)
	{
		object->Collision(fElapsedTime);

		// Check if the ennemi is in the vacuum
		if (object->bIsVacuumable)
		{
			if (player->IsVacuuming())
			{
				polygon sEnnemy;
				sEnnemy.pos = {
					(object->px - fOffsetX) * nTileWidth + (object->fDynWidth / 2.0f),
					(object->py - fOffsetY) * nTileHeight + (object->fDynHeight / 2.0f)
				};
				sEnnemy.angle = 0.0f;
				sEnnemy.o.push_back({ -object->fDynWidth / 2.0f, -object->fDynHeight / 2.0f });
				sEnnemy.o.push_back({ -object->fDynWidth / 2.0f, +object->fDynHeight / 2.0f });
				sEnnemy.o.push_back({ +object->fDynWidth / 2.0f, +object->fDynHeight / 2.0f });
				sEnnemy.o.push_back({ +object->fDynWidth / 2.0f, -object->fDynHeight / 2.0f });
				sEnnemy.p.resize(4);

				for (int i = 0; i < sEnnemy.o.size(); i++)
				{
					sEnnemy.p[i] =
					{	// 2D Rotation Transform + 2D Translation
						(sEnnemy.o[i].x * cosf(sEnnemy.angle)) - (sEnnemy.o[i].y * sinf(sEnnemy.angle)) + sEnnemy.pos.x,
						(sEnnemy.o[i].x * sinf(sEnnemy.angle)) + (sEnnemy.o[i].y * cosf(sEnnemy.angle)) + sEnnemy.pos.y,
					};
				}

				// Debug aoe
				//DrawLine(sEnnemy.p[0].x, sEnnemy.p[0].y, sEnnemy.p[1].x, sEnnemy.p[1].y, olc::YELLOW);
				//DrawLine(sEnnemy.p[1].x, sEnnemy.p[1].y, sEnnemy.p[2].x, sEnnemy.p[2].y, olc::YELLOW);
				//DrawLine(sEnnemy.p[2].x, sEnnemy.p[2].y, sEnnemy.p[3].x, sEnnemy.p[3].y, olc::YELLOW);
				//DrawLine(sEnnemy.p[3].x, sEnnemy.p[3].y, sEnnemy.p[0].x, sEnnemy.p[0].y, olc::YELLOW);

				polygon sVacuum;
				sVacuum.pos = {
					(player->GetPlayerPosX() + (player->GetFaceDir() > 0.0f ? 1.75f : -0.75f) - fOffsetX) * (float)nTileWidth,
					(player->GetPlayerPosY() + 0.5f - fOffsetY) * (float)nTileHeight
				}; // 1 block ahead the player's looking direction
				sVacuum.angle = 0.0f;
				sVacuum.o.push_back({ -(float)nTileWidth * 1.25f, -(float)nTileHeight / (player->GetFaceDir() > 0.0f ? 2.0f : 1.0f) });
				sVacuum.o.push_back({ -(float)nTileWidth * 1.25f, +(float)nTileHeight / (player->GetFaceDir() > 0.0f ? 2.0f : 1.0f) });
				sVacuum.o.push_back({ +(float)nTileWidth * 1.25f, +(float)nTileHeight / (player->GetFaceDir() > 0.0f ? 1.0f : 2.0f) });
				sVacuum.o.push_back({ +(float)nTileWidth * 1.25f, -(float)nTileHeight / (player->GetFaceDir() > 0.0f ? 1.0f : 2.0f) });
				sVacuum.p.resize(4);

				for (int i = 0; i < sVacuum.o.size(); i++)
				{
					sVacuum.p[i] =
					{	// 2D Rotation Transform + 2D Translation (angle is always 0 here, no rotation allowed)
						(sVacuum.o[i].x * cosf(sVacuum.angle)) - (sVacuum.o[i].y * sinf(sVacuum.angle)) + sVacuum.pos.x,
						(sVacuum.o[i].x * sinf(sVacuum.angle)) + (sVacuum.o[i].y * cosf(sVacuum.angle)) + sVacuum.pos.y,
					};
				}

				// Debug AOE
				//DrawLine(sVacuum.p[0].x, sVacuum.p[0].y, sVacuum.p[1].x, sVacuum.p[1].y, olc::GREEN);
				//DrawLine(sVacuum.p[1].x, sVacuum.p[1].y, sVacuum.p[2].x, sVacuum.p[2].y, olc::GREEN);
				//DrawLine(sVacuum.p[2].x, sVacuum.p[2].y, sVacuum.p[3].x, sVacuum.p[3].y, olc::GREEN);
				//DrawLine(sVacuum.p[3].x, sVacuum.p[3].y, sVacuum.p[0].x, sVacuum.p[0].y, olc::GREEN);

				if (ShapeOverlap_DIAG(sEnnemy, sVacuum))
				{
					ChangeEnnemyProperties(object, true);
					Attack(object, 0);

					player->VacuumEnnemy(object);
				}
				else
				{
					ChangeEnnemyProperties(object, false);
					object->bSwallowable = false;
				}
			}
			else
			{
				ChangeEnnemyProperties(object, false);
				object->bSwallowable = false;
			}
		}

		// Check collision with player to damage him
		if (player->IsAttackable() && !player->IsSwallowing() && !object->bVacuumed)
		{
			CheckIfPlayerIsDamaged(object, 0.0f, fOffsetX, fOffsetY);
		}
	}

	// Projectiles
	for (auto& object : vecProjectiles)
	{
		object->Collision(fElapsedTime);

		// check if a Projectile hits a creature
		if (!object->bRedundant)
		{
			if (object->bFriendly)
			{
				// check from which corner we draw the sprite
				float sprPosX = (object->nCornerSpr == 0 || object->nCornerSpr == 3) ? 0.0f : object->fDynWidth;
				float sprPosY = (object->nCornerSpr == 0 || object->nCornerSpr == 1) ? 0.0f : -object->fDynHeight;

				// Create an AOE on the projectile and check for ennemies and AOE overlap
				polygon sAOE;
				sAOE.pos = {
					(object->px - fOffsetX) * nTileWidth + (object->fDynWidth / 2.0f) + sprPosX,
					(object->py - fOffsetY) * nTileHeight + (object->fDynHeight / 2.0f) + sprPosY
				};
				sAOE.angle = atan2f(object->vy, object->vx);
				sAOE.o.push_back({ -object->fDynWidth / 2.0f, -object->fDynHeight / 2.0f });
				sAOE.o.push_back({ -object->fDynWidth / 2.0f, +object->fDynHeight / 2.0f });
				sAOE.o.push_back({ +object->fDynWidth / 2.0f, +object->fDynHeight / 2.0f });
				sAOE.o.push_back({ +object->fDynWidth / 2.0f, -object->fDynHeight / 2.0f });
				sAOE.p.resize(4);

				for (int i = 0; i < sAOE.o.size(); i++)
				{
					sAOE.p[i] =
					{	// 2D Rotation Transform + 2D Translation
						(sAOE.o[i].x * cosf(sAOE.angle)) - (sAOE.o[i].y * sinf(sAOE.angle)) + sAOE.pos.x,
						(sAOE.o[i].x * sinf(sAOE.angle)) + (sAOE.o[i].y * cosf(sAOE.angle)) + sAOE.pos.y,
					};
				}

				// Debug aoe
				//DrawLine(sAOE.p[0].x, sAOE.p[0].y, sAOE.p[1].x, sAOE.p[1].y, olc::RED);
				//DrawLine(sAOE.p[1].x, sAOE.p[1].y, sAOE.p[2].x, sAOE.p[2].y, olc::RED);
				//DrawLine(sAOE.p[2].x, sAOE.p[2].y, sAOE.p[3].x, sAOE.p[3].y, olc::RED);
				//DrawLine(sAOE.p[3].x, sAOE.p[3].y, sAOE.p[0].x, sAOE.p[0].y, olc::RED);

				// Check if an ennemy take the attack
				for (auto& dyn : vecEnnemies)
				{
					polygon sEnnemy;
					sEnnemy.pos = {
						((float)dyn->px - fOffsetX) * nTileWidth + (float)dyn->fDynWidth / 2.0f,
						((float)dyn->py - fOffsetY) * nTileHeight + (float)dyn->fDynHeight / 2.0f
					}; // Center of the ennemy
					sEnnemy.angle = 0.0f;
					sEnnemy.o.push_back({ -dyn->fDynWidth / 2.0f, -dyn->fDynHeight / 2.0f });
					sEnnemy.o.push_back({ -dyn->fDynWidth / 2.0f, +dyn->fDynHeight / 2.0f });
					sEnnemy.o.push_back({ +dyn->fDynWidth / 2.0f, +dyn->fDynHeight / 2.0f });
					sEnnemy.o.push_back({ +dyn->fDynWidth / 2.0f, -dyn->fDynHeight / 2.0f });
					sEnnemy.p.resize(4);

					for (int i = 0; i < sEnnemy.o.size(); i++)
					{
						sEnnemy.p[i] =
						{	// 2D Rotation Transform + 2D Translation (angle is always 0 here, no rotation allowed)
							(sEnnemy.o[i].x * cosf(sEnnemy.angle)) - (sEnnemy.o[i].y * sinf(sEnnemy.angle)) + sEnnemy.pos.x,
							(sEnnemy.o[i].x * sinf(sEnnemy.angle)) + (sEnnemy.o[i].y * cosf(sEnnemy.angle)) + sEnnemy.pos.y,
						};
					}

					// Debug AOE
					//DrawLine(sEnnemy.p[0].x, sEnnemy.p[0].y, sEnnemy.p[1].x, sEnnemy.p[1].y, olc::BLUE);
					//DrawLine(sEnnemy.p[1].x, sEnnemy.p[1].y, sEnnemy.p[2].x, sEnnemy.p[2].y, olc::BLUE);
					//DrawLine(sEnnemy.p[2].x, sEnnemy.p[2].y, sEnnemy.p[3].x, sEnnemy.p[3].y, olc::BLUE);
					//DrawLine(sEnnemy.p[3].x, sEnnemy.p[3].y, sEnnemy.p[0].x, sEnnemy.p[0].y, olc::BLUE);

					if (ShapeOverlap_DIAG(sAOE, sEnnemy))
					{
						if (dyn->bIsAttackable)
						{
							Attack((cDynamicCreature*)dyn, object->nDamage);
							if (object->bOneHit)
								object->bRedundant = true;
						}
					}
				}
			}
			else
			{
				if (player->IsAttackable())
				{
					CheckIfPlayerIsDamaged(object, atan2f(object->vy, object->vx), fOffsetX, fOffsetY);
				}
			}
		}
	}

	for (auto& object : vecEnnemies)
	{
		object->Update(fElapsedTime, player->GetPlayerPosX(), player->GetPlayerPosY(), this);
	}

	for (auto& object : vecProjectiles)
	{
		object->Update(fElapsedTime, player->GetPlayerPosX(), player->GetPlayerPosY(), this);
	}

	// Remove dead ennemies
	vecEnnemies.erase(remove_if(vecEnnemies.begin(), vecEnnemies.end(), [](const cDynamicCreature* d)
	{
		return ((cDynamicCreature*)d)->bDead;
	}), vecEnnemies.end());

	// Remove swallowed ennemies
	if (player->IsSwallowing())
	{
		vecEnnemies.erase(remove_if(vecEnnemies.begin(), vecEnnemies.end(), [](const cDynamicCreature* d)
		{
			return ((cDynamicCreature*)d)->bSwallowable;
		}), vecEnnemies.end());
	}

	// Erase and delete redundant projectiles
	vecProjectiles.erase(remove_if(vecProjectiles.begin(), vecProjectiles.end(), [](const cDynamic* d)
	{
		return ((cDynamicProjectile*)d)->bRedundant;
	}), vecProjectiles.end());

	// Draw Ennemies
	for (auto& object : vecEnnemies)
	{
		object->DrawSelf(this, fOffsetX, fOffsetY);
	}

	// Draw Projectiles
	for (auto& object : vecProjectiles)
	{
		object->DrawSelf(this, fOffsetX, fOffsetY);
	}

	if (bInBossLvl && vecEnnemies.empty())
	{
		// Wait a little before begin win animation
		fWaitBeforeWinAnimation += fElapsedTime;

		if (fWaitBeforeWinAnimation >= cfTimeBeforeWinAnimation)
		{
			bBossKilled = true;
			if (fWinTimer < animPlayer.mapStates["boss_killed"].size() * animPlayer.fTimeBetweenFrames)
				animPlayer.ChangeState("boss_killed");
		}

		// When win animation is over, quit the level
		if (fWinTimer >= cfBossKilledAnimation)
		{
			if (nCurrentLevel + 1 == levels.size())
			{
				// If Player finishes the last level of the game, the game is over
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
	}

	player->UpdateInvulnerability(fElapsedTime);
	t.Translate((player->GetPlayerPosX() - fOffsetX) * nTileWidth + (nTileWidth / 2), (player->GetPlayerPosY() - fOffsetY) * nTileHeight + (nTileHeight / 2));
	player->DrawKirbo(this, t);

#pragma region HUD

	HUD->HealthBar(this, sprHealthBar);
	HUD->HealthPoints(this, sprHealthPoint, player->GetHealth());

	if (bInBossLvl)
		HUD->BossHealthBar(this, sprBossHealthBar, vecEnnemies);

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
	else if (GetKey(olc::Key::SPACE).bPressed)
	{
		if (pauseMenu->GetPlayerChoice() == 1)
		{
			animPlayer.ChangeState("riding_star");
			nGameState = GS_WORLDMAP;
		}
		else
		{
			nGameState = GS_MAIN;
		}
	}

	return true;
}

bool OneLoneCoder_Platformer::GameState_LoadBossLevel(float fElapsedTime)
{
	// Destroy all dynamics
	vecEnnemies.clear();
	vecProjectiles.clear();

	if (level->LoadLevel(bossLevels[nCurrentLevel]))
	{
		LoadLevelProperties();

		level->PopulateBoss(vecEnnemies, nCurrentLevel);

		sprBackground = new olc::Sprite(bossLevelsBackgrounds[nCurrentLevel]);
	}

	// Reset variables when level is loading
	ResetVariables();

	bInBossLvl = true;

	nGameState = GS_MAIN;

	return true;
}

bool OneLoneCoder_Platformer::GameState_SelectMenu(float fElapsedTime)
{
	selectMenu->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
	{
		if (selectMenu->GetPlayerChoice() == 0)
		{
			animPlayer.ChangeState("riding_star");
			nGameState = GS_WORLDMAP;
		}
		else
		{
			nGameState = GS_CONTROLS;
		}
	}
	return true;
}

bool OneLoneCoder_Platformer::GameState_Controls(float fElapsedTime)
{
	controlsMenu->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
		nGameState = GS_SELECTMENU;

	return true;
}

void OneLoneCoder_Platformer::LoadLevelProperties()
{
	player->SetPlayerPosX(level->GetInitPlayerPosX());
	player->SetPlayerPosY(level->GetInitPlayerPosY());
	sLevel = level->GetLevel();
}

void OneLoneCoder_Platformer::CheckIfPlayerIsDamaged(cDynamic* object, float angle, float fOffsetX, float fOffsetY)
{
	// check from which corner we draw the sprite
	float sprPosX = (object->nCornerSpr == 0 || object->nCornerSpr == 3) ? 0.0f : object->fDynWidth;
	float sprPosY = (object->nCornerSpr == 0 || object->nCornerSpr == 1) ? 0.0f : -object->fDynHeight;

	polygon sAOE;
	sAOE.pos = {
		(object->px - fOffsetX) * nTileWidth + (object->fDynWidth / 2.0f) + sprPosX,
		(object->py - fOffsetY) * nTileHeight + (object->fDynHeight / 2.0f) + sprPosY
	};
	sAOE.angle = angle;
	sAOE.o.push_back({ -object->fDynWidth / 2.0f, -object->fDynHeight / 2.0f });
	sAOE.o.push_back({ -object->fDynWidth / 2.0f, +object->fDynHeight / 2.0f });
	sAOE.o.push_back({ +object->fDynWidth / 2.0f, +object->fDynHeight / 2.0f });
	sAOE.o.push_back({ +object->fDynWidth / 2.0f, -object->fDynHeight / 2.0f });
	sAOE.p.resize(4);

	for (int i = 0; i < sAOE.o.size(); i++)
	{
		sAOE.p[i] =
		{	// 2D Rotation Transform + 2D Translation
			(sAOE.o[i].x * cosf(sAOE.angle)) - (sAOE.o[i].y * sinf(sAOE.angle)) + sAOE.pos.x,
			(sAOE.o[i].x * sinf(sAOE.angle)) + (sAOE.o[i].y * cosf(sAOE.angle)) + sAOE.pos.y,
		};
	}

	// Debug aoe
	//DrawLine(sAOE.p[0].x, sAOE.p[0].y, sAOE.p[1].x, sAOE.p[1].y, olc::RED);
	//DrawLine(sAOE.p[1].x, sAOE.p[1].y, sAOE.p[2].x, sAOE.p[2].y, olc::RED);
	//DrawLine(sAOE.p[2].x, sAOE.p[2].y, sAOE.p[3].x, sAOE.p[3].y, olc::RED);
	//DrawLine(sAOE.p[3].x, sAOE.p[3].y, sAOE.p[0].x, sAOE.p[0].y, olc::RED);

	polygon sPlayer;
	sPlayer.pos = {
		(player->GetPlayerPosX() + 0.5f - fOffsetX) * (float)nTileWidth,
		(player->GetPlayerPosY() + 0.5f - fOffsetY) * (float)nTileHeight
	}; // Center of the player
	sPlayer.angle = 0.0f;
	sPlayer.o.push_back({ -(float)nTileWidth / 2.2f, -(float)nTileHeight / 2.2f });	// little reduction of the player hitbox to allow a little overlap with attack
	sPlayer.o.push_back({ -(float)nTileWidth / 2.2f, +(float)nTileHeight / 2.2f });	// little reduction of the player hitbox to allow a little overlap with attack
	sPlayer.o.push_back({ +(float)nTileWidth / 2.2f, +(float)nTileHeight / 2.2f });	// little reduction of the player hitbox to allow a little overlap with attack
	sPlayer.o.push_back({ +(float)nTileWidth / 2.2f, -(float)nTileHeight / 2.2f });	// little reduction of the player hitbox to allow a little overlap with attack
	sPlayer.p.resize(4);

	for (int i = 0; i < sPlayer.o.size(); i++)
	{
		sPlayer.p[i] =
		{	// 2D Rotation Transform + 2D Translation (angle is always 0 here, no rotation allowed)
			(sPlayer.o[i].x * cosf(sPlayer.angle)) - (sPlayer.o[i].y * sinf(sPlayer.angle)) + sPlayer.pos.x,
			(sPlayer.o[i].x * sinf(sPlayer.angle)) + (sPlayer.o[i].y * cosf(sPlayer.angle)) + sPlayer.pos.y,
		};
	}

	// Debug AOE
	//DrawLine(sPlayer.p[0].x, sPlayer.p[0].y, sPlayer.p[1].x, sPlayer.p[1].y, olc::BLUE);
	//DrawLine(sPlayer.p[1].x, sPlayer.p[1].y, sPlayer.p[2].x, sPlayer.p[2].y, olc::BLUE);
	//DrawLine(sPlayer.p[2].x, sPlayer.p[2].y, sPlayer.p[3].x, sPlayer.p[3].y, olc::BLUE);
	//DrawLine(sPlayer.p[3].x, sPlayer.p[3].y, sPlayer.p[0].x, sPlayer.p[0].y, olc::BLUE);

	if (ShapeOverlap_DIAG(sAOE, sPlayer))
		player->Damage(object);
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

void OneLoneCoder_Platformer::Attack(cDynamicCreature* victim, int damage)
{
	if (victim != nullptr)
	{
		// Attack victim with damage
		victim->nHealth -= damage;

		// Knock victim back
		float tx = victim->px - player->GetPlayerPosX();
		float ty = victim->py - player->GetPlayerPosY();
		float d = sqrtf(tx * tx + ty * ty);
		if (d < 1) d = 1.0f;

		// After a hit, the object experiences knock back, where it is temporarily
		// under system control. This delivers two functions, the first being
		// a visual indicator to the player that something has happened, and the second
		// it stops the ability to spam attacks on a single creature
		if (victim->bIsKnockable)
			victim->KnockBack(tx / d, ty / d, cfKnockBackDuration);
		else
			victim->KnockBack(0.0f, 0.0f, cfKnockBackDuration);
	}
}

void OneLoneCoder_Platformer::ChangeEnnemyProperties(cDynamicCreature* victim, bool vaccumedState)
{
	victim->bSolidVsDyn = !vaccumedState;
	victim->bVacuumed = vaccumedState;
	victim->bIsKnockable = !vaccumedState;
}

bool OneLoneCoder_Platformer::IsSolidTile(wchar_t tile)
{
	// List Here all the tiles that are solid
	return tile == '#' || tile == 'G' || tile == 'B';
}

bool OneLoneCoder_Platformer::IsSemiSolidTile(wchar_t tile)
{
	// List Here all the tiles that are semi solid
	return tile == '?';
}

cDynamicProjectile* OneLoneCoder_Platformer::CreateProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner)
{
	cDynamicProjectile* p = new cDynamicProjectile(ox, oy, bFriend, velx, vely, duration, mapProjectiles[sprite], affectedByGravity, damage, solidVSMap, oneHit, corner);
	return p;
}

void OneLoneCoder_Platformer::AddProjectile(cDynamicProjectile* proj)
{
	vecProjectiles.push_back(proj);
}

float OneLoneCoder_Platformer::GetTileWidth()
{
	return (float)nTileWidth;
}

float OneLoneCoder_Platformer::GetTileHeight()
{
	return (float)nTileHeight;
}

float OneLoneCoder_Platformer::GetGravityValue()
{
	return cfGravity;
}

float OneLoneCoder_Platformer::GetGroundDynamicOverlay()
{
	return cfGrdDynamicOverlay;
}

wchar_t OneLoneCoder_Platformer::GetTile(int x, int y)
{
	if (x >= 0 && x < level->GetWidth() && y >= 0 && y < level->GetHeight())
		return sLevel[y * level->GetWidth() + x];
	else
		return L' ';
}

void OneLoneCoder_Platformer::SetTile(int x, int y, wchar_t c)
{
	if (x >= 0 && x < level->GetWidth() && y >= 0 && y < level->GetHeight())
		sLevel[y * level->GetWidth() + x] = c;
}

bool OneLoneCoder_Platformer::IsInBossLevel()
{
	return bInBossLvl;
}

void OneLoneCoder_Platformer::SetbInBossLevel(bool inBossLevel)
{
	bInBossLvl = inBossLevel;
}

bool OneLoneCoder_Platformer::IsBossKilled()
{
	return bBossKilled;
}

void OneLoneCoder_Platformer::SetbBossKilled(bool bossKilled)
{
	bBossKilled = bossKilled;
}

void OneLoneCoder_Platformer::SetGameState(std::string gameState)
{
	if (gameState == "GS_LOADING")				nGameState = GS_LOADING;
	else if (gameState == "GS_TITLE")			nGameState = GS_TITLE;
	else if (gameState == "GS_MAIN")			nGameState = GS_MAIN;
	else if (gameState == "GS_TRANSITION")		nGameState = GS_TRANSITION;
	else if (gameState == "GS_LOADLEVEL")		nGameState = GS_LOADLEVEL;
	else if (gameState == "GS_WORLDMAP")		nGameState = GS_WORLDMAP;
	else if (gameState == "GS_ENDSCREEN")		nGameState = GS_ENDSCREEN;
	else if (gameState == "GS_PAUSE")			nGameState = GS_PAUSE;
	else if (gameState == "GS_LOADBOSSLEVEL")	nGameState = GS_LOADBOSSLEVEL;
	else if (gameState == "GS_SELECTMENU")		nGameState = GS_SELECTMENU;
	else if (gameState == "GS_CONTROLS")		nGameState = GS_CONTROLS;
}

void OneLoneCoder_Platformer::LowerCameraPosition()
{
	fCameraLookingDown -= cfCameraMoveSpeed;
}

void OneLoneCoder_Platformer::RaiseCameraPosition()
{
	fCameraLookingDown += cfCameraMoveSpeed;
}

float OneLoneCoder_Platformer::GetDragValue()
{
	return cfDrag;
}

void OneLoneCoder_Platformer::SetPlayerChoice(int choice)
{
	pauseMenu->SetPlayerChoice(choice);
}

float OneLoneCoder_Platformer::GetGrdDynamicOverlay()
{
	return cfGrdDynamicOverlay;
}

void OneLoneCoder_Platformer::UpdateWinTimer(float fElapsedTime)
{
	fWinTimer += fElapsedTime;
}

float OneLoneCoder_Platformer::GetWinTimer()
{
	return fWinTimer;
}

void OneLoneCoder_Platformer::ResetVariables()
{
	player->ResetVariables();
	bBossKilled = false;
	bShake = false;
	fStopTimebeforeDeadAnim = 0.0f;
	fWaitBeforeWinAnimation = 0.0f;
	fWinTimer = 0.0f;
}

void OneLoneCoder_Platformer::ActivateShakeEffect(bool activate, int shakeAmplitudeX, int shakeAmplitudeY)
{
	bShake = activate;
	nShakeAmplitudeX = shakeAmplitudeX;
	nShakeAmplitudeY = shakeAmplitudeY;
}

void OneLoneCoder_Platformer::WindEffect(float direction, float windPower, bool activate)
{
	bWind = activate;
	fWindDirection = direction;
	fWindPower = windPower;
}

void OneLoneCoder_Platformer::DrawGroundTile(int x, int y, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY, olc::Sprite* spriteTiles, wchar_t tile)
{
	// Get all tiles around the current tile
	wchar_t tilesAround[3][3];
	tilesAround[0][0] = GetTile(x - 1 + fOffsetX, y - 1 + fOffsetY); tilesAround[0][1] = GetTile(x + 0 + fOffsetX, y - 1 + fOffsetY);	tilesAround[0][2] = GetTile(x + 1 + fOffsetX, y - 1 + fOffsetY);
	tilesAround[1][0] = GetTile(x - 1 + fOffsetX, y + 0 + fOffsetY); tilesAround[1][1] = tile;											tilesAround[1][2] = GetTile(x + 1 + fOffsetX, y + 0 + fOffsetY);
	tilesAround[2][0] = GetTile(x - 1 + fOffsetX, y + 1 + fOffsetY); tilesAround[2][1] = GetTile(x + 0 + fOffsetX, y + 1 + fOffsetY);	tilesAround[2][2] = GetTile(x + 1 + fOffsetX, y + 1 + fOffsetY);

	// Check the 47 configurations
	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 6 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 7 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 6 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 7 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 6 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 7 * nTileWidth, 2 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 6 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 7 * nTileWidth, 3 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 6 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 7 * nTileWidth, 4 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 5 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 5 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 5 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 5 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 4 * nTileWidth, 5 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5 * nTileWidth, 5 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 6 * nTileWidth, 5 * nTileHeight, nTileWidth, nTileHeight);
		SetPixelMode(olc::Pixel::NORMAL);
		return;
	}
}

void OneLoneCoder_Platformer::BreakLoop()
{
	bBreakLoop = true;
}
