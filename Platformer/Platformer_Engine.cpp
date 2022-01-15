#include "Platformer_Engine.h"

OneLoneCoder_Platformer::OneLoneCoder_Platformer()
{
	sAppName = "Kirbo Superstar";
}

bool OneLoneCoder_Platformer::OnUserCreate()
{
	controller.Initialize();
	olc::SOUND::InitialiseAudio();
	return true;
}

bool OneLoneCoder_Platformer::OnUserUpdate(float fElapsedTime)
{
	if (fElapsedTime <= 0.15f)
	{
		controller.Update(fElapsedTime);

		switch (nGameState)
		{
			case GS_SPLASHSCREEN:	GameState_SplashScreen(fElapsedTime);  break;
			case GS_LOADING:		GameState_Loading(fElapsedTime);	   break;
			case GS_TITLE:			GameState_Title(fElapsedTime);		   break;
			case GS_MAIN:			GameState_Main(fElapsedTime);		   break;
			case GS_TRANSITION:		GameState_Transition(fElapsedTime);    break;
			case GS_LOADLEVEL:		GameState_LoadLevel(fElapsedTime);     break;
			case GS_WORLDMAP:		GameState_WorldMap(fElapsedTime);	   break;
			case GS_ENDSCREEN:		GameState_EndScreen(fElapsedTime);	   break;
			case GS_PAUSE:			GameState_PauseMenu(fElapsedTime);	   break;
			case GS_LOADBOSSLEVEL:	GameState_LoadBossLevel(fElapsedTime); break;
			case GS_SELECTMENU:		GameState_SelectMenu(fElapsedTime);    break;
			case GS_CONTROLS:		GameState_Controls(fElapsedTime);	   break;
			case GS_CREDITS:		GameState_Credits(fElapsedTime);	   break;
			case GS_CLOSE:			GameState_Close(fElapsedTime);		   break;
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

	if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		nGameState = GS_MAIN;

	return false;
}

bool OneLoneCoder_Platformer::GameState_SplashScreen(float fElapsedTime)
{
	fSplashScreenTimer += fElapsedTime;

	DrawSprite(0, 0, new olc::Sprite("assets/gfx/OLCPixelGameEngineSplashScreen.png"));

	if (fSplashScreenTimer >= fSplashScreenTime || GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		nGameState = GS_LOADING;

	return false;
}

bool OneLoneCoder_Platformer::GameState_Loading(float fElapsedTime)
{
	srand(time(NULL));

	switch (nLoadingState)
	{
		case LS_CLEARSCREEN:
		{
			UpdateProgressBar("Loading 0%");

			nLoadingState = LS_LEVELS;
		}
		break;
		case LS_LEVELS:
		{
			level = new cLevel();
			cLevel::engine = this;
			sprDoor = new olc::Sprite("assets/gfx/door.png");

			level->LoadLevelsList();
			level->LoadBossLevelsList();
			level->LoadLevelsEnnemiesList();
			level->LoadLevelsPlatformsList();
			level->LoadLevelsTilesList();
			level->LoadLevelsGrdTilesList();
			level->LoadLevelsBackGroundList();
			level->LoadLevelsBossBckGrdList();
			level->LoadLevelsMusics();
			level->LoadBossLevelsMusics();

			level->InitialiseThreadPool();

			UpdateProgressBar("Loading 6%");

			nLoadingState = LS_ANIMATIONS;
		}
		break;
		case LS_ANIMATIONS:
		{
			animPlayer.LoadAnimations();

			UpdateProgressBar("Loading 12%");

			nLoadingState = LS_PROJECTILES;
		}
		break;
		case LS_PROJECTILES:
		{
			mapProjectiles = cDynamicProjectile::LoadProjectilesSprites();

			UpdateProgressBar("Loading 13%");

			nLoadingState = LS_PLATFORMS;
		}
		break;
		case LS_PLATFORMS:
		{
			mapPlatforms = cDynamicMovingPlatform::LoadMovingPlatformsSprites();
			sprDoorSwitchOff = new olc::Sprite("assets/gfx/doorSwitchOff.png");
			sprDoorSwitchOn = new olc::Sprite("assets/gfx/doorSwitchOn.png");

			UpdateProgressBar("Loading 15%");

			nLoadingState = LS_TITLE;
		}
		break;
		case LS_TITLE:
		{
			sprTitleScreen = new olc::Sprite("assets/gfx/title screen.png");
			titleScreen = new cTitleScreen(this, sprTitleScreen);

			UpdateProgressBar("Loading 18%");

			nLoadingState = LS_WORLDMAP;
		}
		break;
		case LS_WORLDMAP:
		{
			sprWorldMap = new olc::Sprite("assets/gfx/WorldMap.png");
			sprLock = new olc::Sprite("assets/gfx/Lock.png");
			worldMap = new cWorldMap(this, sprWorldMap, sprLock, &animPlayer);
			worldMap->SetUnlockedLevel(level->GetUnlockedLvl());

			UpdateProgressBar("Loading 23%");

			nLoadingState = LS_TRANSITION;
		}
		break;
		case LS_TRANSITION:
		{
			sprTransition = new olc::Sprite("assets/gfx/transitionScreen.png");
			transition = new cTransition(this, sprTransition, &animPlayer);

			UpdateProgressBar("Loading 30%");

			nLoadingState = LS_ENDSCREEN;
		}
		break;
		case LS_ENDSCREEN:
		{
			sprEndScreen = new olc::Sprite("assets/gfx/endScreen.png");
			endScreen = new cEndScreen(this, sprEndScreen);

			UpdateProgressBar("Loading 40%");

			nLoadingState = LS_PAUSEMENU;
		}
		break;
		case LS_PAUSEMENU:
		{
			sprPauseMenu = new olc::Sprite("assets/gfx/PauseMenu.png");
			sprCursor = new olc::Sprite("assets/gfx/cursor.png");
			pauseMenu = new cPauseMenu(this, sprPauseMenu, sprCursor);

			UpdateProgressBar("Loading 46%");

			nLoadingState = LS_SELECTMENU;
		}
		break;
		case LS_SELECTMENU:
		{
			sprSelectMenu = new olc::Sprite("assets/gfx/SelectMenu.png");
			selectMenu = new cSelectMenu(this, sprSelectMenu, sprCursor);

			UpdateProgressBar("Loading 51%");

			nLoadingState = LS_CONTROLSMENU;
		}
		break;
		case LS_CONTROLSMENU:
		{
			sprControlsMenu = new olc::Sprite("assets/gfx/ControlsMenu.png");
			controlsMenu = new cControlsMenu(this, sprControlsMenu);

			UpdateProgressBar("Loading 52%");

			nLoadingState = LS_CREDITSMENU;
		}
		break;
		case LS_CREDITSMENU:
		{
			sprCreditsMenu = new olc::Sprite("assets/gfx/CreditsMenu.png");
			creditsMenu = new cCreditsMenu(this, sprCreditsMenu);

			UpdateProgressBar("Loading 58%");

			nLoadingState = LS_HUD;
		}
		break;
		case LS_HUD:
		{
			HUD = new cHUD();
			sprHealthBar = new olc::Sprite("assets/gfx/emptyHealthBar.png");
			sprHealthPoint = new olc::Sprite("assets/gfx/healthPoint.png");
			sprBossHealthBar = new olc::Sprite("assets/gfx/emptyBossHealthBar.png");

			UpdateProgressBar("Loading 66%");

			nLoadingState = LS_ENGINEPOINTERS;
		}
		break;
		case LS_ENGINEPOINTERS:
		{
			cCamera::engine = this;
			cDynamicCreature::engine = this;
			cDynamicCreatureBladeKnight::engine = this;
			cDynamicCreatureBomber::engine = this;
			cDynamicCreatureFrosty::engine = this;
			cDynamicCreatureKingDDD::engine = this;
			cDynamicCreatureKracko::engine = this;
			cDynamicCreatureMrShineMrBright::engine = this;
			cDynamicCreatureRocky::engine = this;
			cDynamicCreatureSSTierMetaKnight::engine = this;
			cDynamicCreatureWaddleDee::engine = this;
			cDynamicCreatureWhispyWood::engine = this;
			cDynamicMovingPlatform::engine = this;
			cDynamicProjectile::engine = this;
			cDynamicWall::engine = this;
			cItemCandy::engine = this;
			cItemDamage::engine = this;
			cItemDefense::engine = this;
			cItemTomato::engine = this;
			cPlayer::engine = this;

			UpdateProgressBar("Loading 74%");

			nLoadingState = LS_PLAYER;
		}
		break;
		case LS_PLAYER:
		{
			player = new cPlayer(&animPlayer);

			UpdateProgressBar("Loading 78%");

			nLoadingState = LS_CAMERA;
		}
		break;
		case LS_CAMERA:
		{
			camera = new cCamera();
			camera->InitialiseThreadPool();

			UpdateProgressBar("Loading 95%");

			nLoadingState = LS_SOUNDS;
		}
		break;
		case LS_SOUNDS:
		{
			sndTitleScreen = olc::SOUND::LoadAudioSample("assets/snd/titleScreen.wav");
			sndWorldMap = olc::SOUND::LoadAudioSample("assets/snd/worldMap.wav");
			sndBossKilled = olc::SOUND::LoadAudioSample("assets/snd/bossKilled.wav");
			sndWind = olc::SOUND::LoadAudioSample("assets/snd/wind.wav");
			sndInvincibility = olc::SOUND::LoadAudioSample("assets/snd/invincibility.wav");

			AddSharedSound("whispyScream", sndWhispyScream, "assets/snd/whispyScream.wav");
			AddSharedSound("loseLife", sndLoseLife, "assets/snd/loseLife.wav");
			AddSharedSound("punch", sndPunch, "assets/snd/punch.wav");
			AddSharedSound("slap", sndSlap, "assets/snd/slap.wav");
			AddSharedSound("kirboWalk", sndKirboWalk, "assets/snd/kirboWalk.wav");
			AddSharedSound("kirboFly", sndKirboFly, "assets/snd/kirboFly.wav");
			AddSharedSound("explosion", sndExplosion, "assets/snd/explosion.wav");
			AddSharedSound("kirboJump", sndKirboJump, "assets/snd/kirboJump.wav");
			AddSharedSound("kirboHit", sndKirboHit, "assets/snd/kirboHit.wav");
			AddSharedSound("beginVacuum", sndKirboBeginVacuum, "assets/snd/kirboBeginVacuum.wav");
			AddSharedSound("vacuum", sndKirboVacuum, "assets/snd/kirboVacuum.wav");
			AddSharedSound("swallow", sndKirboSwallow, "assets/snd/kirboSwallow.wav");
			AddSharedSound("wetSlap", sndWetSlap, "assets/snd/wetSlap.wav");
			AddSharedSound("earthQuake", sndEarthQuake, "assets/snd/earthQuake.wav");
			AddSharedSound("blow", sndBlow, "assets/snd/blow.wav");
			AddSharedSound("inhale", sndInhale, "assets/snd/inhale.wav");
			AddSharedSound("poyo01", sndPoyo01, "assets/snd/poyo01.wav");
			AddSharedSound("poyo02", sndPoyo02, "assets/snd/poyo02.wav");
			AddSharedSound("sunShootingMoon", sndSunShootingMoon, "assets/snd/sunShootingMoon.wav");
			AddSharedSound("sunShootUp", sndSunShootUp, "assets/snd/sunShootUp.wav");
			AddSharedSound("beamCharge", sndBeamCharge, "assets/snd/beamCharge.wav");
			AddSharedSound("beam", sndBeam, "assets/snd/beam.wav");
			AddSharedSound("volcanoBoom", sndVolcanoBoom, "assets/snd/volcanoBoom.wav");
			AddSharedSound("iceBlow", sndIceBlow, "assets/snd/iceBlow.wav");
			AddSharedSound("patpat", sndpatpat, "assets/snd/patpat.wav");
			AddSharedSound("itemLaunch", sndItemLaunch, "assets/snd/itemLaunch.wav");
			AddSharedSound("superstar", sndSuperstar, "assets/snd/superstar.wav");
			AddSharedSound("frostyJump", sndFrostyJump, "assets/snd/frostyJump.wav");
			AddSharedSound("frostyGroundPound", sndFrostyGroundPound, "assets/snd/frostyGroundPound.wav");
			AddSharedSound("lightning", sndLightning, "assets/snd/lightning.wav");
			AddSharedSound("electricity", sndElectricity, "assets/snd/electricity.wav");
			AddSharedSound("electricity2", sndElectricity2, "assets/snd/electricity2.wav");
			AddSharedSound("meleeControllerRape", sndMeleeControllerRape, "assets/snd/meleeControllerRape.wav");
			AddSharedSound("dash", sndDash, "assets/snd/dash.wav");
			AddSharedSound("hiyayayaya", sndHiyayayaya, "assets/snd/hiyayayaya.wav");
			AddSharedSound("singleSwordStrike", sndSingleSwordStrike, "assets/snd/singleSwordStrike.wav");
			AddSharedSound("multipleSwordStrike", sndMultipleSwordStrike, "assets/snd/multipleSwordStrike.wav");
			AddSharedSound("ha", sndHa, "assets/snd/ha.wav");
			AddSharedSound("tornado", sndTornado, "assets/snd/tornado.wav");
			AddSharedSound("loudScreaming", sndLoudScreaming, "assets/snd/loudScreaming.wav");
			AddSharedSound("behold", sndBehold, "assets/snd/behold.wav");
			AddSharedSound("kingDDDJump", sndKingDDDJump, "assets/snd/kingDDDJump.wav");
			AddSharedSound("kingDDDUpSmash", sndKingDDDUpSmash, "assets/snd/kingDDDUpSmash.wav");
			AddSharedSound("kingDDDUpSmashHammer", sndKingDDDUpSmashHammer, "assets/snd/kingDDDUpSmashHammer.wav");
			AddSharedSound("kingDDDSideSmash", sndKingDDDSideSmash, "assets/snd/kingDDDSideSmash.wav");
			AddSharedSound("kingDDDDownSmashHammer", sndKingDDDDownSmashHammer, "assets/snd/kingDDDDownSmashHammer.wav");
			AddSharedSound("kingDDDSideB", sndKingDDDSideB, "assets/snd/kingDDDSideB.wav");
			AddSharedSound("spikeBallBounce", sndSpikeBallBounce, "assets/snd/spikeBallBounce.wav");
			AddSharedSound("kingDDDDownBCharge", sndKingDDDDownBCharge, "assets/snd/kingDDDDownBCharge.wav");
			AddSharedSound("kingDDDDownBPrepare", sndKingDDDDownBPrepare, "assets/snd/kingDDDDownBPrepare.wav");
			AddSharedSound("kingDDDDownBAttack", sndKingDDDDownBAttack, "assets/snd/kingDDDDownBAttack.wav");
			AddSharedSound("kingDDDUpBJump", sndKingDDDUpBJump, "assets/snd/kingDDDUpBJump.wav");
			AddSharedSound("kingDDDUpBLanding", sndKingDDDUpBLanding, "assets/snd/kingDDDUpBLanding.wav");
			AddSharedSound("kingDDDVacuum", sndKingDDDVacuum, "assets/snd/kingDDDVacuum.wav");
			AddSharedSound("kingDDDUpAir", sndKingDDDUpAir, "assets/snd/kingDDDUpAir.wav");
			AddSharedSound("kingDDDAirAtkVoice", sndKingDDDAirAtkVoice, "assets/snd/kingDDDAirAtkVoice.wav");
			AddSharedSound("kingDDDAirAtkHammer", sndKingDDDAirAtkHammer, "assets/snd/kingDDDAirAtkHammer.wav");
			AddSharedSound("itemPicked", sndItemPicked, "assets/snd/itemPicked.wav");
			AddSharedSound("rockyFall", sndRockyFall, "assets/snd/rockyFall.wav");

			UpdateProgressBar("Loading 99.9999999999999");

			nLoadingState = LS_ASSETS;
		}
		break;
		case LS_ASSETS:
		{
			cAssets::get().LoadSprites();

			bLoadFinished = true;
		}
		break;
	}

	if (bLoadFinished)
		nGameState = GS_TITLE;

	return true;
}

bool OneLoneCoder_Platformer::GameState_LoadLevel(float fElapsedTime)
{
	// Destroy all dynamics
	vecEnnemies.clear();
	vecProjectiles.clear();
	vecPlatforms.clear();

	level->SetCurrentLvl(worldMap->GetSelectedLevel());
	if (level->LoadLevel(level->GetLevelName()))
	{
		LoadLevelProperties();

		level->PopulateEnnemies(vecEnnemies, level->GetLevelsEnnemiesName());
		level->PopulatePlatforms(vecPlatforms, level->GetLevelsPlatformsName());

		spriteTiles = new olc::Sprite(level->GetLevelsTilesName());
		sprGrdTiles = new olc::Sprite(level->GetLevelsGrdTilesName());
		sprBackground = new olc::Sprite(level->GetLevelsBackGroundName());
		sndLevelMusic = olc::SOUND::LoadAudioSample(level->GetLevelsMusicsName());
	}

	// Reset variables when level is loading
	player->InitialiseKirboHealth();
	ResetVariables();

	transitionAnim = rand() % 4;

	bInBossLvl = false;

	olc::SOUND::StopAll();
	olc::SOUND::PlaySample(sndLevelMusic, true);

	nGameState = GS_TRANSITION;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Title(float fElapsedTime)
{
	if (!olc::SOUND::IsSamplePlaying(sndTitleScreen))
		olc::SOUND::PlaySample(sndTitleScreen, true);

	titleScreen->Update(this, fElapsedTime);

	if (GetAnyKey() || controller.AnyButtonPressed())
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
		olc::SOUND::StopSample(sndInvincibility);

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

	for (auto& object : vecPlatforms)
	{
		object->Update(fElapsedTime, player->GetPlayerPosX(), player->GetPlayerPosY());
	}

	player->Collisions(fElapsedTime, level);

	camera->SetPositions(player->GetPlayerPosX(), player->GetPlayerPosY());

	camera->DrawLevel(level, fElapsedTime);

	// Ennemies
	for (auto& object : vecEnnemies)
	{
		object->Collision(fElapsedTime);

		// Check if the ennemi is in the vacuum
		if (object->IsVacuumable())
		{
			if (player->IsVacuuming())
			{
				player->Vacuum(object, camera->GetOffsetX(), camera->GetOffsetY());
			}
			else
			{
				object->Vacuumed(false);
				object->SetSwallowable(false);
			}
		}

		// Check collision with player to damage him
		if ((player->IsAttackable() && !player->IsSwallowing() && !object->IsVacuumed()) || player->HasCandyPower())
		{
			player->CheckKirboCollisionWithEnemy(object, camera->GetOffsetX(), camera->GetOffsetY());
		}
	}

	// Projectiles
	for (auto& object : vecProjectiles)
	{
		object->Collision(fElapsedTime, level);

		// check if a Projectile hits a creature
		if (!object->IsRedundant())
		{
			if (object->IsFriendly())
			{
				object->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());

				// Check if an ennemy take the attack
				for (auto& dyn : vecEnnemies)
				{
					dyn->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());

					if (cHitbox::ShapeOverlap_DIAG(object->GetHitbox(), dyn->GetHitbox()))
					{
						if (dyn->IsAttackable())
						{
							object->PlaySoundEffect();
							player->Attack(dyn, object->GetDamage());
							if (object->IsOneHit())
								object->SetRedundant(true);
						}
					}
				}
			}
			else
			{
				if (player->IsAttackable())
				{
					player->CheckKirboCollisionWithEnemy(object, camera->GetOffsetX(), camera->GetOffsetY());
				}
			}
		}
	}

	for (auto& object : vecPlatforms)
	{
		object->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());
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
		return ((cDynamicCreature*)d)->IsDead();
	}), vecEnnemies.end());

	// Remove swallowed ennemies
	if (player->IsSwallowing())
	{
		vecEnnemies.erase(remove_if(vecEnnemies.begin(), vecEnnemies.end(), [](const cDynamicCreature* d)
		{
			return ((cDynamicCreature*)d)->IsSwallowable();
		}), vecEnnemies.end());
	}

	// Erase and delete redundant projectiles
	vecProjectiles.erase(remove_if(vecProjectiles.begin(), vecProjectiles.end(), [](const cDynamic* d)
	{
		return ((cDynamicProjectile*)d)->IsRedundant();
	}), vecProjectiles.end());

	for (auto& object : vecPlatforms)
	{
		object->DrawSwitch(camera->GetOffsetX(), camera->GetOffsetY());
	}

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

	// Draw Platforms
	for (auto& object : GetClosePlatforms(player->GetPlayerPosX(), player->GetPlayerPosY()))
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
			if (level->IsLastOfGame())
			{
				nGameState = GS_ENDSCREEN;

				return true;
			}
			else if (level->IsLastUnlocked())
			{
				level->UnlockNewLvl();

				worldMap->SetUnlockedLevel(level->GetUnlockedLvl());
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
	if (player->HasDamageBooster())
		HUD->DamageBoost(this, GetTilesSprites());
	if (player->HasDefenseBooster())
		HUD->DefenseBoost(this, GetTilesSprites());

#pragma endregion

	return true;
}

bool OneLoneCoder_Platformer::GameState_WorldMap(float fElapsedTime)
{
	worldMap->Update(this, fElapsedTime, GetController());

	if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		nGameState = GS_LOADLEVEL;

	if (GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(B).bPressed)
	{
		olc::SOUND::StopAll();
		nGameState = GS_SELECTMENU;
	}

	return false;
}

bool OneLoneCoder_Platformer::GameState_EndScreen(float fElapsedTime)
{
	endScreen->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		nGameState = GS_TITLE;

	return false;
}

bool OneLoneCoder_Platformer::GameState_PauseMenu(float fElapsedTime)
{
	pauseMenu->Update(this, fElapsedTime, GetController());

	if (GetKey(olc::Key::P).bPressed || controller.GetButton(START).bPressed)
	{
		nGameState = GS_MAIN;
	}
	else if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
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
	vecPlatforms.clear();

	if (level->LoadLevel(level->GetBossLevelName()))
	{
		LoadLevelProperties();

		level->PopulateBoss(vecEnnemies);

		sprBackground = new olc::Sprite(level->GetLevelsBossBckGrdName());
		sndBossLevelMusic = olc::SOUND::LoadAudioSample(level->GetBossLevelsMusicsName());
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
	selectMenu->Update(this, fElapsedTime, GetController());

	if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
	{
		if (selectMenu->GetPlayerChoice() == 0)
			ReturnToWorldMap();
		else if (selectMenu->GetPlayerChoice() == 1)
			nGameState = GS_CONTROLS;
		else if (selectMenu->GetPlayerChoice() == 2)
			nGameState = GS_CREDITS;
		else
			nGameState = GS_CLOSE;
	}
	return true;
}

bool OneLoneCoder_Platformer::GameState_Controls(float fElapsedTime)
{
	controlsMenu->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(A).bPressed || controller.GetButton(B).bPressed)
		nGameState = GS_SELECTMENU;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Credits(float fElapsedTime)
{
	creditsMenu->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(A).bPressed || controller.GetButton(B).bPressed)
		nGameState = GS_SELECTMENU;

	return true;
}

bool OneLoneCoder_Platformer::GameState_Close(float fElapsedTime)
{
	// Destroy all dynamics
	vecEnnemies.clear();
	vecProjectiles.clear();
	vecPlatforms.clear();

	// Stop Audio
	olc::SOUND::StopAll();
	olc::SOUND::DestroyAudio();

	// End the olc pixel game Engine
	olc_Terminate();

	return true;
}

void OneLoneCoder_Platformer::LoadLevelProperties()
{
	player->SetPlayerPosX(level->GetInitPlayerPosX());
	player->SetPlayerPosY(level->GetInitPlayerPosY());
}

bool OneLoneCoder_Platformer::GetAnyKey()
{
	return
		GetKey(olc::Key::NONE).bPressed ||

		GetKey(olc::Key::A).bPressed || GetKey(olc::Key::B).bPressed || GetKey(olc::Key::C).bPressed || GetKey(olc::Key::D).bPressed ||
		GetKey(olc::Key::E).bPressed || GetKey(olc::Key::F).bPressed || GetKey(olc::Key::G).bPressed || GetKey(olc::Key::H).bPressed ||
		GetKey(olc::Key::I).bPressed || GetKey(olc::Key::J).bPressed || GetKey(olc::Key::K).bPressed || GetKey(olc::Key::L).bPressed ||
		GetKey(olc::Key::M).bPressed || GetKey(olc::Key::N).bPressed || GetKey(olc::Key::O).bPressed || GetKey(olc::Key::P).bPressed ||
		GetKey(olc::Key::Q).bPressed || GetKey(olc::Key::R).bPressed || GetKey(olc::Key::S).bPressed || GetKey(olc::Key::T).bPressed ||
		GetKey(olc::Key::U).bPressed || GetKey(olc::Key::V).bPressed || GetKey(olc::Key::W).bPressed || GetKey(olc::Key::X).bPressed ||
		GetKey(olc::Key::Y).bPressed || GetKey(olc::Key::Z).bPressed ||

		GetKey(olc::Key::K0).bPressed || GetKey(olc::Key::K1).bPressed || GetKey(olc::Key::K2).bPressed || GetKey(olc::Key::K3).bPressed ||
		GetKey(olc::Key::K4).bPressed || GetKey(olc::Key::K5).bPressed || GetKey(olc::Key::K6).bPressed || GetKey(olc::Key::K7).bPressed ||
		GetKey(olc::Key::K8).bPressed || GetKey(olc::Key::K9).bPressed ||

		GetKey(olc::Key::F1).bPressed || GetKey(olc::Key::F2).bPressed || GetKey(olc::Key::F3).bPressed || GetKey(olc::Key::F4).bPressed ||
		GetKey(olc::Key::F5).bPressed || GetKey(olc::Key::F6).bPressed || GetKey(olc::Key::F7).bPressed || GetKey(olc::Key::F8).bPressed ||
		GetKey(olc::Key::F9).bPressed || GetKey(olc::Key::F10).bPressed || GetKey(olc::Key::F11).bPressed || GetKey(olc::Key::F12).bPressed ||

		GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::RIGHT).bPressed ||

		GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::TAB).bPressed || GetKey(olc::Key::SHIFT).bPressed || GetKey(olc::Key::CTRL).bPressed ||
		GetKey(olc::Key::INS).bPressed || GetKey(olc::Key::DEL).bPressed || GetKey(olc::Key::HOME).bPressed || GetKey(olc::Key::END).bPressed ||
		GetKey(olc::Key::PGUP).bPressed || GetKey(olc::Key::PGDN).bPressed || GetKey(olc::Key::BACK).bPressed || GetKey(olc::Key::ESCAPE).bPressed ||
		GetKey(olc::Key::RETURN).bPressed || GetKey(olc::Key::ENTER).bPressed || GetKey(olc::Key::PAUSE).bPressed || GetKey(olc::Key::SCROLL).bPressed ||

		GetKey(olc::Key::NP0).bPressed || GetKey(olc::Key::NP1).bPressed || GetKey(olc::Key::NP2).bPressed || GetKey(olc::Key::NP3).bPressed ||
		GetKey(olc::Key::NP4).bPressed || GetKey(olc::Key::NP5).bPressed || GetKey(olc::Key::NP6).bPressed || GetKey(olc::Key::NP7).bPressed ||
		GetKey(olc::Key::NP8).bPressed || GetKey(olc::Key::NP9).bPressed || GetKey(olc::Key::NP_MUL).bPressed ||

		GetKey(olc::Key::NP_DIV).bPressed || GetKey(olc::Key::NP_ADD).bPressed || GetKey(olc::Key::NP_SUB).bPressed || GetKey(olc::Key::NP_DECIMAL).bPressed ||

		GetKey(olc::Key::PERIOD).bPressed || GetKey(olc::Key::EQUALS).bPressed || GetKey(olc::Key::COMMA).bPressed || GetKey(olc::Key::MINUS).bPressed ||

		GetKey(olc::Key::OEM_1).bPressed || GetKey(olc::Key::OEM_2).bPressed || GetKey(olc::Key::OEM_3).bPressed || GetKey(olc::Key::OEM_4).bPressed ||
		GetKey(olc::Key::OEM_5).bPressed || GetKey(olc::Key::OEM_6).bPressed || GetKey(olc::Key::OEM_7).bPressed || GetKey(olc::Key::OEM_8).bPressed ||

		GetKey(olc::Key::CAPS_LOCK).bPressed || GetKey(olc::Key::ENUM_END).bPressed;
}

bool OneLoneCoder_Platformer::IsSolidTile(wchar_t tile)
{
	// List Here all the tiles that are solid
	return tile == '#' || tile == 'G' || tile == 'B';
}

bool OneLoneCoder_Platformer::IsSemiSolidTile(wchar_t tile)
{
	// List Here all the tiles that are semi solid
	return tile == '_';
}

void OneLoneCoder_Platformer::AddProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner, bool breackableAgainstTiles, float fDrag, std::string sound, bool bouncy, std::string bounceSound)
{
	cDynamicProjectile* p = new cDynamicProjectile(ox, oy, bFriend, velx, vely, duration, mapProjectiles[sprite], affectedByGravity, damage, solidVSMap, oneHit, corner, breackableAgainstTiles, fDrag, bouncy, bounceSound);
	if (sound != "")
		p->SetSoundEffect(sound);
	vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit, int corner, std::string sound)
{
	cDynamicProjectile* p = new cDynamicProjectileBoomerang(ox, oy, bFriend, velx, vely, duration, mapProjectiles[sprite], damage, solidVSMap, oneHit, corner);
	if (sound != "")
		p->SetSoundEffect(sound);
	vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddOrbital(float ox, float oy, bool bFriend, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit, int corner, float cx, float cy, float angrot, std::string sound)
{
	cDynamicProjectile* p = new cDynamicProjectileOrbital(ox, oy, bFriend, duration, mapProjectiles[sprite], damage, solidVSMap, oneHit, corner, cx, cy, angrot);
	if (sound != "")
		p->SetSoundEffect(sound);
	vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddPlatform(float ox, float oy, std::string sprite)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatform(ox, oy, mapPlatforms[sprite]);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalSinePtfm(float ox, float oy, std::string sprite, float amplitude, float frequency, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformHorSine(ox, oy, mapPlatforms[sprite], amplitude, frequency, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalSinePtfm(float ox, float oy, std::string sprite, float amplitude, float frequency, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformVerSine(ox, oy, mapPlatforms[sprite], amplitude, frequency, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalSinglePtfm(float ox, float oy, std::string sprite, float tx, float vx, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformHorSingle(ox, oy, mapPlatforms[sprite], tx, vx, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalSinglePtfm(float ox, float oy, std::string sprite, float ty, float vy, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformVerSingle(ox, oy, mapPlatforms[sprite], ty, vy, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddWall(float ox, float oy, std::string sprite, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY)
{
	cDynamicMovingPlatform* ptfm = new cDynamicWall(ox, oy, mapPlatforms[sprite], leftSolid, rightSolid, trigger, trgX, trgY);
	vecPlatforms.push_back(ptfm);
}

std::vector<cDynamicMovingPlatform*> OneLoneCoder_Platformer::GetClosePlatforms(float px, float py)
{
	std::vector<cDynamicMovingPlatform*> closePtfms;
	for (auto& ptfm : vecPlatforms)
	{
		if (fabs(px - (ptfm->GetPX())) < (((float)ScreenWidth()) / 64.0f) && fabs(py - (ptfm->GetPY())) < (((float)ScreenHeight()) / 64.0f))
		{
			closePtfms.push_back(ptfm);
		}
	}
	return closePtfms;
}

olc::Sprite* OneLoneCoder_Platformer::GetDoorSwitch(bool on)
{
	return on ? sprDoorSwitchOn : sprDoorSwitchOff;
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
	else if (gameState == "GS_CREDITS")			nGameState = GS_CREDITS;
}

float OneLoneCoder_Platformer::GetDragValue()
{
	return cfDrag;
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
	player->SetDamageBooster(1);
	player->SetDefenseBooster(1);
}

void OneLoneCoder_Platformer::SetPlayerChoice(int choice)
{
	pauseMenu->SetPlayerChoice(choice);
}

cCamera* OneLoneCoder_Platformer::GetCamera()
{
	return camera;
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

void OneLoneCoder_Platformer::AddSharedSound(std::string name, int sound, std::string fileName)
{
	sound = olc::SOUND::LoadAudioSample(fileName);
	sharedSounds[name] = sound;
}

void OneLoneCoder_Platformer::PlaySample(std::string name, bool loop, bool dontPlayIfAlreadyPlaying)
{
	if (dontPlayIfAlreadyPlaying)
	{
		if (!olc::SOUND::IsSamplePlaying(sharedSounds[name]))
			olc::SOUND::PlaySample(sharedSounds[name]);
	}
	else
	{
		olc::SOUND::PlaySample(sharedSounds[name]);
	}
}

void OneLoneCoder_Platformer::StopSample(std::string name)
{
	if (olc::SOUND::IsSamplePlaying(sharedSounds[name]))
		olc::SOUND::StopSample(sharedSounds[name]);
}

bool OneLoneCoder_Platformer::IsSamplePlaying(std::string name)
{
	return olc::SOUND::IsSamplePlaying(sharedSounds[name]);
}

void OneLoneCoder_Platformer::PlayLevelMusic()
{
	olc::SOUND::PlaySample(sndLevelMusic, true);
}

void OneLoneCoder_Platformer::StopLevelMusic()
{
	olc::SOUND::StopSample(sndLevelMusic);
}

void OneLoneCoder_Platformer::ChangeKirboVelocities(float vx, float vy)
{
	player->SetVelocities(vx, vy);
}

void OneLoneCoder_Platformer::SetKirboGrabbed(bool grabbed)
{
	player->SetGrabbedByEnnemy(grabbed);
}

void OneLoneCoder_Platformer::ChangeKirboAnimation(std::string animation)
{
	player->ChangeAnimation(animation);
}

void OneLoneCoder_Platformer::SetKirboVisible(bool visible)
{
	player->SetVisible(visible);
}

void OneLoneCoder_Platformer::SetKirboAttackable(bool attackable)
{
	player->SetAttackable(attackable);
}

void OneLoneCoder_Platformer::SetKirboPositions(float px, float py)
{
	player->SetPlayerPosX(px);
	player->SetPlayerPosY(py);
}

bool OneLoneCoder_Platformer::CheckIfKirboCollisionWithEnnemy(cDynamic* object)
{
	return player->CheckIfEnnemyCollision(object, camera->GetOffsetX(), camera->GetOffsetY());
}

bool OneLoneCoder_Platformer::IsKirboAttackable()
{
	return player->IsAttackable();
}

void OneLoneCoder_Platformer::HealPlayer()
{
	player->Heal();
}

void OneLoneCoder_Platformer::PlayerGetCandy(float candyTime)
{
	olc::SOUND::PlaySample(sndInvincibility);

	player->SetInvincible(candyTime);
	player->SetCandyPower(true);
}

void OneLoneCoder_Platformer::BuffPlayerDamage()
{
	player->SetDamageBooster(2);
}

void OneLoneCoder_Platformer::BuffPlayerDefense()
{
	player->SetDefenseBooster(2);
}

ControllerManager* OneLoneCoder_Platformer::GetController()
{
	return &controller;
}

void OneLoneCoder_Platformer::UpdateProgressBar(std::string loadPercent)
{
	Clear(olc::BLACK);
	DrawString(30, 440, loadPercent, olc::WHITE, 5);
}

std::string OneLoneCoder_Platformer::ToStr(std::wstring str)
{
	//setup converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(str);
}
