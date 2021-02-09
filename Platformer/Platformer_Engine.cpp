#include "Platformer_Engine.h"

OneLoneCoder_Platformer::OneLoneCoder_Platformer()
{
	sAppName = "Tile Based Platform Game";
}

bool OneLoneCoder_Platformer::OnUserCreate()
{
	olc::SOUND::InitialiseAudio();
	return true;
}

bool OneLoneCoder_Platformer::OnUserUpdate(float fElapsedTime)
{
	if (fElapsedTime <= 0.15f)
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
			case GS_CLOSE:			GameState_Close(fElapsedTime); break;
		}
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
	levelsMusics = level->LoadLevelsMusics();
	bossLevelsMusics = level->LoadBossLevelsMusics();

	level->InitialiseThreadPool();

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

	cCamera::engine = this;
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

#pragma region Camera

	camera = new cCamera();
	camera->InitialiseThreadPool();

#pragma endregion

#pragma region Sounds

	sndTitleScreen = olc::SOUND::LoadAudioSample("assets/snd/titleScreen.wav");
	sndWorldMap = olc::SOUND::LoadAudioSample("assets/snd/worldMap.wav");
	sndBossKilled = olc::SOUND::LoadAudioSample("assets/snd/bossKilled.wav");
	sndWind = olc::SOUND::LoadAudioSample("assets/snd/wind.wav");

	AddSharedSound("whispyScream", sndWhispyScream, "assets/snd/whispyScream.wav");
	AddSharedSound("loseLife", sndLoseLife, "assets/snd/loseLife.wav");
	AddSharedSound("punch", sndPunch, "assets/snd/punch.wav");
	AddSharedSound("slap", sndSlap, "assets/snd/slap.wav");
	AddSharedSound("kirboWalk", sndSlap, "assets/snd/kirboWalk.wav");
	AddSharedSound("kirboFly", sndSlap, "assets/snd/kirboFly.wav");
	AddSharedSound("explosion", sndSlap, "assets/snd/explosion.wav");
	AddSharedSound("kirboJump", sndKirboJump, "assets/snd/kirboJump.wav");
	AddSharedSound("kirboHit", sndKirboHit, "assets/snd/kirboHit.wav");
	AddSharedSound("beginVacuum", sndKirboBeginVacuum, "assets/snd/kirboBeginVacuum.wav");
	AddSharedSound("vacuum", sndKirboBeginVacuum, "assets/snd/kirboVacuum.wav");
	AddSharedSound("swallow", sndKirboSwallow, "assets/snd/kirboSwallow.wav");
	AddSharedSound("wetSlap", sndWetSlap, "assets/snd/wetSlap.wav");
	AddSharedSound("earthQuake", sndEarthQuake, "assets/snd/earthQuake.wav");
	AddSharedSound("blow", sndEarthQuake, "assets/snd/blow.wav");
	AddSharedSound("inhale", sndInhale, "assets/snd/inhale.wav");
	AddSharedSound("poyo01", sndPoyo01, "assets/snd/poyo01.wav");
	AddSharedSound("poyo02", sndPoyo02, "assets/snd/poyo02.wav");
	AddSharedSound("sunShootingMoon", sndSunShootingMoon, "assets/snd/sunShootingMoon.wav");
	AddSharedSound("sunShootUp", sndSunShootUp, "assets/snd/sunShootUp.wav");
	AddSharedSound("beamCharge", sndBeamCharge, "assets/snd/beamCharge.wav");
	AddSharedSound("beam", sndBeam, "assets/snd/beam.wav");
	AddSharedSound("volcanoBoom", sndVolcanoBoom, "assets/snd/volcanoBoom.wav");

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
		sndLevelMusic = olc::SOUND::LoadAudioSample(levelsMusics[nCurrentLevel]);
	}

	// Reset variables when level is loading
	player->InitialiseKirboHealth();
	ResetVariables();

	srand(time(NULL));
	transitionAnim = rand() % 4;

	bInBossLvl = false;

	olc::SOUND::StopAll();
	olc::SOUND::PlaySample(sndLevelMusic, true);

	nGameState = GS_TRANSITION;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Title(float fElapsedTime)
{
	if (!olc::SOUND::IsSamplePlaying(sndTitleScreen)) olc::SOUND::PlaySample(sndTitleScreen, true);

	titleScreen->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
	{
		olc::SOUND::StopAll();
		nGameState = GS_SELECTMENU;
	}

	return true;
}

bool OneLoneCoder_Platformer::GameState_Main(float fElapsedTime)
{
	// Stop time a while before dead animation
	if (player->IsDead())
	{
		// stop looping kirbo sounds
		olc::SOUND::StopSample(sndKirboFly);
		olc::SOUND::StopSample(sndKirboWalk);
		// stop music
		olc::SOUND::StopSample(sndLevelMusic);
		olc::SOUND::StopSample(sndBossLevelMusic);

		fStopTimebeforeDeadAnim += fElapsedTime;

		if (fStopTimebeforeDeadAnim < cfStopTimebeforeDeadAnim)
			return true;
	}

	animPlayer.Update(fElapsedTime);

	player->HandleInput(fElapsedTime, camera, level);
	// Handle pause button pressed
	if (bBreakLoop)
	{
		bBreakLoop = false;
		return true;
	}

	camera->ClampCameraOffset();

	player->ApplyGravity(fElapsedTime);

	player->Update(fElapsedTime);

	// Initialize the Transform2D object
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

	camera->SetPositions(player->GetPlayerPosX(), player->GetPlayerPosY());

	camera->DrawLevel(level, fElapsedTime);

	// Ennemies
	for (auto& object : vecEnnemies)
	{
		object->Collision(fElapsedTime);

		// Check if the ennemi is in the vacuum
		if (object->bIsVacuumable)
		{
			if (player->IsVacuuming())
			{
				player->Vacuum(object, camera->GetOffsetX(), camera->GetOffsetY());
			}
			else
			{
				object->Vacuumed(false);
				object->bSwallowable = false;
			}
		}

		// Check collision with player to damage him
		if (player->IsAttackable() && !player->IsSwallowing() && !object->bVacuumed)
		{
			player->CheckIfDamaged(object, camera->GetOffsetX(), camera->GetOffsetY());
		}
	}

	// Projectiles
	for (auto& object : vecProjectiles)
	{
		object->Collision(fElapsedTime, level);

		// check if a Projectile hits a creature
		if (!object->bRedundant)
		{
			if (object->bFriendly)
			{
				cHitbox sAOE = object->Hitbox(camera->GetOffsetX(), camera->GetOffsetY());

				// debug AOE
				//sAOE.Draw(this, olc::RED);

				// Check if an ennemy take the attack
				for (auto& dyn : vecEnnemies)
				{
					cHitbox sEnnemy = dyn->Hitbox(camera->GetOffsetX(), camera->GetOffsetY());

					// debug AOE
					//sEnnemy.Draw(this, olc::YELLOW);

					if (cHitbox::ShapeOverlap_DIAG(&sAOE, &sEnnemy))
					{
						if (dyn->bIsAttackable)
						{
							object->SoundEffect();
							player->Attack(dyn, object->nDamage);
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
					player->CheckIfDamaged(object, camera->GetOffsetX(), camera->GetOffsetY());
				}
			}
		}
	}

	for (auto& object : vecEnnemies)
	{
		object->Update(fElapsedTime, player->GetPlayerPosX(), player->GetPlayerPosY());
	}

	for (auto& object : vecProjectiles)
	{
		object->Update(fElapsedTime, player->GetPlayerPosX(), player->GetPlayerPosY());
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
		object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
	}

	// Draw Projectiles
	for (auto& object : vecProjectiles)
	{
		object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
	}

	if (bInBossLvl && vecEnnemies.empty() && !player->IsDead())
	{
		// Wait a little before begin win animation
		fWaitBeforeWinAnimation += fElapsedTime;

		if (fWaitBeforeWinAnimation >= cfTimeBeforeWinAnimation)
		{
			// do it only once
			if (!bBossKilled)
			{
				olc::SOUND::StopAll();
				olc::SOUND::PlaySample(sndBossKilled);
			}

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
			ReturnToWorldMap();

			return true;
		}
	}

	player->UpdateInvulnerability(fElapsedTime);
	t.Translate((player->GetPlayerPosX() - camera->GetOffsetX()) * nTileWidth + (nTileWidth / 2), (player->GetPlayerPosY() - camera->GetOffsetY()) * nTileHeight + (nTileHeight / 2));
	player->DrawKirbo(t);

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

	if (GetKey(olc::Key::ESCAPE).bPressed)
	{
		olc::SOUND::StopAll();
		nGameState = GS_SELECTMENU;
	}

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
			ReturnToWorldMap();
		else
			nGameState = GS_MAIN;
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
		sndBossLevelMusic = olc::SOUND::LoadAudioSample(bossLevelsMusics[nCurrentLevel]);
	}

	// Reset variables when level is loading
	ResetVariables();

	bInBossLvl = true;

	olc::SOUND::StopAll();
	olc::SOUND::PlaySample(sndBossLevelMusic, true);

	nGameState = GS_MAIN;

	return true;
}

bool OneLoneCoder_Platformer::GameState_SelectMenu(float fElapsedTime)
{
	selectMenu->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed)
	{
		if (selectMenu->GetPlayerChoice() == 0)
			ReturnToWorldMap();
		else if (selectMenu->GetPlayerChoice() == 1)
			nGameState = GS_CONTROLS;
		else
			nGameState = GS_CLOSE;
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

bool OneLoneCoder_Platformer::GameState_Close(float fElapsedTime)
{
	olc::SOUND::StopAll();
	olc::SOUND::DestroyAudio();
	olc_Terminate();

	return true;
}

void OneLoneCoder_Platformer::LoadLevelProperties()
{
	player->SetPlayerPosX(level->GetInitPlayerPosX());
	player->SetPlayerPosY(level->GetInitPlayerPosY());
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
	camera->SetShake(false);
	WindEffect(0.0f, 0.0f, false);
	fStopTimebeforeDeadAnim = 0.0f;
	fWaitBeforeWinAnimation = 0.0f;
	fWinTimer = 0.0f;
}

void OneLoneCoder_Platformer::ActivateShakeEffect(bool activate, int shakeAmplitudeX, int shakeAmplitudeY)
{
	camera->ActivateShakeEffect(activate, shakeAmplitudeX, shakeAmplitudeY);
}

void OneLoneCoder_Platformer::WindEffect(float direction, float windPower, bool activate)
{
	bWind = activate;
	fWindDirection = direction;
	fWindPower = windPower;

	if (bWind)
	{
		if (!olc::SOUND::IsSamplePlaying(sndWind))
			olc::SOUND::PlaySample(sndWind, true);
	}
	else
	{
		olc::SOUND::StopSample(sndWind);
	}
}

void OneLoneCoder_Platformer::BreakLoop()
{
	bBreakLoop = true;
}

olc::Sprite* OneLoneCoder_Platformer::GetBackGround()
{
	return sprBackground;
}

olc::Sprite* OneLoneCoder_Platformer::GetTilesSprites()
{
	return spriteTiles;
}

olc::Sprite* OneLoneCoder_Platformer::GetGroundTiles()
{
	return sprGrdTiles;
}

olc::Sprite* OneLoneCoder_Platformer::GetDoorSpr()
{
	return sprDoor;
}

void OneLoneCoder_Platformer::ReturnToWorldMap()
{
	olc::SOUND::StopAll();
	olc::SOUND::PlaySample(sndWorldMap, true);
	WindEffect(0.0f, 0.0f, false);
	animPlayer.ChangeState("riding_star");
	nGameState = GS_WORLDMAP;
}

void OneLoneCoder_Platformer::AddSharedSound(std::string name, int sound, std::string fileName)
{
	sound = olc::SOUND::LoadAudioSample(fileName);
	sharedSounds[name] = sound;
}

int OneLoneCoder_Platformer::GetSound(std::string name)
{
	return sharedSounds[name];
}
