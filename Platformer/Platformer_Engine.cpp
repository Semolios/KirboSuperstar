#include "Platformer_Engine.h"

OneLoneCoder_Platformer::OneLoneCoder_Platformer()
{
    sAppName = "Kirbo Superstar";
}

bool OneLoneCoder_Platformer::OnUserCreate()
{
    // Init Decals
    decPlayer = new olc::Decal(&sprPlayer);
    decDoorUp = new olc::Decal(&sprDoorUp);
    decBackground = new olc::Decal(&sprBackground);
    decParallax1 = new olc::Decal(&sprParallax1);
    decParallax2 = new olc::Decal(&sprParallax2);
    decParallax3 = new olc::Decal(&sprParallax3);
    decSpecialTiles = new olc::Decal(&sprSpecialTiles);
    decGrdTiles = new olc::Decal(&sprGrdTiles);
    decDoor = new olc::Decal(&sprDoor);
    decHealthBar = new olc::Decal(&sprHealthBar);
    decHealthPoint = new olc::Decal(&sprHealthPoint);
    decBossHealthBar = new olc::Decal(&sprBossHealthBar);

    controller.Initialize();
    waveEngine.InitialiseAudio();
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
            case GS_MANUAL:	       GameState_Manual(fElapsedTime);        break;
            case GS_LEVELSTART:	   GameState_LevelStart(fElapsedTime);    break;
            case GS_LOADLEVEL:	   GameState_LoadLevel(fElapsedTime);     break;
            case GS_WORLDMAP:	   GameState_WorldMap(fElapsedTime);	  break;
            case GS_ENDSCREEN:	   GameState_EndScreen(fElapsedTime);	  break;
            case GS_PAUSE:		   GameState_PauseMenu(fElapsedTime);	  break;
            case GS_LOADBOSSLEVEL: GameState_LoadBossLevel(fElapsedTime); break;
            case GS_SELECTMENU:	   GameState_SelectMenu(fElapsedTime);    break;
            case GS_CONTROLS:	   GameState_Controls(fElapsedTime);	  break;
            case GS_SOUNDS:		   GameState_Sounds(fElapsedTime);		  break;
            case GS_SCREENMODE:	   GameState_ScreenMode(fElapsedTime);	  break;
            case GS_CREDITS:	   GameState_Credits(fElapsedTime);		  break;
            case GS_CLOSE:		   GameState_Close(fElapsedTime);	      break;
            case GS_TRANSITION:	   GameState_Transition(fElapsedTime);    break;
        }
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_Manual(float fElapsedTime)
{
    manual->Update(this, fElapsedTime);

    if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
    {
        if (!manual->IsLastPage())
        {
            manual->NextPage();
        }
        else
        {
            TransitionTo("GS_LEVELSTART", true, false);
            manual->SetPage(0);
        }
    }

    return false;
}

bool OneLoneCoder_Platformer::GameState_LevelStart(float fElapsedTime)
{
    levelStart->Update(this, fElapsedTime, &sprPlayer, decPlayer);

    switch (levelStartAnim)
    {
        case 0: animPlayer.ChangeState("idle", &sprPlayer, decPlayer);        break;
        case 1: animPlayer.ChangeState("run", &sprPlayer, decPlayer);		  break;
        case 2: animPlayer.ChangeState("flying", &sprPlayer, decPlayer);	  break;
        case 3: animPlayer.ChangeState("riding_star", &sprPlayer, decPlayer); break;
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

    DrawKirboString(295, 450, "press any key");

    SetPixelMode(olc::Pixel::NORMAL);

    if (GetAnyKey() || controller.AnyButtonPressed())
        TransitionTo("GS_MAIN", true, false);

    return false;
}

bool OneLoneCoder_Platformer::GameState_SplashScreen(float fElapsedTime)
{
    fSplashScreenTimer += fElapsedTime;

    olc::Sprite sprSplashScreen("assets/gfx/OLCPixelGameEngineSplashScreen.png");

    DrawSprite(0, 0, &sprSplashScreen);

    if (fSplashScreenTimer >= fSplashScreenTime || GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
        TransitionTo("GS_LOADING", false, false);

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
            sprBackground = olc::Sprite();
            sprParallax1 = olc::Sprite();
            sprParallax2 = olc::Sprite();
            sprParallax3 = olc::Sprite();
            sprSpecialTiles = olc::Sprite();
            sprGrdTiles = olc::Sprite();
            sprDoor = olc::Sprite("assets/gfx/bossDoor.png");
            sprDoorUp = olc::Sprite("assets/gfx/doorUp.png");
            checkPoint = new cCheckPoint();
            
            decDoor->Update();
            decDoorUp->Update();

            level->LoadLevelsList();
            level->LoadBossesList();
            level->LoadEnnemiesList();
            level->LoadMechanismsList();
            level->LoadBossMechanismsList();
            level->LoadSpecialTilesList();
            level->LoadGroundTilesList();
            level->LoadBackGroundsList();
            level->LoadBossesBackGroundsList();
            level->LoadMusicsList();
            level->LoadBossesMusicsList();

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
            mapDecProjectiles = cDynamicProjectile::LoadProjectilesDecals(mapProjectiles);

            UpdateProgressBar("Loading 13%");

            nLoadingState = LS_MECHANISMS;
        }
        break;
        case LS_MECHANISMS:
        {
            sprDoorSwitchOff = olc::Sprite("assets/gfx/doorSwitchOff.png");
            sprDoorSwitchOn  = olc::Sprite("assets/gfx/doorSwitchOn.png");
            decDoorSwitchOff = new olc::Decal(&sprDoorSwitchOff);
            decDoorSwitchOn  = new olc::Decal(&sprDoorSwitchOn);
            mapPlatforms = cDynamicMovingPlatform::LoadMovingPlatformsSprites();
            mapDecPlatforms = cDynamicMovingPlatform::LoadMovingPlatformsDecals(mapPlatforms);
            mapTeleports = cDynamicTeleport::LoadTeleportsSprites();
            mapDecTeleports = cDynamicTeleport::LoadTeleportsDecals(mapTeleports);
            mapWinds = cDynamicWind::LoadWindSprites();
            mapDecWinds = cDynamicWind::LoadWindDecals(mapWinds);

            UpdateProgressBar("Loading 16%");

            nLoadingState = LS_TITLE;
        }
        break;
        case LS_TITLE:
        {
            sprTitleScreen = olc::Sprite("assets/gfx/titleScreen.png");
            titleScreen = new cTitleScreen(this, &sprTitleScreen);

            UpdateProgressBar("Loading 20%");

            nLoadingState = LS_WORLDMAP;
        }
        break;
        case LS_WORLDMAP:
        {
            sprWorldMap = olc::Sprite("assets/gfx/WorldMap.png");
            sprLock = olc::Sprite("assets/gfx/Lock.png");
            worldMap = new cWorldMap(this, &sprWorldMap, &sprLock, &animPlayer);
            worldMap->SetUnlockedLevel(level->GetUnlockedLvl());

            UpdateProgressBar("Loading 23%");

            nLoadingState = LS_MANUAL;
        }
        break;
        case LS_MANUAL:
        {
            sprVacuum = olc::Sprite("assets/gfx/kirboVacuum00.png");;
            sprLaunchJesusCross = olc::Sprite("assets/gfx/kirbolaunchingcross04.png");
            sprJesusCross = olc::Sprite("assets/gfx/jesuscross.png");
            sprSlapAttack = olc::Sprite("assets/gfx/slap02.png");
            sprJump = olc::Sprite("assets/gfx/kirboJump.png");
            sprFly = olc::Sprite("assets/gfx/kirboFlying00.png");
            sprCameraDown = olc::Sprite("assets/gfx/cameraDown.png");
            manual = new cManual(this,
                                 &sprVacuum,
                                 &sprLaunchJesusCross,
                                 &sprJesusCross,
                                 &sprSlapAttack,
                                 &sprJump,
                                 &sprFly,
                                 &sprCameraDown);

            UpdateProgressBar("Loading 27%");

            nLoadingState = LS_LEVELSTART;
        }
        break;
        case LS_LEVELSTART:
        {
            sprLevelStart = olc::Sprite("assets/gfx/levelStartScreen.png");
            levelStart = new cLevelStart(this, &sprLevelStart, &animPlayer);

            UpdateProgressBar("Loading 30%");

            nLoadingState = LS_ENDSCREEN;
        }
        break;
        case LS_ENDSCREEN:
        {
            sprEndScreen = olc::Sprite("assets/gfx/endScreen.png");
            endScreen = new cEndScreen(this, &sprEndScreen, &animPlayer);

            UpdateProgressBar("Loading 40%");

            nLoadingState = LS_PAUSEMENU;
        }
        break;
        case LS_PAUSEMENU:
        {
            sprPauseMenu = olc::Sprite("assets/gfx/PauseMenu.png");
            sprCursor = olc::Sprite("assets/gfx/cursor.png");
            pauseMenu = new cPauseMenu(&sprPauseMenu, &sprCursor);

            UpdateProgressBar("Loading 46%");

            nLoadingState = LS_SELECTMENU;
        }
        break;
        case LS_SELECTMENU:
        {
            sprSelectMenu = olc::Sprite("assets/gfx/SelectMenu.png");
            selectMenu = new cSelectMenu(&sprSelectMenu, &sprCursor);

            UpdateProgressBar("Loading 51%");

            nLoadingState = LS_SOUNDMENU;
        }
        break;
        case LS_SOUNDMENU:
        {
            sprSoundMenuNoSound = olc::Sprite("assets/gfx/soundMenuNoSound.png");
            sprSoundMenuLow = olc::Sprite("assets/gfx/soundMenuLow.png");
            sprSoundMenuMid = olc::Sprite("assets/gfx/soundMenuMid.png");
            sprSoundMenuHigh = olc::Sprite("assets/gfx/soundMenuHigh.png");
            sprSoundMenuMax = olc::Sprite("assets/gfx/soundMenuMax.png");
            sprSoundMenuRightArrow = olc::Sprite("assets/gfx/rightArrow.png");
            sprSoundMenuLeftArrow = olc::Sprite("assets/gfx/leftArrow.png");
            soundMenu = new cSoundMenu(&sprSoundMenuNoSound,
                                       &sprSoundMenuLow,
                                       &sprSoundMenuMid,
                                       &sprSoundMenuHigh,
                                       &sprSoundMenuMax,
                                       &sprSoundMenuRightArrow,
                                       &sprSoundMenuLeftArrow);
            UpdateVolume(soundMenu->GetSoundVolume());

            UpdateProgressBar("Loading 52%");

            nLoadingState = LS_SCREENMODEMENU;
        }
        break;
        case LS_SCREENMODEMENU:
        {
            sprScreenModeMenu = olc::Sprite("assets/gfx/screenMode.png");
            screenModeMenu = new cScreenModeMenu(&sprScreenModeMenu, &sprSoundMenuRightArrow, &sprSoundMenuLeftArrow);

            UpdateProgressBar("Loading 53%");

            nLoadingState = LS_CONTROLSMENU;
        }
        break;
        case LS_CONTROLSMENU:
        {
            sprControlsMenu = olc::Sprite("assets/gfx/ControlsMenu.png");
			sprMenuBar		= olc::Sprite("assets/gfx/MenuBar.png");
			sprUnselQuit	= olc::Sprite("assets/gfx/unselQuit.png");
			sprSelQuit		= olc::Sprite("assets/gfx/selQuit.png");
            sprUnselDefault = olc::Sprite("assets/gfx/unselDefault.png");
			sprSelDefault	= olc::Sprite("assets/gfx/selDefault.png");
            // Controller sprites
			sprX	 = olc::Sprite("assets/gfx/XBtn.png");
			sprY	 = olc::Sprite("assets/gfx/YBtn.png");
			sprB	 = olc::Sprite("assets/gfx/BBtn.png");
            sprPause = olc::Sprite("assets/gfx/PauseBtn.png");
			sprUp	 = olc::Sprite("assets/gfx/UpBtn.png");
			sprDown	 = olc::Sprite("assets/gfx/DownBtn.png");
			sprLeft	 = olc::Sprite("assets/gfx/LeftBtn.png");
            sprRight = olc::Sprite("assets/gfx/RightBtn.png");
			sprA	 = olc::Sprite("assets/gfx/ABtn.png");
            sprRBump = olc::Sprite("assets/gfx/RBumpBtn.png");

            controlsMenu = new cControlsMenu(&sprControlsMenu,
                                             &sprMenuBar,
                                             &sprUnselQuit,
                                             &sprSelQuit,
                                             &sprUnselDefault,
                                             &sprSelDefault,
                                             &sprX,
                                             &sprY,
                                             &sprB,
                                             &sprPause,
                                             &sprUp,
                                             &sprDown,
                                             &sprLeft,
                                             &sprRight,
                                             &sprA,
                                             &sprRBump);

            ApplyControls();

            UpdateProgressBar("Loading 54%");

            nLoadingState = LS_CREDITSMENU;
        }
        break;
        case LS_CREDITSMENU:
        {
            sprCreditsMenu = olc::Sprite("assets/gfx/CreditsMenu.png");
            creditsMenu = new cCreditsMenu(&sprCreditsMenu);

            UpdateProgressBar("Loading 58%");

            nLoadingState = LS_HUD;
        }
        break;
        case LS_HUD:
        {
            HUD = new cHUD();
            sprHealthBar = olc::Sprite("assets/gfx/emptyHealthBar.png");
            sprHealthPoint = olc::Sprite("assets/gfx/healthPoint.png");
            sprBossHealthBar = olc::Sprite("assets/gfx/emptyBossHealthBar.png");
            decHealthBar->Update();
            decHealthPoint->Update();
            decBossHealthBar->Update();

            UpdateProgressBar("Loading 66%");

            nLoadingState = LS_ENGINEPOINTERS;
        }
        break;
        case LS_ENGINEPOINTERS:
        {
            cDynamicCreature::engine = this;
            cDynamicCreatureBladeKnight::engine = this;
            cDynamicCreatureBomber::engine = this;
            cDynamicCreatureFrosty::engine = this;
            cDynamicCreatureKingDDD::engine = this;
            cDynamicCreatureKracko::engine = this;
            cDynamicCreatureMrShineMrBright::engine = this;
            cDynamicCreatureRocky::engine = this;
            cDynamicCreatureSpawnerBoss::engine = this;
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

            UpdateProgressBar("Loading 95%");

            nLoadingState = LS_SOUNDS;
        }
        break;
        case LS_SOUNDS:
        {
            sndTitleScreen.LoadAudioWaveform("assets/snd/titleScreen.wav");
            sndWorldMap.LoadAudioWaveform("assets/snd/worldMap.wav");
            sndBossKilled.LoadAudioWaveform("assets/snd/bossKilled.wav");
            sndWind.LoadAudioWaveform("assets/snd/wind.wav");
            sndInvincibility.LoadAudioWaveform("assets/snd/invincibility.wav");
            sndMenu.LoadAudioWaveform("assets/snd/menus.wav");
            sndTransition.LoadAudioWaveform("assets/snd/transition.wav");
            sndEndScreen.LoadAudioWaveform("assets/snd/endScreen.wav");

            AddSharedSound("whispyScream", &sndWhispyScream, "assets/snd/whispyScream.wav");
            AddSharedSound("loseLife", &sndLoseLife, "assets/snd/loseLife.wav");
            AddSharedSound("punch", &sndPunch, "assets/snd/punch.wav");
            AddSharedSound("slap", &sndSlap, "assets/snd/slap.wav");
            AddSharedSound("kirboWalk", &sndKirboWalk, "assets/snd/kirboWalk.wav");
            AddSharedSound("kirboFly", &sndKirboFly, "assets/snd/kirboFly.wav");
            AddSharedSound("explosion", &sndExplosion, "assets/snd/explosion.wav");
            AddSharedSound("kirboJump", &sndKirboJump, "assets/snd/kirboJump.wav");
            AddSharedSound("kirboHit", &sndKirboHit, "assets/snd/kirboHit.wav");
            AddSharedSound("beginVacuum", &sndKirboBeginVacuum, "assets/snd/kirboBeginVacuum.wav");
            AddSharedSound("vacuum", &sndKirboVacuum, "assets/snd/kirboVacuum.wav");
            AddSharedSound("swallow", &sndKirboSwallow, "assets/snd/kirboSwallow.wav");
            AddSharedSound("wetSlap", &sndWetSlap, "assets/snd/wetSlap.wav");
            AddSharedSound("earthQuake", &sndEarthQuake, "assets/snd/earthQuake.wav");
            AddSharedSound("blow", &sndBlow, "assets/snd/blow.wav");
            AddSharedSound("inhale", &sndInhale, "assets/snd/inhale.wav");
            AddSharedSound("poyo01", &sndPoyo01, "assets/snd/poyo01.wav");
            AddSharedSound("poyo02", &sndPoyo02, "assets/snd/poyo02.wav");
            AddSharedSound("sunShootingMoon", &sndSunShootingMoon, "assets/snd/sunShootingMoon.wav");
            AddSharedSound("sunShootUp", &sndSunShootUp, "assets/snd/sunShootUp.wav");
            AddSharedSound("beamCharge", &sndBeamCharge, "assets/snd/beamCharge.wav");
            AddSharedSound("beam", &sndBeam, "assets/snd/beam.wav");
            AddSharedSound("volcanoBoom", &sndVolcanoBoom, "assets/snd/volcanoBoom.wav");
            AddSharedSound("iceBlow", &sndIceBlow, "assets/snd/iceBlow.wav");
            AddSharedSound("patpat", &sndpatpat, "assets/snd/patpat.wav");
            AddSharedSound("itemLaunch", &sndItemLaunch, "assets/snd/itemLaunch.wav");
            AddSharedSound("superstar", &sndSuperstar, "assets/snd/superstar.wav");
            AddSharedSound("frostyJump", &sndFrostyJump, "assets/snd/frostyJump.wav");
            AddSharedSound("frostyGroundPound", &sndFrostyGroundPound, "assets/snd/frostyGroundPound.wav");
            AddSharedSound("lightning", &sndLightning, "assets/snd/lightning.wav");
            AddSharedSound("electricity", &sndElectricity, "assets/snd/electricity.wav");
            AddSharedSound("electricity2", &sndElectricity2, "assets/snd/electricity2.wav");
            AddSharedSound("meleeControllerRape", &sndMeleeControllerRape, "assets/snd/meleeControllerRape.wav");
            AddSharedSound("dash", &sndDash, "assets/snd/dash.wav");
            AddSharedSound("hiyayayaya", &sndHiyayayaya, "assets/snd/hiyayayaya.wav");
            AddSharedSound("singleSwordStrike", &sndSingleSwordStrike, "assets/snd/singleSwordStrike.wav");
            AddSharedSound("multipleSwordStrike", &sndMultipleSwordStrike, "assets/snd/multipleSwordStrike.wav");
            AddSharedSound("ha", &sndHa, "assets/snd/ha.wav");
            AddSharedSound("tornado", &sndTornado, "assets/snd/tornado.wav");
            AddSharedSound("loudScreaming", &sndLoudScreaming, "assets/snd/loudScreaming.wav");
            AddSharedSound("behold", &sndBehold, "assets/snd/behold.wav");
            AddSharedSound("kingDDDJump", &sndKingDDDJump, "assets/snd/kingDDDJump.wav");
            AddSharedSound("kingDDDUpSmash", &sndKingDDDUpSmash, "assets/snd/kingDDDUpSmash.wav");
            AddSharedSound("kingDDDUpSmashHammer", &sndKingDDDUpSmashHammer, "assets/snd/kingDDDUpSmashHammer.wav");
            AddSharedSound("kingDDDSideSmash", &sndKingDDDSideSmash, "assets/snd/kingDDDSideSmash.wav");
            AddSharedSound("kingDDDDownSmashHammer", &sndKingDDDDownSmashHammer, "assets/snd/kingDDDDownSmashHammer.wav");
            AddSharedSound("kingDDDSideB", &sndKingDDDSideB, "assets/snd/kingDDDSideB.wav");
            AddSharedSound("spikeBallBounce", &sndSpikeBallBounce, "assets/snd/spikeBallBounce.wav");
            AddSharedSound("kingDDDDownBCharge", &sndKingDDDDownBCharge, "assets/snd/kingDDDDownBCharge.wav");
            AddSharedSound("kingDDDDownBPrepare", &sndKingDDDDownBPrepare, "assets/snd/kingDDDDownBPrepare.wav");
            AddSharedSound("kingDDDDownBAttack", &sndKingDDDDownBAttack, "assets/snd/kingDDDDownBAttack.wav");
            AddSharedSound("kingDDDUpBJump", &sndKingDDDUpBJump, "assets/snd/kingDDDUpBJump.wav");
            AddSharedSound("kingDDDUpBLanding", &sndKingDDDUpBLanding, "assets/snd/kingDDDUpBLanding.wav");
            AddSharedSound("kingDDDVacuum", &sndKingDDDVacuum, "assets/snd/kingDDDVacuum.wav");
            AddSharedSound("kingDDDUpAir", &sndKingDDDUpAir, "assets/snd/kingDDDUpAir.wav");
            AddSharedSound("kingDDDAirAtkVoice", &sndKingDDDAirAtkVoice, "assets/snd/kingDDDAirAtkVoice.wav");
            AddSharedSound("kingDDDAirAtkHammer", &sndKingDDDAirAtkHammer, "assets/snd/kingDDDAirAtkHammer.wav");
            AddSharedSound("itemPicked", &sndItemPicked, "assets/snd/itemPicked.wav");
            AddSharedSound("rockyFall", &sndRockyFall, "assets/snd/rockyFall.wav");
            AddSharedSound("boom", &sndBoom, "assets/snd/boom.wav");
            AddSharedSound("enterDoor", &sndEnterDoor, "assets/snd/enterDoor.wav");
            AddSharedSound("menuBip", &sndMenuBip, "assets/snd/menuBip.wav");
            AddSharedSound("gateOpening", &sndGateOpening, "assets/snd/gateOpening.wav");
            AddSharedSound("gateOpened", &sndGateOpened, "assets/snd/gateOpened.wav");
            AddSharedSound("creditsMenu", &sndCreditMenu, "assets/snd/CreditsMenu.wav");

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
        TransitionTo("GS_TITLE", true, false);

    return true;
}

bool OneLoneCoder_Platformer::GameState_LoadLevel(float fElapsedTime)
{
    DestroyAllDynamics();

    level->SetCurrentLvl(worldMap->GetSelectedLevel());
    if (level->LoadLevel(level->GetName()))
    {
        SetKirboPositions(level->GetInitPlayerPosX(), level->GetInitPlayerPosY());

        level->PopulateEnnemies(vecEnnemies, level->GetEnnemies());
        level->PopulateMechanisms(level->GetMechanisms());

        sprSpecialTiles.LoadFromFile(level->GetSpecialTilesSpritesheet());
        decSpecialTiles->Update();
        sprGrdTiles.LoadFromFile(level->GetGroundTilesSpritesheet());
        decGrdTiles->Update();
        sprBackground.LoadFromFile(level->GetBackGroundSpritesheet());
        decBackground->Update();
        sprParallax1.LoadFromFile(level->GetParallax1Spritesheet());
        decParallax1->Update();
        sprParallax2.LoadFromFile(level->GetParallax2Spritesheet());
        decParallax2->Update();
        sprParallax3.LoadFromFile(level->GetParallax3Spritesheet());
        decParallax3->Update();
        sndLevelMusic.LoadAudioWaveform(level->GetMusic());
    }

    // Reset variables when level is loading
    player->InitialiseKirboHealth();
    ResetVariables();

    levelStartAnim = rand() % 4;

    bInBossLvl = false;

    waveEngine.StopAll();
    pwLevelMusic = waveEngine.PlayWaveform(&sndLevelMusic, true);

    // Permanent wind in Halberd level
    if (worldMap->GetSelectedLevel() == 4)
    {
        ActivateShakeEffect(true, 40, 40);
    }

    checkPoint->Use(level, this, vecPlatforms);

    if (worldMap->GetSelectedLevel() == 0)
    {
        TransitionTo("GS_MANUAL", true, false);
    }
    else
    {
        TransitionTo("GS_LEVELSTART", true, false);
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_Title(float fElapsedTime)
{
    if (!waveEngine.IsWaveformPlaying(&sndTitleScreen))
        pwTitleScreen = waveEngine.PlayWaveform(&sndTitleScreen, true);

    titleScreen->Update(this, fElapsedTime);

    if (GetAnyKey() || controller.AnyButtonPressed())
    {
        waveEngine.StopAll();
        TransitionTo("GS_SELECTMENU", true, false);
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_Main(float fElapsedTime)
{
    float angle   = 0.0f;
    float offsetX = 0.0f; 
    float offsetY = 0.0f;

    UpdateGame(fElapsedTime, &angle, &offsetX, &offsetY);

    DrawGame(fElapsedTime, angle, offsetX, offsetY);

    // Don't move kirbo before the white fading is finished
    player->ChangePosAfterTP();

    return true;
}

bool OneLoneCoder_Platformer::GameState_WorldMap(float fElapsedTime)
{
    worldMap->Update(this, fElapsedTime, GetController(), &sprPlayer, decPlayer);

    if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
        nGameState = GS_LOADLEVEL; // This transition is not visible for the player, no white screen and sound

    if (GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(B).bPressed)
    {
        waveEngine.StopAll();
        TransitionTo("GS_SELECTMENU", true, false);
    }

    return false;
}

bool OneLoneCoder_Platformer::GameState_EndScreen(float fElapsedTime)
{
    endScreen->Update(this, fElapsedTime, &sprPlayer, decPlayer);

    animPlayer.ChangeState("joy", &sprPlayer, decPlayer);

    if (bPlayMusic)
    {
        bPlayMusic = false;
        pwEndScreen = waveEngine.PlayWaveform(&sndEndScreen);
    }

    SetPixelMode(olc::Pixel::ALPHA);
    DrawKirboString(400, 400, "Thank you for playing", 2, true);
    DrawKirboString(400, 432, "Kirbo Superstar", 2, true);
    SetPixelMode(olc::Pixel::NORMAL);

    if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
    {
        waveEngine.StopAll();
        bPlayMusic = true;
        TransitionTo("GS_TITLE", true, false);
    }

    return false;
}

bool OneLoneCoder_Platformer::GameState_PauseMenu(float fElapsedTime)
{
    pauseMenu->Update(this, fElapsedTime, GetController());

    if (GetKey(olc::Key::P).bPressed || controller.GetButton(START).bPressed)
    {
        TransitionTo("GS_MAIN", true, false);
    }
    else if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
    {
        if (pauseMenu->GetPlayerChoice() == 1)
            ReturnToWorldMap(false);
        else
            TransitionTo("GS_MAIN", true, false);
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_LoadBossLevel(float fElapsedTime)
{
    DestroyAllDynamics();

    if (level->LoadLevel(level->GetBoss()))
    {
        SetKirboPositions(level->GetInitPlayerPosX(), level->GetInitPlayerPosY());

        level->PopulateBoss(vecEnnemies);
        level->PopulateMechanisms(level->GetBossMechanisms());

        sprBackground.LoadFromFile(level->GetBossBackGroundSpritesheet());
        decBackground->Update();
        sndBossLevelMusic.LoadAudioWaveform(level->GetBossMusic());
    }

    // Reset variables when level is loading
    ResetVariables();

    bInBossLvl = true;

    waveEngine.StopAll();
    pwBossLevelMusic = waveEngine.PlayWaveform(&sndBossLevelMusic, true);

    TransitionTo("GS_MAIN", false, false);

    return true;
}

bool OneLoneCoder_Platformer::GameState_SelectMenu(float fElapsedTime)
{
    selectMenu->Update(this, fElapsedTime, GetController());

    if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
    {
        if (!selectMenu->IsInOptionSubmenu())
        {
            if (selectMenu->GetPlayerChoice() == 0)
                ReturnToWorldMap(false);
            else if (selectMenu->GetPlayerChoice() == 1)
                selectMenu->SetInOptionSubmenu(true);
            else if (selectMenu->GetPlayerChoice() == 2)
                TransitionTo("GS_CREDITS", true, false);
            else
                nGameState = GS_CLOSE; // No need a transition when closing the game
        }
        else
        {
            if (selectMenu->GetPlayerChoice() == 0)
                GoToControlsMenu();
            else if (selectMenu->GetPlayerChoice() == 1)
                GoToSoundMenu();
            else if (selectMenu->GetPlayerChoice() == 2)
                GoToScreenModeMenu();
            else
                selectMenu->SetInOptionSubmenu(false);
        }
    }

    // Can quit the option submenu with escape or B
    if ((GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(B).bPressed) && selectMenu->IsInOptionSubmenu())
        selectMenu->SetInOptionSubmenu(false);

    return true;
}

bool OneLoneCoder_Platformer::GameState_Controls(float fElapsedTime)
{
    controlsMenu->Update(this, fElapsedTime, GetController());

    if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
    {
        if (controlsMenu->GetSelectedItem() == 11)
        {
            controlsMenu->UpdateSavedControls(this);
            controlsMenu->SetSelectedItem(0);
            ApplyControls();
            waveEngine.StopAll();
            TransitionTo("GS_SELECTMENU", true, false);
        }
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_Sounds(float fElapsedTime)
{
    soundMenu->Update(this, fElapsedTime, GetController());

    if (GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(B).bPressed)
    {
        waveEngine.StopAll();
        TransitionTo("GS_SELECTMENU", true, false);
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_ScreenMode(float fElapsedTime)
{
    screenModeMenu->Update(this, fElapsedTime, GetController());

    if (GetKey(olc::Key::ESCAPE).bPressed || controller.GetButton(B).bPressed)
    {
        waveEngine.StopAll();
        TransitionTo("GS_SELECTMENU", true, false);
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_Credits(float fElapsedTime)
{
    creditsMenu->Update(this, fElapsedTime);

    if (GetAnyKey() || controller.AnyButtonPressed() || creditsMenu->IsCreditOver())
    {
        TransitionTo("GS_SELECTMENU", true, false);
        creditsMenu->ResetTextPosTimer();
        StopSample("creditsMenu");
    }

    return true;
}

bool OneLoneCoder_Platformer::GameState_Close(float fElapsedTime)
{
    DestroyAllDynamics();

    // Stop Audio
    waveEngine.StopAll();
    waveEngine.DestroyAudio();

    // End the olc pixel game Engine
    olc_Terminate();

    return true;
}

bool OneLoneCoder_Platformer::GameState_Transition(float fElapsedTime)
{
    if (bPlayTransitionSound)
    {
        pwTransition = waveEngine.PlayWaveform(&sndTransition);
        bPlayTransitionSound = false;
    }

    if (bDrawGame)
    {
        DrawGame(fElapsedTime, 0, 800, 600);
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

void OneLoneCoder_Platformer::AddProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner, bool breackableAgainstTiles, float fDrag, std::string sound, bool bouncy, std::string bounceSound, bool scenery, std::string effect, float effectDuration, bool pickable, bool reflectible)
{
    cDynamicProjectile* p = new cDynamicProjectile(ox, oy, bFriend, velx, vely, duration, mapDecProjectiles[sprite], affectedByGravity, damage, solidVSMap, oneHit, sprite, corner, breackableAgainstTiles, fDrag, bouncy, bounceSound, scenery);
    if (sound != "")
        p->SetSoundEffect(sound);
    if (effect != "")
        p->SetEffect(effect, effectDuration);
    p->SetPickable(pickable);
    p->SetReflectible(reflectible);
    vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit, int corner, std::string sound, bool scenery)
{
    cDynamicProjectile* p = new cDynamicProjectileBoomerang(ox, oy, bFriend, velx, vely, duration, mapDecProjectiles[sprite], damage, solidVSMap, oneHit, corner, scenery, sprite);
    if (sound != "")
        p->SetSoundEffect(sound);
    vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddOrbital(float ox, float oy, bool bFriend, float duration, std::string sprite, int damage, bool solidVSMap, bool oneHit, int corner, float cx, float cy, float angrot, std::string sound, bool scenery)
{
    cDynamicProjectile* p = new cDynamicProjectileOrbital(ox, oy, bFriend, duration, mapDecProjectiles[sprite], damage, solidVSMap, oneHit, corner, cx, cy, angrot, scenery, sprite);
    if (sound != "")
        p->SetSoundEffect(sound);
    vecProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddPreparedProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::string sprite, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner, bool breackableAgainstTiles, float fDrag, std::string sound, bool bouncy, std::string bounceSound, bool scenery)
{
    cDynamicProjectile* p = new cDynamicProjectile(ox, oy, bFriend, velx, vely, duration, mapDecProjectiles[sprite], affectedByGravity, damage, solidVSMap, oneHit, sprite, corner, breackableAgainstTiles, fDrag, bouncy, bounceSound, scenery);
    vecPreparedProjectiles.push_back(p);
}

void OneLoneCoder_Platformer::AddPlatform(float ox, float oy, std::string sprite, std::wstring iced)
{
    cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatform(ox, oy, mapDecPlatforms[sprite], iced);
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddScenery(float ox, float oy, std::string sprite, bool foreground)
{
    cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatform(ox, oy, mapDecPlatforms[sprite]);
    ptfm->SetForeground(foreground);
    
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger)
{
    cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformHorSine(ox, oy, mapDecPlatforms[sprite], iced, amplitude, frequency, trigger);
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalSinePtfm(float ox, float oy, std::string sprite, std::wstring iced, float amplitude, float frequency, std::wstring trigger)
{
    cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformVerSine(ox, oy, mapDecPlatforms[sprite], iced, amplitude, frequency, trigger);
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float tx, float vx, std::wstring trigger)
{
    cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformHorSingle(ox, oy, mapDecPlatforms[sprite], iced, tx, vx, trigger);
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalSinglePtfm(float ox, float oy, std::string sprite, std::wstring iced, float ty, float vy, std::wstring trigger)
{
    cDynamicMovingPlatform* ptfm = new cDynamicMovingPlatformVerSingle(ox, oy, mapDecPlatforms[sprite], iced, ty, vy, trigger);
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddWall(float ox, float oy, std::string sprite, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY, std::string id)
{
    cDynamicMovingPlatform* ptfm = new cDynamicWall(ox, oy, mapDecPlatforms[sprite], leftSolid, rightSolid, trigger, trgX, trgY);
    if (id != "")
    {
        ptfm->SetIdentifier(id);
    }
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddCeiling(float ox, float oy, std::string sprite, std::wstring topSolid, std::wstring linkToPreviousPtfm)
{
    cDynamicMovingPlatform* ptfm = new cDynamicCeiling(ox, oy, mapDecPlatforms[sprite], topSolid, linkToPreviousPtfm);
    if (ptfm->IsLinkedToPreviousPtfm())
    {
        ptfm->LinkPtfm(vecPlatforms.front());
    }
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHorizontalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime)
{
    cDynamicMovingPlatform* ptfm = new cDynamicHorizontalCrusher(ox, oy, mapDecPlatforms[sprite], side, waitTime);
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddVerticalCrusher(float ox, float oy, std::string sprite, std::wstring side, float waitTime)
{
    cDynamicMovingPlatform* ptfm = new cDynamicVerticalCrusher(ox, oy, mapDecPlatforms[sprite], side, waitTime);
    vecPlatforms.push_back(ptfm);
}

void OneLoneCoder_Platformer::AddHarmfulBloc(float ox, float oy, std::string sprite, float dmg, std::wstring tangible)
{
    cDynamicMovingPlatform* ptfm = new cDynamicHarmfulBloc(ox, oy, mapDecPlatforms[sprite], dmg, tangible);
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
		if (fabs(px - (ptfm->GetPX()										  )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (ptfm->GetPY()										    )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (ptfm->GetPX() + ptfm->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (ptfm->GetPY()										    )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (ptfm->GetPX()										  )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (ptfm->GetPY() + ptfm->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)) ||
            fabs(px - (ptfm->GetPX() + ptfm->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (ptfm->GetPY() + ptfm->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)))
        {
            closePtfms.push_back(ptfm);
        }
    }
    return closePtfms;
}

olc::Decal* OneLoneCoder_Platformer::GetDoorSwitch(bool on)
{
    return on ? decDoorSwitchOn : decDoorSwitchOff;
}

void OneLoneCoder_Platformer::AddWind(float ox, float oy, std::string sprite, std::wstring direction, float power)
{
    cDynamicWind* wind = new cDynamicWind(ox, oy, mapDecWinds[sprite], direction, power);
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
		if (fabs(px - (wind->GetPX()										  )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (wind->GetPY()										    )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (wind->GetPX() + wind->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (wind->GetPY()										    )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (wind->GetPX()										  )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (wind->GetPY() + wind->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)) ||
            fabs(px - (wind->GetPX() + wind->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (wind->GetPY() + wind->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)))
        {
            closeWinds.push_back(wind);
        }
    }
    return closeWinds;
}

void OneLoneCoder_Platformer::AddTeleport(float ax, float ay, float bx, float by, std::string sprite)
{
    cDynamicTeleport* tp = new cDynamicTeleport(ax, ay, bx, by, mapDecTeleports[sprite]);
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
		if (fabs(px - (tp->GetPX()										  )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetPY()										    )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (tp->GetPX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetPY()										    )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (tp->GetPX()										  )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetPY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)) ||
            fabs(px - (tp->GetPX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetPY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)))
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
		if (fabs(px - (tp->GetDestX()										 )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetDestY()										  )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (tp->GetDestX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetDestY()										  )) < ((((float)ScreenHeight()) / 64.0f)) ||
			fabs(px - (tp->GetDestX()										 )) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetDestY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)) ||
            fabs(px - (tp->GetDestX() + tp->GetCurrentSprite()->width / 64.0f)) < ((((float)ScreenWidth()) / 64.0f)) && fabs(py - (tp->GetDestY() + tp->GetCurrentSprite()->height / 64.0f)) < ((((float)ScreenHeight()) / 64.0f)))
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
	if		(gameState == "GS_LOADING")		  nGameState = GS_LOADING;
    else if (gameState == "GS_TITLE")		  nGameState = GS_TITLE;
    else if (gameState == "GS_MAIN")		  nGameState = GS_MAIN;
    else if (gameState == "GS_MANUAL")	      nGameState = GS_MANUAL;
    else if (gameState == "GS_LEVELSTART")	  nGameState = GS_LEVELSTART;
    else if (gameState == "GS_LOADLEVEL")	  nGameState = GS_LOADLEVEL;
    else if (gameState == "GS_WORLDMAP")	  nGameState = GS_WORLDMAP;
    else if (gameState == "GS_ENDSCREEN")	  nGameState = GS_ENDSCREEN;
    else if (gameState == "GS_PAUSE")		  nGameState = GS_PAUSE;
    else if (gameState == "GS_LOADBOSSLEVEL") nGameState = GS_LOADBOSSLEVEL;
    else if (gameState == "GS_SELECTMENU")	  nGameState = GS_SELECTMENU;
    else if (gameState == "GS_CONTROLS")	  nGameState = GS_CONTROLS;
    else if (gameState == "GS_SOUNDS")		  nGameState = GS_SOUNDS;
    else if (gameState == "GS_SCREENMODE")	  nGameState = GS_SCREENMODE;
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

olc::Decal* OneLoneCoder_Platformer::GetBackGroundDecal()
{
    return decBackground;
}

olc::Decal* OneLoneCoder_Platformer::GetParallax1Decal()
{
    return decParallax1;
}

olc::Decal* OneLoneCoder_Platformer::GetParallax2Decal()
{
    return decParallax2;
}

olc::Decal* OneLoneCoder_Platformer::GetParallax3Decal()
{
    return decParallax3;
}

olc::Decal* OneLoneCoder_Platformer::GetSpecialTilesDecal()
{
    return decSpecialTiles;
}

olc::Decal* OneLoneCoder_Platformer::GetGroundTilesDecal()
{
    return decGrdTiles;
}

olc::Decal* OneLoneCoder_Platformer::GetDoorDecal()
{
    return decDoor;
}

void OneLoneCoder_Platformer::ReturnToWorldMap(bool drawGame)
{
    waveEngine.StopAll();
    pwWorldMap = waveEngine.PlayWaveform(&sndWorldMap, true);
    WindEffect(0.0f, 0.0f, false);
    animPlayer.ChangeState("riding_star", &sprPlayer, decPlayer);
    TransitionTo("GS_WORLDMAP", true, drawGame);
    player->SetDamageBooster(1);
    player->SetDefenseBooster(1);
    player->SetCandyPower(false);
    player->SetAttackable(true);
}

void OneLoneCoder_Platformer::GoToControlsMenu()
{
    waveEngine.StopAll();
    pwMenu = waveEngine.PlayWaveform(&sndMenu, true);
    TransitionTo("GS_CONTROLS", true, false);
}

void OneLoneCoder_Platformer::GoToSoundMenu()
{
    waveEngine.StopAll();
    pwMenu = waveEngine.PlayWaveform(&sndMenu, true);
    TransitionTo("GS_SOUNDS", true, false);
}

void OneLoneCoder_Platformer::GoToScreenModeMenu()
{
    waveEngine.StopAll();
    pwMenu = waveEngine.PlayWaveform(&sndMenu, true);
    TransitionTo("GS_SCREENMODE", true, false);
}

void OneLoneCoder_Platformer::HitStop()
{
    fHitStop = cfHitStopDuration;
}

void OneLoneCoder_Platformer::BossHitStop()
{
    fHitStop = cfBossHitStopDuration;
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
        if (!waveEngine.IsWaveformPlaying(&sndWind))
            pwWind = waveEngine.PlayWaveform(&sndWind, true);
    }
    else
    {
        if (waveEngine.IsWaveformPlaying(&sndWind))
            waveEngine.StopWaveform(pwWind);
    }
}

void OneLoneCoder_Platformer::HitShake()
{
    camera->HitShake();
}

void OneLoneCoder_Platformer::AddSharedSound(std::string name, olc::sound::Wave* sound, std::string fileName)
{
    sound->LoadAudioWaveform(fileName);
    sharedSounds[name] = sound;
}

void OneLoneCoder_Platformer::PlaySample(std::string name, bool loop, bool dontPlayIfAlreadyPlaying)
{
    if (dontPlayIfAlreadyPlaying)
    {
        if (!waveEngine.IsWaveformPlaying(sharedSounds[name]))
        {
            sharedPlaying[name] = waveEngine.PlayWaveform(sharedSounds[name]);
        }
    }
    else
    {
        sharedPlaying[name] = waveEngine.PlayWaveform(sharedSounds[name]);
    }
}

void OneLoneCoder_Platformer::StopSample(std::string name)
{
    try
    {
        if (waveEngine.IsWaveformPlaying(sharedSounds[name]))
            waveEngine.StopWaveform(sharedPlaying[name]);
    }
    catch (const std::exception&)
    {
    }
}

bool OneLoneCoder_Platformer::IsSamplePlaying(std::string name)
{
    return waveEngine.IsWaveformPlaying(sharedSounds[name]);
}

void OneLoneCoder_Platformer::PlayLevelMusic()
{
    pwLevelMusic = waveEngine.PlayWaveform(&sndLevelMusic, true);
}

void OneLoneCoder_Platformer::StopLevelMusic()
{
    if (waveEngine.IsWaveformPlaying(&sndLevelMusic))
        waveEngine.StopWaveform(pwLevelMusic);
}

void OneLoneCoder_Platformer::UpdateVolume(int volume)
{
    waveEngine.SetOutputVolume((float)volume / 10.0f);
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
    player->ChangeAnimation(animation, &sprPlayer, decPlayer);
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
    pwInvincibility = waveEngine.PlayWaveform(&sndInvincibility);

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

void OneLoneCoder_Platformer::ProjectPlayer(float vx, float vy)
{
    player->SetVelocities(vx, vy);
    player->SetProjected(true);
}

void OneLoneCoder_Platformer::StopProjectPlayer()
{
    player->SetProjected(false);
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
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);

    std::string un = username;

    std::wifstream file("C:/Users/" + un + "/AppData/Roaming/Kirbo Superstar/controls.txt");

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

    if (key == "0") k = olc::Key::K0;
    if (key == "1") k = olc::Key::K1;
    if (key == "2") k = olc::Key::K2;
    if (key == "3") k = olc::Key::K3;
    if (key == "4") k = olc::Key::K4;
    if (key == "5") k = olc::Key::K5;
    if (key == "6") k = olc::Key::K6;
    if (key == "7") k = olc::Key::K7;
    if (key == "8") k = olc::Key::K8;
    if (key == "9") k = olc::Key::K9;

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

    if (key == olc::Key::K0) str = "0";
    if (key == olc::Key::K1) str = "1";
    if (key == olc::Key::K2) str = "2";
    if (key == olc::Key::K3) str = "3";
    if (key == olc::Key::K4) str = "4";
    if (key == olc::Key::K5) str = "5";
    if (key == olc::Key::K6) str = "6";
    if (key == olc::Key::K7) str = "7";
    if (key == olc::Key::K8) str = "8";
    if (key == olc::Key::K9) str = "9";

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

void OneLoneCoder_Platformer::TransitionTo(std::string newState, bool playTransitionSound, bool drawGame)
{
    bPlayTransitionSound = playTransitionSound;
    bDrawGame = drawGame;
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

std::vector<cDynamicCreature*> OneLoneCoder_Platformer::GetCloseEnnemies(float px, float py)
{
    std::vector<cDynamicCreature*> closeEnnemies;
    for (auto& mob : vecEnnemies)
    {
        // Check the 4 corners to avoid the item disappearing when player is far from top left corner
        // -----------------------------------------------------------------
        // ------ /!\ FPS drops considerably if item is too large /!\ ------
        // -----------------------------------------------------------------
		if (fabs(px - (mob->GetPX()						   )) < ((((float)ScreenWidth()) / 64.0f) * 1.25f) && fabs(py - (mob->GetPY()						 )) < ((((float)ScreenHeight()) / 64.0f) * 1.25f) ||
			fabs(px - (mob->GetPX() + mob->GetNormalizedW())) < ((((float)ScreenWidth()) / 64.0f) * 1.25f) && fabs(py - (mob->GetPY()						 )) < ((((float)ScreenHeight()) / 64.0f) * 1.25f) ||
			fabs(px - (mob->GetPX()						   )) < ((((float)ScreenWidth()) / 64.0f) * 1.25f) && fabs(py - (mob->GetPY() + mob->GetNormalizedH())) < ((((float)ScreenHeight()) / 64.0f) * 1.25f) ||
            fabs(px - (mob->GetPX() + mob->GetNormalizedW())) < ((((float)ScreenWidth()) / 64.0f) * 1.25f) && fabs(py - (mob->GetPY() + mob->GetNormalizedH())) < ((((float)ScreenHeight()) / 64.0f) * 1.25f))
        {
            closeEnnemies.push_back(mob);
        }
    }
    return closeEnnemies;
}

void OneLoneCoder_Platformer::AddNewWaddleDee(int px, int py, bool alwaysGeneratePickUp)
{
    cDynamicCreature* mob = new cDynamicCreatureWaddleDee(level);
    vecEnnemies.push_back(mob);
    mob->InitDecal();
    mob->SetPosition(px, py);
    mob->AlwaysGeneratePickUp(alwaysGeneratePickUp);
}

void OneLoneCoder_Platformer::NukeAllEnnemies()
{
    if (vecEnnemies.size() != 0)
    {
        vecEnnemies.clear();
    }
}

std::string OneLoneCoder_Platformer::ToStr(std::wstring str)
{
    std::wstring w(str);
    std::string result;
    for (char x : w)
        result += x;

    return result;
}

void OneLoneCoder_Platformer::UpdateGame(float fElapsedTime, float* angle, float* offsetX, float* offsetY)
{
    // Stop time a while before dead animation
    if (player->IsDead())
    {
        // stop music
        if (waveEngine.IsWaveformPlaying(&sndLevelMusic))
            waveEngine.StopWaveform(pwLevelMusic);
        if (waveEngine.IsWaveformPlaying(&sndBossLevelMusic))
            waveEngine.StopWaveform(pwBossLevelMusic);
        if (waveEngine.IsWaveformPlaying(&sndInvincibility))
            waveEngine.StopWaveform(pwInvincibility);

        fStopTimebeforeDeadAnim += fElapsedTime;

        if (fStopTimebeforeDeadAnim < cfStopTimebeforeDeadAnim)
            return;
    }

    if (fHitStop > 0.0f)
    {
        fHitStop -= fElapsedTime;
        return;
    }

    animPlayer.Update(fElapsedTime, &sprPlayer, decPlayer);

    player->HandleInput(fElapsedTime, camera, level, this, &sprPlayer, decPlayer);
    // Handle pause button pressed
    if (bBreakLoop)
    {
        bBreakLoop = false;
        return;
    }

    camera->ClampOffset();

    player->ApplyGravity(fElapsedTime, this);

    player->Update(fElapsedTime, this, &sprPlayer, decPlayer);

    player->OneCycleAnimations(fElapsedTime, *angle, *offsetX, *offsetY, mapProjectiles, level, this, &sprPlayer, decPlayer);
    // Handle State Change
    if (bBreakLoop)
    {
        bBreakLoop = false;
        return;
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
        player->Collisions(fElapsedTime, level, this, &sprPlayer, decPlayer);

    camera->SetPositions(player->GetPosX(), player->GetPosY());

    camera->CalculateFOV(level, this, fElapsedTime);

    player->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY(), this);

    camera->SpawnSceneries(level, fElapsedTime, this);

    checkPoint->Update(level, player, vecPlatforms);

    // Ennemies
    for (auto& object : vecEnnemies)
    {
        object->Collision(fElapsedTime);

        // Check if the ennemi is in the vacuum
        if (object->IsVacuumable())
        {
            if (player->IsVacuuming())
            {
                player->Vacuum(object, camera->GetOffsetX(), camera->GetOffsetY(), this);
            }
            else
            {
                object->Vacuumed(false);
                object->SetSwallowable(false);
            }
        }

        // Check collision with player to damage him
        if ((player->IsAttackable() && !player->IsSwallowing() && !object->IsVacuumed() && !player->IsDead()) || player->HasCandyPower())
        {
            player->EnemyCollision(object, camera->GetOffsetX(), camera->GetOffsetY(), this, &sprPlayer, decPlayer);
        }
    }

    // Projectiles
    for (auto& object : vecProjectiles)
    {
        object->Collision(fElapsedTime, level);

        // check if a Projectile hits a creature
        if (!object->IsRedundant())
        {
            if (!object->IsScenery())
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
                                object->SpawnEffect(dyn->GetPX() + (dyn->GetNormalizedW() / 2.0f) - 1.1f, dyn->GetPY() + (dyn->GetNormalizedH() / 2.0f) - 1.5f); // hardcoded values because it's only used for jesus cross
                                player->Attack(dyn, object->GetDamage());
                                if (object->IsOneHit())
                                    object->SetRedundant(true);
                            }
                        }
                    }

                    // Reflect projectiles
                    for (auto& enemyProj : vecProjectiles)
                    {
                        if (!enemyProj->IsRedundant() && !enemyProj->IsScenery() && !enemyProj->IsFriendly() && enemyProj->IsReflectible())
                        {
                            enemyProj->UpdateHitbox(camera->GetOffsetX(), camera->GetOffsetY());
                            if (cHitbox::ShapeOverlap_DIAG(object->GetHitbox(), enemyProj->GetHitbox()))
                            {
                                enemyProj->SetFriendly(true);
                                if (object->GetVX() * enemyProj->GetVX() <= 0.0f)
                                {
                                    object->PlaySoundEffect();
                                    object->SpawnEffect(enemyProj->GetPX() + (enemyProj->GetNormalizedW() / 2.0f) - 1.1f, enemyProj->GetPY() + (enemyProj->GetNormalizedH() / 2.0f) - 1.5f); // hardcoded values because it's only used for jesus cross
                                    enemyProj->SetVX(-enemyProj->GetVX());
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (player->IsAttackable())
                    {
                        player->EnemyCollision(object, camera->GetOffsetX(), camera->GetOffsetY(), this, &sprPlayer, decPlayer);
                    }
                }
            }
            else if (object->IsPickable())
            {
                player->PickupGrabbed(object, camera->GetOffsetX(), camera->GetOffsetY(), this);
            }
        }
    }

    for (auto& object : vecPreparedProjectiles)
    {
        vecProjectiles.push_back(object);
    }
    vecPreparedProjectiles.clear();

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
        for (auto& dyn : vecEnnemies)
        {
            if (dyn->IsSwallowable())
            {
                dyn->GeneratePickUp();
            }
        }

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

    if (bInBossLvl && vecEnnemies.empty() && !player->IsDead())
    {
        // Wait a little before begin win animation
        fWaitBeforeWinAnimation += fElapsedTime;

        if (fWaitBeforeWinAnimation >= cfTimeBeforeWinAnimation)
        {
            // do it only once
            if (!bBossKilled)
            {
                waveEngine.StopAll();
                pwBossKilled = waveEngine.PlayWaveform(&sndBossKilled);
            }

            bBossKilled = true;
            if (fWinTimer < animPlayer.mapStates["boss_killed"].size() * animPlayer.fTimeBetweenFrames)
                animPlayer.ChangeState("boss_killed", &sprPlayer, decPlayer);
        }

        // When win animation is over, quit the level
        if (fWinTimer >= cfBossKilledAnimation)
        {
            if (level->IsLastOfGame())
            {
                TransitionTo("GS_ENDSCREEN", true, true);

                return;
            }
            else if (level->IsLastUnlocked())
            {
                level->UnlockNewLvl();

                worldMap->SetUnlockedLevel(level->GetUnlockedLvl());
            }
            ReturnToWorldMap(true);

            return;
        }
    }
}

void OneLoneCoder_Platformer::DrawGame(float fElapsedTime, float angle, float offsetX, float offsetY)
{
    // Draw Background
    camera->DrawBackground(level, this, bInBossLvl);

    // Draw Platforms
    for (auto& object : GetClosePlatforms(player->GetPosX(), player->GetPosY()))
    {
        if (!object->IsForeground())
        {
            object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
        }
    }

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

    // Draw Wind
    for (auto& object : GetCloseWinds(player->GetPosX(), player->GetPosY()))
    {
        object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
    }

    // Draw tiles
    camera->DrawLevel(level);

    // Draw Ennemies
    for (auto& object : GetCloseEnnemies(player->GetPosX(), player->GetPosY()))
    {
        object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
    }

    // Draw Projectiles
    for (auto& object : vecProjectiles)
    {
        object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
    }

    // Draw HUD if player close to a door
    player->DrawDoorCommand(level, this, camera->GetOffsetX(), camera->GetOffsetY());

    // Draw Player
    player->UpdateInvulnerability(fElapsedTime, this);
    player->DrawKirbo((player->GetPosX() - camera->GetOffsetX()) * nTileWidth + (nTileWidth / 2) + offsetX, (player->GetPosY() - camera->GetOffsetY()) * nTileHeight + (nTileHeight / 2) + offsetY, angle, player->GetFaceDir(), this, decPlayer);
    
    // Draw foreground same plan items
    for (auto& object : GetClosePlatforms(player->GetPosX(), player->GetPosY()))
    {
        if (object->IsForeground())
        {
            object->DrawSelf(camera->GetOffsetX(), camera->GetOffsetY());
        }
    }

    // Draw foreground parallax
    if (!bInBossLvl)
    {
        camera->DrawForeground(level, this);
    }

    // Draw HUD
    HUD->HealthBar(this, decHealthBar);
    HUD->HealthPoints(this, decHealthPoint, player->GetHealth());

    if (bInBossLvl)
        HUD->BossHealthBar(this, decBossHealthBar, vecEnnemies);
    if (player->HasDamageBooster())
        HUD->DamageBoost(this, decSpecialTiles);
    if (player->HasDefenseBooster())
        HUD->DefenseBoost(this, decSpecialTiles);
}

olc::Sprite* OneLoneCoder_Platformer::GetLoadedSprite(std::string spr)
{
    if (spr == "controllerA") return &sprA;
    if (spr == "controllerB") return &sprB;
    if (spr == "controllerX") return &sprX;
    if (spr == "controllerY") return &sprY;
    if (spr == "controllerP") return &sprPause;
    if (spr == "controllerU") return &sprUp;
    if (spr == "controllerD") return &sprDown;
    if (spr == "controllerL") return &sprLeft;
    if (spr == "controllerR") return &sprRight;

    if (spr == "ControlsMenuBckGrd") return &sprControlsMenu;

    if (spr == "doorUp") return &sprDoorUp;
}

olc::Decal* OneLoneCoder_Platformer::GetLoadedDecal(std::string dec)
{
    if (dec == "doorUp") return decDoorUp;
}
