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
			case GS_SPLASHSCREEN:  GameState_SplashScreen(fElapsedTime);  break;
			case GS_LOADING:	   GameState_Loading(fElapsedTime);		  break;
			case GS_TITLE:		   GameState_Title(fElapsedTime);		  break;
			case GS_MAIN:		   GameState_Main(fElapsedTime);		  break;
			case GS_LEVELSTART:	   GameState_LevelStart(fElapsedTime);    break;
			case GS_LOADLEVEL:	   GameState_LoadLevel(fElapsedTime);     break;
			case GS_WORLDMAP:	   GameState_WorldMap(fElapsedTime);	  break;
			case GS_ENDSCREEN:	   GameState_EndScreen(fElapsedTime);	  break;
			case GS_PAUSE:		   GameState_PauseMenu(fElapsedTime);	  break;
			case GS_LOADBOSSLEVEL: GameState_LoadBossLevel(fElapsedTime); break;
			case GS_SELECTMENU:	   GameState_SelectMenu(fElapsedTime);    break;
			case GS_CONTROLS:	   GameState_Controls(fElapsedTime);	  break;
			case GS_CREDITS:	   GameState_Credits(fElapsedTime);		  break;
			case GS_CLOSE:		   GameState_Close(fElapsedTime);	      break;
			case GS_TRANSITION:	   GameState_Transition(fElapsedTime);    break;
		}
	}

	return true;
}

bool OneLoneCoder_Platformer::GameState_LevelStart(float fElapsedTime)
{
	levelStart->Update(this, fElapsedTime);

	switch (levelStartAnim)
	{
		case 0: animPlayer.ChangeState("idle");		   break;
		case 1: animPlayer.ChangeState("run");		   break;
		case 2: animPlayer.ChangeState("flying");	   break;
		case 3: animPlayer.ChangeState("riding_star"); break;
	}

	SetPixelMode(olc::Pixel::ALPHA);
	switch (worldMap->GetSelectedLevel())
	{
		case 0: DrawKirboString(50, 400, "green green", 2);		   break;
		case 1: DrawKirboString(50, 400, "grass land", 2);		   break;
		case 2: DrawKirboString(50, 400, "iceberg", 2);			   break;
		case 3: DrawKirboString(50, 400, "cloudy park", 2);		   break;
		case 4: DrawKirboString(50, 400, "halberd", 2);			   break;
		case 5: DrawKirboString(50, 400, "fountain of dreams", 2); break;
	}
	SetPixelMode(olc::Pixel::NORMAL);

	if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		TransitionTo("GS_MAIN", true);

	return false;
}

bool OneLoneCoder_Platformer::GameState_SplashScreen(float fElapsedTime)
{
	fSplashScreenTimer += fElapsedTime;

	DrawSprite(0, 0, new olc::Sprite("assets/gfx/OLCPixelGameEngineSplashScreen.png"));

	if (fSplashScreenTimer >= fSplashScreenTime || GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		TransitionTo("GS_LOADING", false);

	return false;
}

bool OneLoneCoder_Platformer::GameState_Loading(float fElapsedTime)
{
	srand(time(NULL));

	switch (nLoadingState)
	{
		case LS_CLEARSCREEN:
		{
			cAssets::get().LoadFont();
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
			level->LoadBossesList();
			level->LoadEnnemiesList();
			level->LoadMechanismsList();
			level->LoadSpecialTilesList();
			level->LoadGroundTilesList();
			level->LoadBackGroundsList();
			level->LoadBossesBackGroundsList();
			level->LoadMusicsList();
			level->LoadBossesMusicsList();

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

			nLoadingState = LS_MECHANISMS;
		}
		break;
		case LS_MECHANISMS:
		{
			mapPlatforms = cDynamicMovingPlatform::LoadMovingPlatformsSprites();
			sprDoorSwitchOff = new olc::Sprite("assets/gfx/doorSwitchOff.png");
			sprDoorSwitchOn = new olc::Sprite("assets/gfx/doorSwitchOn.png");
			mapWinds = cDynamicWind::LoadWindSprites();
			mapTeleports = cDynamicTeleport::LoadTeleportsSprites();

			UpdateProgressBar("Loading 16%");

			nLoadingState = LS_TITLE;
		}
		break;
		case LS_TITLE:
		{
			sprTitleScreen = new olc::Sprite("assets/gfx/title screen.png");
			titleScreen = new cTitleScreen(this, sprTitleScreen);

			UpdateProgressBar("Loading 20%");

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

			nLoadingState = LS_LEVELSTART;
		}
		break;
		case LS_LEVELSTART:
		{
			sprLevelStart = new olc::Sprite("assets/gfx/levelStartScreen.png");
			levelStart = new cLevelStart(this, sprLevelStart, &animPlayer);

			UpdateProgressBar("Loading 30%");

			nLoadingState = LS_ENDSCREEN;
		}
		break;
		case LS_ENDSCREEN:
		{
			sprEndScreen = new olc::Sprite("assets/gfx/endScreen.png");
			endScreen = new cEndScreen(this, sprEndScreen, &animPlayer);

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
			sprMenuBar = new olc::Sprite("assets/gfx/MenuBar.png");
			sprUnselQuit = new olc::Sprite("assets/gfx/unselQuit.png");
			sprSelQuit = new olc::Sprite("assets/gfx/selQuit.png");
			sprUnselDefault = new olc::Sprite("assets/gfx/unselDefault.png");
			sprSelDefault = new olc::Sprite("assets/gfx/selDefault.png");
			// Controller sprites
			sprX = new olc::Sprite("assets/gfx/XBtn.png");
			sprY = new olc::Sprite("assets/gfx/YBtn.png");
			sprB = new olc::Sprite("assets/gfx/BBtn.png");
			sprPause = new olc::Sprite("assets/gfx/PauseBtn.png");
			sprUp = new olc::Sprite("assets/gfx/UpBtn.png");
			sprDown = new olc::Sprite("assets/gfx/DownBtn.png");
			sprLeft = new olc::Sprite("assets/gfx/LeftBtn.png");
			sprRight = new olc::Sprite("assets/gfx/RightBtn.png");
			sprA = new olc::Sprite("assets/gfx/ABtn.png");
			sprRBump = new olc::Sprite("assets/gfx/RBumpBtn.png");

			controlsMenu = new cControlsMenu(this,
											 sprControlsMenu,
											 sprMenuBar,
											 sprUnselQuit,
											 sprSelQuit,
											 sprUnselDefault,
											 sprSelDefault,
											 sprX,
											 sprY,
											 sprB,
											 sprPause,
											 sprUp,
											 sprDown,
											 sprLeft,
											 sprRight,
											 sprA,
											 sprRBump);

			ApplyControls();

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
			cControlsMenu::engine = this;
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
			cDynamicHorizontalCrusher::engine = this;
			cDynamicMovingPlatform::engine = this;
			cDynamicProjectile::engine = this;
			cDynamicTeleport::engine = this;
			cDynamicVerticalCrusher::engine = this;
			cDynamicWall::engine = this;
			cDynamicWind::engine = this;
			cItemCandy::engine = this;
			cItemDamage::engine = this;
			cItemDefense::engine = this;
			cItemMinorHeal::engine = this;
			cItemTomato::engine = this;
			cPauseMenu::engine = this;
			cPlayer::engine = this;
			cSelectMenu::engine = this;

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
			sndMenu = olc::SOUND::LoadAudioSample("assets/snd/menus.wav");
			sndTransition = olc::SOUND::LoadAudioSample("assets/snd/transition.wav");
			sndEndScreen = olc::SOUND::LoadAudioSample("assets/snd/endScreen.wav");

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
			AddSharedSound("boom", sndBoom, "assets/snd/boom.wav");
			AddSharedSound("enterDoor", sndEnterDoor, "assets/snd/enterDoor.wav");
			AddSharedSound("menuBip", sndMenuBip, "assets/snd/menuBip.wav");
			AddSharedSound("gateOpening", sndGateOpening, "assets/snd/gateOpening.wav");
			AddSharedSound("gateOpened", sndGateOpened, "assets/snd/gateOpened.wav");

			UpdateProgressBar("Loading 99.99999999999999999999999999999");

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
		TransitionTo("GS_TITLE", true);

	return true;
}

bool OneLoneCoder_Platformer::GameState_LoadLevel(float fElapsedTime)
{
	DestroyAllDynamics();

	level->SetCurrentLvl(worldMap->GetSelectedLevel());
	if (level->LoadLevel(level->GetName()))
	{
		LoadLevelProperties();

		level->PopulateEnnemies(vecEnnemies, level->GetEnnemies());
		level->PopulateMechanisms(level->GetMechanisms());

		spriteTiles = new olc::Sprite(level->GetSpecialTiles());
		sprGrdTiles = new olc::Sprite(level->GetGroundTiles());
		sprBackground = new olc::Sprite(level->GetBackGround());
		sndLevelMusic = olc::SOUND::LoadAudioSample(level->GetMusic());
	}

	// Reset variables when level is loading
	player->InitialiseKirboHealth();
	ResetVariables();

	levelStartAnim = rand() % 4;

	bInBossLvl = false;

	olc::SOUND::StopAll();
	olc::SOUND::PlaySample(sndLevelMusic, true);

	// Permanent wind in Halberd level
	if (worldMap->GetSelectedLevel() == 4)
	{
		ActivateShakeEffect(true, 40, 40);
	}

	TransitionTo("GS_LEVELSTART", true);

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
		TransitionTo("GS_SELECTMENU", true);
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

	camera->ClampOffset();

	player->ApplyGravity(fElapsedTime);

	player->Update(fElapsedTime);

	// Initialize the Transform2D object
	olc::GFX2D::Transform2D t;
	t.Translate(((float)-nTileWidth / 2.0f) - cnSpriteOffsetX, ((float)-nTileWidth / 2.0f) - cnSpriteOffsetY);
	t.Scale(player->GetFaceDir() * 1.0f, 1.0f);

	player->OneCycleAnimations(fElapsedTime, &t, mapProjectiles, level);
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
		object->Update(fElapsedTime, player->GetPosX(), player->GetPosY());
	}

	if (!player->IsDead())
		player->Collisions(fElapsedTime, level);

	camera->SetPositions(player->GetPosX(), player->GetPosY());

	camera->CalculateFOV(level);
	camera->DrawBackground(level);

	player->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());

	camera->SpawnSceneries(level, fElapsedTime);

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
			player->EnemyCollision(object, camera->GetOffsetX(), camera->GetOffsetY());
		}
	}

	// Projectiles
	for (auto& object : vecProjectiles)
	{
		object->Collision(fElapsedTime, level);

		// check if a Projectile hits a creature
		if (!object->IsRedundant() && !object->IsScenery())
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
					player->EnemyCollision(object, camera->GetOffsetX(), camera->GetOffsetY());
				}
			}
		}
	}

	for (auto& object : vecPlatforms)
	{
		object->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());
	}

	for (auto& object : vecWinds)
	{
		object->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());
	}

	for (auto& object : vecTeleports)
	{
		object->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());
	}

	for (auto& object : vecTeleports)
	{
		object->UpdateDestHitbox(camera->GetOffsetX(), camera->GetOffsetY());
	}

	for (auto& object : vecEnnemies)
	{
		object->Update(fElapsedTime, player->GetPosX(), player->GetPosY());
	}

	for (auto& object : vecProjectiles)
	{
		object->Update(fElapsedTime, player->GetPosX(), player->GetPosY());
	}

	for (auto& object : vecWinds)
	{
		object->Update(fElapsedTime, player->GetPosX(), player->GetPosY());
	}

	for (auto& object : vecTeleports)
	{
		object->Update(fElapsedTime, player->GetPosX(), player->GetPosY());
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

	// Draw walls switches
	for (auto& object : vecPlatforms)
	{
		object->DrawSwitch(camera->GetOffsetX(), camera->GetOffsetY());
	}

	// Draw Teleports
	for (auto& object : GetCloseTeleport(player->GetPosX(), player->GetPosY()))
	{
		object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
	}
	for (auto& object : GetCloseTeleportDest(player->GetPosX(), player->GetPosY()))
	{
		object->DrawDest(camera->GetOffsetX(), camera->GetOffsetY());
	}

	// Draw Platforms
	for (auto& object : GetClosePlatforms(player->GetPosX(), player->GetPosY()))
	{
		object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
	}

	// Draw Wind
	for (auto& object : GetCloseWinds(player->GetPosX(), player->GetPosY()))
	{
		object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
	}

	camera->DrawLevel(level, fElapsedTime);

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
			if (level->IsLastOfGame())
			{
				TransitionTo("GS_ENDSCREEN", true);

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
	t.Translate((player->GetPosX() - camera->GetOffsetX()) * nTileWidth + (nTileWidth / 2), (player->GetPosY() - camera->GetOffsetY()) * nTileHeight + (nTileHeight / 2));
	player->DrawKirbo(t);

	// Don't move kirbo before the white fading is finished
	player->ChangePosAfterTP();

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
		nGameState = GS_LOADLEVEL; // This transition is not visible for the player, no white screen and sound

	if (GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(B).bPressed)
	{
		olc::SOUND::StopAll();
		TransitionTo("GS_SELECTMENU", true);
	}

	return false;
}

bool OneLoneCoder_Platformer::GameState_EndScreen(float fElapsedTime)
{
	endScreen->Update(this, fElapsedTime);

	animPlayer.ChangeState("joy");

	if (bPlayMusic)
	{
		bPlayMusic = false;
		olc::SOUND::PlaySample(sndEndScreen);
	}

	SetPixelMode(olc::Pixel::ALPHA);
	DrawKirboString(400, 400, "Thank you for playing", 2, true);
	DrawKirboString(400, 432, "Kirbo Superstar", 2, true);
	SetPixelMode(olc::Pixel::NORMAL);

	if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
	{
		olc::SOUND::StopAll();
		bPlayMusic = true;
		TransitionTo("GS_TITLE", true);
	}

	return false;
}

bool OneLoneCoder_Platformer::GameState_PauseMenu(float fElapsedTime)
{
	pauseMenu->Update(this, fElapsedTime, GetController());

	if (GetKey(olc::Key::P).bPressed || controller.GetButton(START).bPressed)
	{
		TransitionTo("GS_MAIN", true);
	}
	else if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
	{
		if (pauseMenu->GetPlayerChoice() == 1)
			ReturnToWorldMap();
		else
			TransitionTo("GS_MAIN", true);
	}

	return true;
}

bool OneLoneCoder_Platformer::GameState_LoadBossLevel(float fElapsedTime)
{
	DestroyAllDynamics();

	if (level->LoadLevel(level->GetBoss()))
	{
		LoadLevelProperties();

		level->PopulateBoss(vecEnnemies);

		sprBackground = new olc::Sprite(level->GetBossBackGround());
		sndBossLevelMusic = olc::SOUND::LoadAudioSample(level->GetBossMusic());
	}

	// Reset variables when level is loading
	ResetVariables();

	bInBossLvl = true;

	olc::SOUND::StopAll();
	olc::SOUND::PlaySample(sndBossLevelMusic, true);

	TransitionTo("GS_MAIN", false);

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
			GoToControlsMenu();
		else if (selectMenu->GetPlayerChoice() == 2)
			TransitionTo("GS_CREDITS", true);
		else
			nGameState = GS_CLOSE; // No need a transition when closing the game
	}
	return true;
}

bool OneLoneCoder_Platformer::GameState_Controls(float fElapsedTime)
{
	controlsMenu->Update(this, fElapsedTime, GetController());

	if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
	{
		if (controlsMenu->GetSelectedItem() == 11)
		{
			controlsMenu->UpdateSavedControls();
			controlsMenu->SetSelectedItem(0);
			ApplyControls();
			olc::SOUND::StopAll();
			TransitionTo("GS_SELECTMENU", true);
		}
	}

	return true;
}

bool OneLoneCoder_Platformer::GameState_Credits(float fElapsedTime)
{
	creditsMenu->Update(this, fElapsedTime);

	if (GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(A).bPressed || controller.GetButton(B).bPressed)
		TransitionTo("GS_SELECTMENU", false);

	return true;
}

bool OneLoneCoder_Platformer::GameState_Close(float fElapsedTime)
{
	DestroyAllDynamics();

	// Stop Audio
	olc::SOUND::StopAll();
	olc::SOUND::DestroyAudio();

	// End the olc pixel game Engine
	olc_Terminate();

	return true;
}

bool OneLoneCoder_Platformer::GameState_Transition(float fElapsedTime)
{
	if (bPlayTransitionSound)
	{
		olc::SOUND::PlaySample(sndTransition);
		bPlayTransitionSound = false;
	}

	fTransitionTimer += fElapsedTime;

	int transparency = (1 - ((fTransitionTime - fTransitionTimer) / fTransitionTime)) * 255;

	if (transparency >= 255)
		transparency = 255;

	FillRectDecal({ 0.0f , 0.0f }, { 800.0f , 480.0f }, olc::Pixel(255, 255, 255, transparency));

	if (fTransitionTimer >= fTransitionTime)
	{
		fTransitionTimer = 0.0f;
		SetGameState(sNextState);
	}

	return true;
}

void OneLoneCoder_Platformer::DestroyAllDynamics()
{
	vecEnnemies.clear();
	vecProjectiles.clear();
	vecPlatforms.clear();
	vecWinds.clear();
	vecTeleports.clear();
}

void OneLoneCoder_Platformer::LoadLevelProperties()
{
	SetKirboPositions(level->GetInitPlayerPosX(), level->GetInitPlayerPosY());
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

olc::Key OneLoneCoder_Platformer::GetFirstKeyPressed()
{
	if (GetKey(olc::Key::NONE).bPressed) return olc::Key::NONE;

	if (GetKey(olc::Key::A).bPressed) return olc::Key::A;
	if (GetKey(olc::Key::B).bPressed) return olc::Key::B;
	if (GetKey(olc::Key::C).bPressed) return olc::Key::C;
	if (GetKey(olc::Key::D).bPressed) return olc::Key::D;
	if (GetKey(olc::Key::E).bPressed) return olc::Key::E;
	if (GetKey(olc::Key::F).bPressed) return olc::Key::F;
	if (GetKey(olc::Key::G).bPressed) return olc::Key::G;
	if (GetKey(olc::Key::H).bPressed) return olc::Key::H;
	if (GetKey(olc::Key::I).bPressed) return olc::Key::I;
	if (GetKey(olc::Key::J).bPressed) return olc::Key::J;
	if (GetKey(olc::Key::K).bPressed) return olc::Key::K;
	if (GetKey(olc::Key::L).bPressed) return olc::Key::L;
	if (GetKey(olc::Key::M).bPressed) return olc::Key::M;
	if (GetKey(olc::Key::N).bPressed) return olc::Key::N;
	if (GetKey(olc::Key::O).bPressed) return olc::Key::O;
	if (GetKey(olc::Key::P).bPressed) return olc::Key::P;
	if (GetKey(olc::Key::Q).bPressed) return olc::Key::Q;
	if (GetKey(olc::Key::R).bPressed) return olc::Key::R;
	if (GetKey(olc::Key::S).bPressed) return olc::Key::S;
	if (GetKey(olc::Key::T).bPressed) return olc::Key::T;
	if (GetKey(olc::Key::U).bPressed) return olc::Key::U;
	if (GetKey(olc::Key::V).bPressed) return olc::Key::V;
	if (GetKey(olc::Key::W).bPressed) return olc::Key::W;
	if (GetKey(olc::Key::X).bPressed) return olc::Key::X;
	if (GetKey(olc::Key::Y).bPressed) return olc::Key::Y;
	if (GetKey(olc::Key::Z).bPressed) return olc::Key::Z;

	if (GetKey(olc::Key::K0).bPressed) return olc::Key::K0;
	if (GetKey(olc::Key::K1).bPressed) return olc::Key::K1;
	if (GetKey(olc::Key::K2).bPressed) return olc::Key::K2;
	if (GetKey(olc::Key::K3).bPressed) return olc::Key::K3;
	if (GetKey(olc::Key::K4).bPressed) return olc::Key::K4;
	if (GetKey(olc::Key::K5).bPressed) return olc::Key::K5;
	if (GetKey(olc::Key::K6).bPressed) return olc::Key::K6;
	if (GetKey(olc::Key::K7).bPressed) return olc::Key::K7;
	if (GetKey(olc::Key::K8).bPressed) return olc::Key::K8;
	if (GetKey(olc::Key::K9).bPressed) return olc::Key::K9;

	if (GetKey(olc::Key::F1).bPressed)  return olc::Key::F1;
	if (GetKey(olc::Key::F2).bPressed)  return olc::Key::F2;
	if (GetKey(olc::Key::F3).bPressed)  return olc::Key::F3;
	if (GetKey(olc::Key::F4).bPressed)  return olc::Key::F4;
	if (GetKey(olc::Key::F5).bPressed)  return olc::Key::F5;
	if (GetKey(olc::Key::F6).bPressed)  return olc::Key::F6;
	if (GetKey(olc::Key::F7).bPressed)  return olc::Key::F7;
	if (GetKey(olc::Key::F8).bPressed)  return olc::Key::F8;
	if (GetKey(olc::Key::F9).bPressed)  return olc::Key::F9;
	if (GetKey(olc::Key::F10).bPressed) return olc::Key::F10;
	if (GetKey(olc::Key::F11).bPressed) return olc::Key::F11;
	if (GetKey(olc::Key::F12).bPressed) return olc::Key::F12;

	if (GetKey(olc::Key::UP).bPressed)    return olc::Key::UP;
	if (GetKey(olc::Key::DOWN).bPressed)  return olc::Key::DOWN;
	if (GetKey(olc::Key::LEFT).bPressed)  return olc::Key::LEFT;
	if (GetKey(olc::Key::RIGHT).bPressed) return olc::Key::RIGHT;

	if (GetKey(olc::Key::SPACE).bPressed)  return olc::Key::SPACE;
	if (GetKey(olc::Key::TAB).bPressed)    return olc::Key::TAB;
	if (GetKey(olc::Key::SHIFT).bPressed)  return olc::Key::SHIFT;
	if (GetKey(olc::Key::CTRL).bPressed)   return olc::Key::CTRL;
	if (GetKey(olc::Key::INS).bPressed)    return olc::Key::INS;
	if (GetKey(olc::Key::DEL).bPressed)    return olc::Key::DEL;
	if (GetKey(olc::Key::HOME).bPressed)   return olc::Key::HOME;
	if (GetKey(olc::Key::END).bPressed)    return olc::Key::END;
	if (GetKey(olc::Key::PGUP).bPressed)   return olc::Key::PGUP;
	if (GetKey(olc::Key::PGDN).bPressed)   return olc::Key::PGDN;
	if (GetKey(olc::Key::BACK).bPressed)   return olc::Key::BACK;
	if (GetKey(olc::Key::ESCAPE).bPressed) return olc::Key::ESCAPE;
	if (GetKey(olc::Key::RETURN).bPressed) return olc::Key::RETURN;
	if (GetKey(olc::Key::ENTER).bPressed)  return olc::Key::ENTER;
	if (GetKey(olc::Key::PAUSE).bPressed)  return olc::Key::PAUSE;
	if (GetKey(olc::Key::SCROLL).bPressed) return olc::Key::SCROLL;

	if (GetKey(olc::Key::NP0).bPressed) return olc::Key::NP0;
	if (GetKey(olc::Key::NP1).bPressed) return olc::Key::NP1;
	if (GetKey(olc::Key::NP2).bPressed) return olc::Key::NP2;
	if (GetKey(olc::Key::NP3).bPressed) return olc::Key::NP3;
	if (GetKey(olc::Key::NP4).bPressed) return olc::Key::NP4;
	if (GetKey(olc::Key::NP5).bPressed) return olc::Key::NP5;
	if (GetKey(olc::Key::NP6).bPressed) return olc::Key::NP6;
	if (GetKey(olc::Key::NP7).bPressed) return olc::Key::NP7;
	if (GetKey(olc::Key::NP8).bPressed) return olc::Key::NP8;
	if (GetKey(olc::Key::NP9).bPressed) return olc::Key::NP9;

	if (GetKey(olc::Key::NP_MUL).bPressed)	   return olc::Key::NP_MUL;
	if (GetKey(olc::Key::NP_DIV).bPressed)	   return olc::Key::NP_DIV;
	if (GetKey(olc::Key::NP_ADD).bPressed)	   return olc::Key::NP_ADD;
	if (GetKey(olc::Key::NP_SUB).bPressed)	   return olc::Key::NP_SUB;
	if (GetKey(olc::Key::NP_DECIMAL).bPressed) return olc::Key::NP_DECIMAL;

	if (GetKey(olc::Key::PERIOD).bPressed) return olc::Key::PERIOD;
	if (GetKey(olc::Key::EQUALS).bPressed) return olc::Key::EQUALS;
	if (GetKey(olc::Key::COMMA).bPressed)  return olc::Key::COMMA;
	if (GetKey(olc::Key::MINUS).bPressed)  return olc::Key::MINUS;

	if (GetKey(olc::Key::OEM_1).bPressed) return olc::Key::OEM_1;
	if (GetKey(olc::Key::OEM_2).bPressed) return olc::Key::OEM_2;
	if (GetKey(olc::Key::OEM_3).bPressed) return olc::Key::OEM_3;
	if (GetKey(olc::Key::OEM_4).bPressed) return olc::Key::OEM_4;
	if (GetKey(olc::Key::OEM_5).bPressed) return olc::Key::OEM_5;
	if (GetKey(olc::Key::OEM_6).bPressed) return olc::Key::OEM_6;
	if (GetKey(olc::Key::OEM_7).bPressed) return olc::Key::OEM_7;
	if (GetKey(olc::Key::OEM_8).bPressed) return olc::Key::OEM_8;

	if (GetKey(olc::Key::CAPS_LOCK).bPressed) return olc::Key::CAPS_LOCK;
	if (GetKey(olc::Key::ENUM_END).bPressed)  return olc::Key::ENUM_END;
}

bool OneLoneCoder_Platformer::IsSolidTile(wchar_t tile)
{
	// List Here all the tiles that are solid
	return tile == '#' || tile == 'G' || tile == 'B' || tile == 'P' || tile == 'H';
}

bool OneLoneCoder_Platformer::IsBreakableTile(wchar_t tile)
{
	return tile == 'B' || tile == 'P' || tile == 'H';
}

bool OneLoneCoder_Platformer::IsSemiSolidTile(wchar_t tile)
{
	// List Here all the tiles that are semi solid
	return tile == '_';
}

void OneLoneCoder_Platformer::AddProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner, bool breackableAgainstTiles, float fDrag, std::string sound, bool bouncy, std::string bounceSound, bool scenery)
{
	cDynamicProjectile* p = new cDynamicProjectile(ox, oy, bFriend, velx, vely, duration, mapProjectiles[sprite], affectedByGravity, damage, solidVSMap, oneHit, corner, breackableAgainstTiles, fDrag, bouncy, bounceSound, scenery);
	if (sound != "")
		p->SetSoundEffect(sound);
	vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit, int corner, std::string sound, bool scenery)
{
	cDynamicProjectile* p = new cDynamicProjectileBoomerang(ox, oy, bFriend, velx, vely, duration, mapProjectiles[sprite], damage, solidVSMap, oneHit, corner, scenery);
	if (sound != "")
		p->SetSoundEffect(sound);
	vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddOrbital(float ox, float oy, bool bFriend, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit, int corner, float cx, float cy, float angrot, std::string sound, bool scenery)
{
	cDynamicProjectile* p = new cDynamicProjectileOrbital(ox, oy, bFriend, duration, mapProjectiles[sprite], damage, solidVSMap, oneHit, corner, cx, cy, angrot, scenery);
	if (sound != "")
		p->SetSoundEffect(sound);
	vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddPlatform(float ox, float oy, std::string sprite, std::wstring iced)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatform(ox, oy, mapPlatforms[sprite], iced);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddScenery(float ox, float oy, std::string sprite)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatform(ox, oy, mapPlatforms[sprite]);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformHorSine(ox, oy, mapPlatforms[sprite], iced, amplitude, frequency, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformVerSine(ox, oy, mapPlatforms[sprite], iced, amplitude, frequency, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float tx, float vx, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformHorSingle(ox, oy, mapPlatforms[sprite], iced, tx, vx, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float ty, float vy, std::wstring trigger)
{
	cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformVerSingle(ox, oy, mapPlatforms[sprite], iced, ty, vy, trigger);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddWall(float ox, float oy, std::string sprite, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY)
{
	cDynamicMovingPlatform* ptfm = new cDynamicWall(ox, oy, mapPlatforms[sprite], leftSolid, rightSolid, trigger, trgX, trgY);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddCeiling(float ox, float oy, std::string sprite, std::wstring topSolid, std::wstring linkToPreviousPtfm)
{
	cDynamicMovingPlatform* ptfm = new cDynamicCeiling(ox, oy, mapPlatforms[sprite], topSolid, linkToPreviousPtfm);
	if (ptfm->IsLinkedToPreviousPtfm())
	{
		ptfm->LinkPtfm(vecPlatforms.front());
	}
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime)
{
	cDynamicMovingPlatform* ptfm = new cDynamicHorizontalCrusher(ox, oy, mapPlatforms[sprite], side, waitTime);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime)
{
	cDynamicMovingPlatform* ptfm = new cDynamicVerticalCrusher(ox, oy, mapPlatforms[sprite], side, waitTime);
	vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHarmfulBloc(float ox, float oy, std::string sprite, float dmg, std::wstring tangible)
{
	cDynamicMovingPlatform* ptfm = new cDynamicHarmfulBloc(ox, oy, mapPlatforms[sprite], dmg, tangible);
	vecPlatforms.push_back(ptfm);
}

std::vector<cDynamicMovingPlatform*> OneLoneCoder_Platformer::GetClosePlatforms(float px, float py)
{
	std::vector<cDynamicMovingPlatform*> closePtfms;
	for (auto& ptfm : vecPlatforms)
	{
		// Check the 4 corners to avoid the item disappearing when player is far from top left corner
		// -----------------------------------------------------------------
		// ------ /!\ FPS drops considerably if item is too large /!\ ------
		// -----------------------------------------------------------------
		if (fabs(px - (ptfm->GetPX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (ptfm->GetPY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (ptfm->GetPX() + ptfm->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (ptfm->GetPY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (ptfm->GetPX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (ptfm->GetPY() + ptfm->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (ptfm->GetPX() + ptfm->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (ptfm->GetPY() + ptfm->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f))
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

void OneLoneCoder_Platformer::AddWind(float ox, float oy, std::string sprite, std::wstring direction, float power)
{
	cDynamicWind* wind = new cDynamicWind(ox, oy, mapWinds[sprite], direction, power);
	vecWinds.push_back(wind);
}

std::vector<cDynamicWind*> OneLoneCoder_Platformer::GetCloseWinds(float px, float py)
{
	std::vector<cDynamicWind*> closeWinds;
	for (auto& wind : vecWinds)
	{
		// Check the 4 corners to avoid the item disappearing when player is far from top left corner
		// -----------------------------------------------------------------
		// ------ /!\ FPS drops considerably if item is too large /!\ ------
		// -----------------------------------------------------------------
		if (fabs(px - (wind->GetPX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (wind->GetPY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (wind->GetPX() + wind->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (wind->GetPY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (wind->GetPX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (wind->GetPY() + wind->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (wind->GetPX() + wind->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (wind->GetPY() + wind->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f))
		{
			closeWinds.push_back(wind);
		}
	}
	return closeWinds;
}

void OneLoneCoder_Platformer::AddTeleport(float ax, float ay, float bx, float by, std::string sprite)
{
	cDynamicTeleport* tp = new cDynamicTeleport(ax, ay, bx, by, mapTeleports[sprite]);
	vecTeleports.push_back(tp);
}

std::vector<cDynamicTeleport*> OneLoneCoder_Platformer::GetCloseTeleport(float px, float py)
{
	std::vector<cDynamicTeleport*> closeTeleports;
	for (auto& tp : vecTeleports)
	{
		// Check the 4 corners to avoid the item disappearing when player is far from top left corner
		// -----------------------------------------------------------------
		// ------ /!\ FPS drops considerably if item is too large /!\ ------
		// -----------------------------------------------------------------
		if (fabs(px - (tp->GetPX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetPY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (tp->GetPX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetPY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (tp->GetPX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetPY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (tp->GetPX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetPY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f))
		{
			closeTeleports.push_back(tp);
		}
	}
	return closeTeleports;
}

std::vector<cDynamicTeleport*> OneLoneCoder_Platformer::GetCloseTeleportDest(float px, float py)
{
	std::vector<cDynamicTeleport*> closeTeleports;
	for (auto& tp : vecTeleports)
	{
		// Check the 4 corners to avoid the item disappearing when player is far from top left corner
		// -----------------------------------------------------------------
		// ------ /!\ FPS drops considerably if item is too large /!\ ------
		// -----------------------------------------------------------------
		if (fabs(px - (tp->GetDestX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetDestY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (tp->GetDestX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetDestY())) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (tp->GetDestX())) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetDestY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f) ||
			fabs(px - (tp->GetDestX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (tp->GetDestY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f) * 1.5f))
		{
			closeTeleports.push_back(tp);
		}
	}
	return closeTeleports;
}

void OneLoneCoder_Platformer::RespawnEnnemies(cLevel* level)
{
	vecEnnemies.clear();
	level->PopulateEnnemies(vecEnnemies, level->GetEnnemies());
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
	if (gameState == "GS_LOADING")			  nGameState = GS_LOADING;
	else if (gameState == "GS_TITLE")		  nGameState = GS_TITLE;
	else if (gameState == "GS_MAIN")		  nGameState = GS_MAIN;
	else if (gameState == "GS_LEVELSTART")	  nGameState = GS_LEVELSTART;
	else if (gameState == "GS_LOADLEVEL")	  nGameState = GS_LOADLEVEL;
	else if (gameState == "GS_WORLDMAP")	  nGameState = GS_WORLDMAP;
	else if (gameState == "GS_ENDSCREEN")	  nGameState = GS_ENDSCREEN;
	else if (gameState == "GS_PAUSE")		  nGameState = GS_PAUSE;
	else if (gameState == "GS_LOADBOSSLEVEL") nGameState = GS_LOADBOSSLEVEL;
	else if (gameState == "GS_SELECTMENU")	  nGameState = GS_SELECTMENU;
	else if (gameState == "GS_CONTROLS")	  nGameState = GS_CONTROLS;
	else if (gameState == "GS_CREDITS")		  nGameState = GS_CREDITS;
	else if (gameState == "GS_Transition")	  nGameState = GS_TRANSITION;
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
	TransitionTo("GS_WORLDMAP", true);
	player->SetDamageBooster(1);
	player->SetDefenseBooster(1);
}

void OneLoneCoder_Platformer::GoToControlsMenu()
{
	olc::SOUND::StopAll();
	olc::SOUND::PlaySample(sndMenu, true);
	TransitionTo("GS_CONTROLS", true);
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
	player->SetPosX(px);
	player->SetPosY(py);
}

bool OneLoneCoder_Platformer::KirboCollisionWithEnnemy(cDynamic* object)
{
	return player->EnemyTouched(object, camera->GetOffsetX(), camera->GetOffsetY());
}

bool OneLoneCoder_Platformer::IsKirboAttackable()
{
	return player->IsAttackable();
}

void OneLoneCoder_Platformer::MaxHealPlayer()
{
	player->MaxHeal();
}

void OneLoneCoder_Platformer::HealPlayer(int hp)
{
	player->Heal(hp);
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
	SetPixelMode(olc::Pixel::ALPHA);

	Clear(olc::DARK_GREY);
	DrawKirboString(30, 440, loadPercent, 2);

	SetPixelMode(olc::Pixel::NORMAL);
}

void OneLoneCoder_Platformer::ApplyControls()
{
	std::wifstream file("assets/ctrls/controls.txt");

	if (file)
	{
		std::wstring line;

		while (std::getline(file, line))
		{
			std::wstring temp;
			std::vector<std::wstring> parts;
			std::wstringstream wss(line);
			while (std::getline(wss, temp, L'='))
			{
				parts.push_back(temp);
			}

			savedControls[ToStr(parts[0])] = ToOlcKey(ToStr(parts[1]));
		}
	}
}

olc::Key OneLoneCoder_Platformer::ToOlcKey(std::string key)
{
	olc::Key k;

	if (key == "none") k = olc::Key::NONE;

	if (key == "a") k = olc::Key::A;
	if (key == "b") k = olc::Key::B;
	if (key == "c") k = olc::Key::C;
	if (key == "d") k = olc::Key::D;
	if (key == "e") k = olc::Key::E;
	if (key == "f") k = olc::Key::F;
	if (key == "g") k = olc::Key::G;
	if (key == "h") k = olc::Key::H;
	if (key == "i") k = olc::Key::I;
	if (key == "j") k = olc::Key::J;
	if (key == "k") k = olc::Key::K;
	if (key == "l") k = olc::Key::L;
	if (key == "m") k = olc::Key::M;
	if (key == "n") k = olc::Key::N;
	if (key == "o") k = olc::Key::O;
	if (key == "p") k = olc::Key::P;
	if (key == "q") k = olc::Key::Q;
	if (key == "r") k = olc::Key::R;
	if (key == "s") k = olc::Key::S;
	if (key == "t") k = olc::Key::T;
	if (key == "u") k = olc::Key::U;
	if (key == "v") k = olc::Key::V;
	if (key == "w") k = olc::Key::W;
	if (key == "x") k = olc::Key::X;
	if (key == "y") k = olc::Key::Y;
	if (key == "z") k = olc::Key::Z;

	if (key == "k0") k = olc::Key::K0;
	if (key == "k1") k = olc::Key::K1;
	if (key == "k2") k = olc::Key::K2;
	if (key == "k3") k = olc::Key::K3;
	if (key == "k4") k = olc::Key::K4;
	if (key == "k5") k = olc::Key::K5;
	if (key == "k6") k = olc::Key::K6;
	if (key == "k7") k = olc::Key::K7;
	if (key == "k8") k = olc::Key::K8;
	if (key == "k9") k = olc::Key::K9;

	if (key == "f1")  k = olc::Key::F1;
	if (key == "f2")  k = olc::Key::F2;
	if (key == "f3")  k = olc::Key::F3;
	if (key == "f4")  k = olc::Key::F4;
	if (key == "f5")  k = olc::Key::F5;
	if (key == "f6")  k = olc::Key::F6;
	if (key == "f7")  k = olc::Key::F7;
	if (key == "f8")  k = olc::Key::F8;
	if (key == "f9")  k = olc::Key::F9;
	if (key == "f10") k = olc::Key::F10;
	if (key == "f11") k = olc::Key::F11;
	if (key == "f12") k = olc::Key::F12;

	if (key == "up")	k = olc::Key::UP;
	if (key == "down")  k = olc::Key::DOWN;
	if (key == "left")  k = olc::Key::LEFT;
	if (key == "right") k = olc::Key::RIGHT;

	if (key == "space")  k = olc::Key::SPACE;
	if (key == "tab")	 k = olc::Key::TAB;
	if (key == "shift")  k = olc::Key::SHIFT;
	if (key == "ctrl")	 k = olc::Key::CTRL;
	if (key == "ins")	 k = olc::Key::INS;
	if (key == "del")	 k = olc::Key::DEL;
	if (key == "home")	 k = olc::Key::HOME;
	if (key == "end")	 k = olc::Key::END;
	if (key == "pgup")	 k = olc::Key::PGUP;
	if (key == "pgdn")	 k = olc::Key::PGDN;
	if (key == "back")	 k = olc::Key::BACK;
	if (key == "escape") k = olc::Key::ESCAPE;
	if (key == "return") k = olc::Key::RETURN;
	if (key == "enter")  k = olc::Key::ENTER;
	if (key == "pause")  k = olc::Key::PAUSE;
	if (key == "scroll") k = olc::Key::SCROLL;

	if (key == "np0") k = olc::Key::NP0;
	if (key == "np1") k = olc::Key::NP1;
	if (key == "np2") k = olc::Key::NP2;
	if (key == "np3") k = olc::Key::NP3;
	if (key == "np4") k = olc::Key::NP4;
	if (key == "np5") k = olc::Key::NP5;
	if (key == "np6") k = olc::Key::NP6;
	if (key == "np7") k = olc::Key::NP7;
	if (key == "np8") k = olc::Key::NP8;
	if (key == "np9") k = olc::Key::NP9;

	if (key == "np_mul")	 k = olc::Key::NP_MUL;
	if (key == "np_div")	 k = olc::Key::NP_DIV;
	if (key == "np_add")	 k = olc::Key::NP_ADD;
	if (key == "np_sub")	 k = olc::Key::NP_SUB;
	if (key == "np_decimal") k = olc::Key::NP_DECIMAL;

	if (key == "period") k = olc::Key::PERIOD;
	if (key == "equals") k = olc::Key::EQUALS;
	if (key == "comma")  k = olc::Key::COMMA;
	if (key == "minus")  k = olc::Key::MINUS;

	if (key == "oem_1") k = olc::Key::OEM_1;
	if (key == "oem_2") k = olc::Key::OEM_2;
	if (key == "oem_3") k = olc::Key::OEM_3;
	if (key == "oem_4") k = olc::Key::OEM_4;
	if (key == "oem_5") k = olc::Key::OEM_5;
	if (key == "oem_6") k = olc::Key::OEM_6;
	if (key == "oem_7") k = olc::Key::OEM_7;
	if (key == "oem_8") k = olc::Key::OEM_8;

	if (key == "caps_lock") k = olc::Key::CAPS_LOCK;
	if (key == "enum_end")  k = olc::Key::ENUM_END;

	return k;
}

std::string OneLoneCoder_Platformer::olcKeyToStr(olc::Key key)
{
	std::string str;

	if (key == olc::Key::NONE) str = "none";

	if (key == olc::Key::A) str = "a";
	if (key == olc::Key::B) str = "b";
	if (key == olc::Key::C) str = "c";
	if (key == olc::Key::D) str = "d";
	if (key == olc::Key::E) str = "e";
	if (key == olc::Key::F) str = "f";
	if (key == olc::Key::G) str = "g";
	if (key == olc::Key::H) str = "h";
	if (key == olc::Key::I) str = "i";
	if (key == olc::Key::J) str = "j";
	if (key == olc::Key::K) str = "k";
	if (key == olc::Key::L) str = "l";
	if (key == olc::Key::M) str = "m";
	if (key == olc::Key::N) str = "n";
	if (key == olc::Key::O) str = "o";
	if (key == olc::Key::P) str = "p";
	if (key == olc::Key::Q) str = "q";
	if (key == olc::Key::R) str = "r";
	if (key == olc::Key::S) str = "s";
	if (key == olc::Key::T) str = "t";
	if (key == olc::Key::U) str = "u";
	if (key == olc::Key::V) str = "v";
	if (key == olc::Key::W) str = "w";
	if (key == olc::Key::X) str = "x";
	if (key == olc::Key::Y) str = "y";
	if (key == olc::Key::Z) str = "z";

	if (key == olc::Key::K0) str = "k0";
	if (key == olc::Key::K1) str = "k1";
	if (key == olc::Key::K2) str = "k2";
	if (key == olc::Key::K3) str = "k3";
	if (key == olc::Key::K4) str = "k4";
	if (key == olc::Key::K5) str = "k5";
	if (key == olc::Key::K6) str = "k6";
	if (key == olc::Key::K7) str = "k7";
	if (key == olc::Key::K8) str = "k8";
	if (key == olc::Key::K9) str = "k9";

	if (key == olc::Key::F1)  str = "f1";
	if (key == olc::Key::F2)  str = "f2";
	if (key == olc::Key::F3)  str = "f3";
	if (key == olc::Key::F4)  str = "f4";
	if (key == olc::Key::F5)  str = "f5";
	if (key == olc::Key::F6)  str = "f6";
	if (key == olc::Key::F7)  str = "f7";
	if (key == olc::Key::F8)  str = "f8";
	if (key == olc::Key::F9)  str = "f9";
	if (key == olc::Key::F10) str = "f10";
	if (key == olc::Key::F11) str = "f11";
	if (key == olc::Key::F12) str = "f12";

	if (key == olc::Key::UP)    str = "up";
	if (key == olc::Key::DOWN)  str = "down";
	if (key == olc::Key::LEFT)  str = "left";
	if (key == olc::Key::RIGHT) str = "right";

	if (key == olc::Key::SPACE)	 str = "space";
	if (key == olc::Key::TAB)	 str = "tab";
	if (key == olc::Key::SHIFT)	 str = "shift";
	if (key == olc::Key::CTRL)	 str = "ctrl";
	if (key == olc::Key::INS)	 str = "ins";
	if (key == olc::Key::DEL)	 str = "del";
	if (key == olc::Key::HOME)	 str = "home";
	if (key == olc::Key::END)	 str = "end";
	if (key == olc::Key::PGUP)	 str = "pgup";
	if (key == olc::Key::PGDN)	 str = "pgdn";
	if (key == olc::Key::BACK)	 str = "back";
	if (key == olc::Key::ESCAPE) str = "escape";
	if (key == olc::Key::RETURN) str = "return";
	if (key == olc::Key::ENTER)  str = "enter";
	if (key == olc::Key::PAUSE)  str = "pause";
	if (key == olc::Key::SCROLL) str = "scroll";

	if (key == olc::Key::NP0) str = "np0";
	if (key == olc::Key::NP1) str = "np1";
	if (key == olc::Key::NP2) str = "np2";
	if (key == olc::Key::NP3) str = "np3";
	if (key == olc::Key::NP4) str = "np4";
	if (key == olc::Key::NP5) str = "np5";
	if (key == olc::Key::NP6) str = "np6";
	if (key == olc::Key::NP7) str = "np7";
	if (key == olc::Key::NP8) str = "np8";
	if (key == olc::Key::NP9) str = "np9";

	if (key == olc::Key::NP_MUL)	 str = "np_mul";
	if (key == olc::Key::NP_DIV)	 str = "np_div";
	if (key == olc::Key::NP_ADD)	 str = "np_add";
	if (key == olc::Key::NP_SUB)	 str = "np_sub";
	if (key == olc::Key::NP_DECIMAL) str = "np_decimal";

	if (key == olc::Key::PERIOD) str = "period";
	if (key == olc::Key::EQUALS) str = "equals";
	if (key == olc::Key::COMMA)  str = "comma";
	if (key == olc::Key::MINUS)  str = "minus";

	if (key == olc::Key::OEM_1) str = "oem_1";
	if (key == olc::Key::OEM_2) str = "oem_2";
	if (key == olc::Key::OEM_3) str = "oem_3";
	if (key == olc::Key::OEM_4) str = "oem_4";
	if (key == olc::Key::OEM_5) str = "oem_5";
	if (key == olc::Key::OEM_6) str = "oem_6";
	if (key == olc::Key::OEM_7) str = "oem_7";
	if (key == olc::Key::OEM_8) str = "oem_8";

	if (key == olc::Key::CAPS_LOCK) str = "caps_lock";
	if (key == olc::Key::ENUM_END)  str = "enum_end";

	return str;
}

olc::Key OneLoneCoder_Platformer::GetSavedControls(std::string control)
{
	return savedControls[control];
}

void OneLoneCoder_Platformer::TransitionTo(std::string newState, bool playTransitionSound)
{
	bPlayTransitionSound = playTransitionSound;
	sNextState = newState;
	nGameState = GS_TRANSITION;
}

void OneLoneCoder_Platformer::DrawKirboString(int x, int y, std::string text, int scale, bool centered)
{
	int pos = 0;
	int textWidth = 0;

	if (centered)
	{
		for (std::string::iterator it = text.begin(); it != text.end(); ++it)
		{
			std::string chr(1, toupper(*it));
			textWidth += 13 * scale;
		}
	}

	for (std::string::iterator it = text.begin(); it != text.end(); ++it)
	{
		std::string chr(1, toupper(*it));
		if (chr != " ")
			DrawSprite(x + pos * 13 * scale - (textWidth / 2), y, cAssets::get().GetSprite(chr), scale);

		++pos;
	};
}

std::string OneLoneCoder_Platformer::ToStr(std::wstring str)
{
	//setup converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(str);
}
