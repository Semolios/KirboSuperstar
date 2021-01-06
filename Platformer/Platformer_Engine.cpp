#include "Platformer_Engine.h"

OneLoneCoder_Platformer::OneLoneCoder_Platformer()
{
	sAppName = "Tile Based Platform Game";
}

bool OneLoneCoder_Platformer::OnUserCreate()
{
	animPlayer.ChangeState("idle");

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

	levels.push_back("assets/lvls/lvl1.txt");
	levels.push_back("assets/lvls/lvl2.txt");
	levels.push_back("assets/lvls/lvl3.txt");
	levels.push_back("assets/lvls/lvl4.txt");
	levels.push_back("assets/lvls/lvl5.txt");
	levels.push_back("assets/lvls/lvl6.txt");

	bossLevels.push_back("assets/lvls/Bosslvl1.txt");
	bossLevels.push_back("assets/lvls/Bosslvl2.txt");
	bossLevels.push_back("assets/lvls/Bosslvl3.txt");
	bossLevels.push_back("assets/lvls/Bosslvl4.txt");
	bossLevels.push_back("assets/lvls/Bosslvl5.txt");
	bossLevels.push_back("assets/lvls/Bosslvl6.txt");

	levelsEnnemies.push_back("assets/lvls/ennemiesLvl1.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl2.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl3.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl4.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl5.txt");
	levelsEnnemies.push_back("assets/lvls/ennemiesLvl6.txt");

	levelsTiles.push_back("assets/gfx/tilemap00.png");
	levelsTiles.push_back("assets/gfx/tilemap01.png");
	levelsTiles.push_back("assets/gfx/tilemap02.png");
	levelsTiles.push_back("assets/gfx/tilemap03.png");
	levelsTiles.push_back("assets/gfx/tilemap04.png");
	levelsTiles.push_back("assets/gfx/tilemap05.png");

	groundTiles.push_back("assets/gfx/grdTileMap00.png");
	groundTiles.push_back("assets/gfx/grdTileMap01.png");
	groundTiles.push_back("assets/gfx/grdTileMap02.png");
	groundTiles.push_back("assets/gfx/grdTileMap03.png");
	groundTiles.push_back("assets/gfx/grdTileMap04.png");
	groundTiles.push_back("assets/gfx/grdTileMap05.png");

	levelsBackgrounds.push_back("assets/gfx/BckGrd00.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd01.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd02.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd03.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd04.png");
	levelsBackgrounds.push_back("assets/gfx/BckGrd05.png");

	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd00.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd01.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd02.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd03.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd04.png");
	bossLevelsBackgrounds.push_back("assets/gfx/BossBckGrd05.png");

	currentLvl = new cLevel();
	cLevel::engine = this;
	sprDoor = new olc::Sprite("assets/gfx/door.png");

#pragma endregion

#pragma region Player Sprites

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
	animPlayer.mapStates["slap"].push_back(new olc::Sprite("assets/gfx/slap01.png"));
	animPlayer.mapStates["slap"].push_back(new olc::Sprite("assets/gfx/slap01.png"));

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

	animPlayer.mapStates["dead"].push_back(new olc::Sprite("assets/gfx/kirboDead.png"));

	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying00.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying01.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying02.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying03.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying02.png"));
	animPlayer.mapStates["flying"].push_back(new olc::Sprite("assets/gfx/kirboFlying01.png"));

	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross00.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross01.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross00.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross01.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross02.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross02.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross03.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross04.png"));
	animPlayer.mapStates["jesus_christ"].push_back(new olc::Sprite("assets/gfx/kirbolaunchingcross04.png"));

	animPlayer.mapStates["begin_vacuum"].push_back(new olc::Sprite("assets/gfx/kirboBeginVacuum00.png"));
	animPlayer.mapStates["begin_vacuum"].push_back(new olc::Sprite("assets/gfx/kirboBeginVacuum01.png"));

	animPlayer.mapStates["vacuum"].push_back(new olc::Sprite("assets/gfx/kirboVacuum00.png"));
	animPlayer.mapStates["vacuum"].push_back(new olc::Sprite("assets/gfx/kirboVacuum01.png"));

	animPlayer.mapStates["swallow"].push_back(new olc::Sprite("assets/gfx/kirboSwallow00.png"));
	animPlayer.mapStates["swallow"].push_back(new olc::Sprite("assets/gfx/kirboSwallow01.png"));
	animPlayer.mapStates["swallow"].push_back(new olc::Sprite("assets/gfx/kirboSwallow02.png"));
	animPlayer.mapStates["swallow"].push_back(new olc::Sprite("assets/gfx/kirboSwallow03.png"));
	animPlayer.mapStates["swallow"].push_back(new olc::Sprite("assets/gfx/kirboSwallow04.png"));
	animPlayer.mapStates["swallow"].push_back(new olc::Sprite("assets/gfx/kirboSwallow04.png"));
	animPlayer.mapStates["swallow"].push_back(new olc::Sprite("assets/gfx/kirboSwallow04.png"));

	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed00.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed01.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed02.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed03.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed04.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed05.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed06.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed07.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed08.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed09.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed10.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed11.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed12.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed13.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed14.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed15.png"));
	animPlayer.mapStates["boss_killed"].push_back(new olc::Sprite("assets/gfx/boss_killed16.png"));

	animPlayer.mapStates["kirbo_goes_away"].push_back(new olc::Sprite("assets/gfx/boss_killed16.png"));

#pragma endregion

#pragma region Projectiles sprites

	mapProjectiles["jesuscross"].push_back(new olc::Sprite("assets/gfx/jesuscross.png"));

	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/explosion00.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/explosion01.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/explosion02.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/explosion03.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/explosion04.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/explosion05.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/explosion06.png"));

	mapProjectiles["slapAOE"].push_back(new olc::Sprite("assets/gfx/slapAOE.png"));

	mapProjectiles["swordDownAOE"].push_back(new olc::Sprite("assets/gfx/swordDownAOE00.png"));
	mapProjectiles["swordDownAOE"].push_back(new olc::Sprite("assets/gfx/swordDownAOE01.png"));
	mapProjectiles["swordDownAOE"].push_back(new olc::Sprite("assets/gfx/swordDownAOE02.png"));

	mapProjectiles["swordUpAOE"].push_back(new olc::Sprite("assets/gfx/swordUpAOE.png"));

	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion00.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion01.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion02.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion03.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion04.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion05.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion06.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion07.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion08.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion09.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion10.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion11.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion12.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion13.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion14.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion15.png"));
	mapProjectiles["bossExplosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion16.png"));

	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround00.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround01.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround02.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround03.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround04.png"));

	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root00.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root02.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root03.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root04.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root05.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root06.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root07.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root04.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root03.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root02.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root01.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root00.png"));

	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple00.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple01.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple02.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple03.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple04.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple05.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple06.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple07.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple08.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple09.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple10.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple11.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple12.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple13.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple14.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple15.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple16.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple17.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple18.png"));

	mapProjectiles["blow"].push_back(new olc::Sprite("assets/gfx/blow.png"));

	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava00.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava01.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava02.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava03.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava04.png"));

	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma00.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma01.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma02.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma03.png"));

	mapProjectiles["magmaBoulder"].push_back(new olc::Sprite("assets/gfx/magmaBoulder.png"));

	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser00.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser01.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser02.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser03.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser04.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser05.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser06.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser07.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser08.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser09.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser10.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser11.png"));

	mapProjectiles["laser"].push_back(new olc::Sprite("assets/gfx/laser00.png"));
	mapProjectiles["laser"].push_back(new olc::Sprite("assets/gfx/laser01.png"));

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
	if (currentLvl->LoadLevel(levels[nCurrentLevel]))
	{
		LoadLevelProperties();

		currentLvl->PopulateEnnemies(vecEnnemies, levelsEnnemies[nCurrentLevel]);

		spriteTiles = new olc::Sprite(levelsTiles[nCurrentLevel]);
		sprGrdTiles = new olc::Sprite(groundTiles[nCurrentLevel]);
		sprBackground = new olc::Sprite(levelsBackgrounds[nCurrentLevel]);
	}

	// Reset variables when level is loading
	fHealth = cfMaxHealth;
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
	if (bDead)
	{
		fStopTimebeforeDeadAnim += fElapsedTime;

		if (fStopTimebeforeDeadAnim < cfStopTimebeforeDeadAnim)
			return true;
	}

	animPlayer.Update(fElapsedTime);

	// Handle input
	if (IsFocused() && CanInteract())
	{
		// Fly, enter a door
		if (GetKey(olc::Key::UP).bHeld)
		{
			if (!bAttacking && !bVacuuming)
			{
				if (GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f) == L'w' && bPlayerOnGround)
					nGameState = GS_LOADBOSSLEVEL;

				fPlayerVelY = -cfPlayerVelY;
				bFlying = true;
				animPlayer.ChangeState("flying");
			}
		}

		// Go down when flying, cross semi solid platform and control camera when onground
		if (GetKey(olc::Key::DOWN).bHeld)
		{
			if (bFlying)
				fPlayerVelY = cfPlayerVelY;

			// If player is on semi solid platform, pass through the platform. cheat a little bit, modify the position of the player to cross it
			if ((IsSemiSolidTile(GetTile(fPlayerPosX + 0.0f, fPlayerPosY + 1.0f)) || IsSemiSolidTile(GetTile(fPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f))) && bPlayerOnGround)
				fPlayerPosY += 0.15;

			if ((IsSolidTile(GetTile(fPlayerPosX + 0.0f, fPlayerPosY + 1.0f)) || IsSolidTile(GetTile(fPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f))) && bPlayerOnGround)
				fCameraLookingDown -= cfCameraMoveSpeed;
		}
		else
		{
			fCameraLookingDown += cfCameraMoveSpeed;
		}

		// Go left
		if (GetKey(olc::Key::LEFT).bHeld)
		{
			if (!bAttacking && !bVacuuming)
			{
				// Init speed by cfMinPlayerVelX + 0.05 or the player won't move when on ground
				if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX -= (cfMinPlayerVelX + 0.05f);

				fPlayerVelX += (bPlayerOnGround ? -cfPlayerAccGrdX : -cfPlayerAccAirX) * fElapsedTime;
				fFaceDir = -1.0f;
			}
		}

		// Go right
		if (GetKey(olc::Key::RIGHT).bHeld)
		{
			if (!bAttacking && !bVacuuming)
			{
				// Init speed by cfMinPlayerVelX + 0.05 or the player won't move when on ground
				if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX += (cfMinPlayerVelX + 0.05f);

				fPlayerVelX += (bPlayerOnGround ? cfPlayerAccGrdX : cfPlayerAccAirX) * fElapsedTime;
				fFaceDir = 1.0f;
			}
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
				bChargeJump = true;
			}
			else if (bDoubleJump && fPlayerVelY > 0)
			{
				bDoubleJump = false;
				bChargeDoubleJump = true;
			}
		}

		// The more you hold, the higher you go
		if (GetKey(olc::Key::SPACE).bHeld)
		{
			if (bChargeJump)
			{
				if (fPlayerVelY >= cfPlayerJumpMinAcc) fPlayerVelY = cfPlayerJumpMinAcc;
				fPlayerVelY += cfJumpIncrement;
				if (fPlayerVelY <= cfPlayerJumpMaxAcc)
				{
					bChargeJump = false;
				}
			}
			else if (bChargeDoubleJump)
			{
				if (fPlayerVelY >= cfPlayerJumpMinAcc) fPlayerVelY = cfPlayerJumpMinAcc;
				fPlayerVelY += cfJumpIncrement;
				if (fPlayerVelY <= cfPlayerDblJumpMaxAcc)
				{
					bChargeDoubleJump = false;
				}
			}
		}

		// if you release space, jump is cancelled so you can't spam space to glide
		if (GetKey(olc::Key::SPACE).bReleased)
		{
			if (bChargeJump) bChargeJump = false;
			if (bChargeDoubleJump) bChargeDoubleJump = false;
		}

		// Slap attack
		if (GetKey(olc::Key::F).bPressed)
		{
			// Can't spam slap, can't slap when player is flying
			if (!bAttacking && !bFlying)
			{
				animPlayer.ChangeState("slap");
				bAttacking = true;
				bSlapping = true;
				bCanSpawnProjectile = true;
				fAnimationTimer = 0.0f;
			}
		}

		// Pause
		if (GetKey(olc::Key::P).bPressed)
		{
			pauseMenu->SetPlayerChoice(0);

			// You can't use pause when you are hit.
			// like you can't pause when you jump in mario sunshine so you can't leave the level when you are falling
			nGameState = GS_PAUSE;
			return true;
		}

		// Launch a Jesus Cross
		if (GetKey(olc::Key::R).bPressed)
		{
			// Can't spam Launching cross, can't launch when player is flying
			if (!bAttacking && !bFlying)
			{
				animPlayer.ChangeState("jesus_christ");
				bAttacking = true;
				bLaunchingJesusCross = true;
				bCanSpawnProjectile = true;
				fAnimationTimer = 0.0f;
			}
		}

		// Vacuum attack
		if (GetKey(olc::Key::E).bHeld)
		{
			// can't Vacuum when player is attacking, swallowing or flying
			if (!bFlying && !bSwallowing)
			{
				if (!bVacuuming && !bAttacking)
				{
					animPlayer.ChangeState("begin_vacuum");
					bVacuuming = true;
					fAnimationTimer = 0.0f;
				}
				bAttacking = true;
			}
			else
			{
				bVacuuming = false;
			}
		}
		else
		{
			bVacuuming = false;
		}
	}

	// Clamp camera offset
	if (fCameraLookingDown <= cfCameraLowerPos) fCameraLookingDown = cfCameraLowerPos;
	if (fCameraLookingDown >= cfCameraUpperPos) fCameraLookingDown = cfCameraUpperPos;

	// Gravity
	fPlayerVelY += cfGravity * fElapsedTime;

	if (bAttacking && CanInteract())
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

				if (CanInteract())
					animPlayer.ChangeState("idle");
			}
			else
			{
				if (CanInteract())
					animPlayer.ChangeState("run");
			}
		}
		else
		{
			if (CanInteract())
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
	t.Translate(((float)-nTileWidth / 2.0f) - cnSpriteOffsetX, ((float)-nTileWidth / 2.0f) - cnSpriteOffsetY);
	t.Scale(fFaceDir * 1.0f, 1.0f);

#pragma region ONE CYCLE ANIMATIONS

	if (bAttacking && !bDead)
	{
		// calculate elapsed time during attack
		fAnimationTimer += fElapsedTime;

		// Slap Attack
		if (bSlapping)
		{
			if (fAnimationTimer >= cfSlapSpawnT * animPlayer.fTimeBetweenFrames)
			{
				if (bCanSpawnProjectile)
				{
					// must offset the AOE so it goes from kirbo's hand
					float fProjectilePosX = fPlayerPosX + (fFaceDir > 0.0f ? 1.0f : -(mapProjectiles["slapAOE"][0]->width / (float)nTileWidth));
					float fProjectilePosY = fPlayerPosY - ((mapProjectiles["slapAOE"][0]->height - (float)nTileHeight) / (float)(2 * nTileHeight));
					cDynamicProjectile* p = CreateProjectile(fProjectilePosX, fProjectilePosY, true, fFaceDir, 0.0f, cfSlapDuration, "slapAOE", false, cnSlapDmg, false, false);
					p->bOneHit = false;
					AddProjectile(p);
					bCanSpawnProjectile = false;
				}
			}
		}

		// Launch a Jesus Cross
		if (bLaunchingJesusCross)
		{
			if (fAnimationTimer >= cfJesusCrossSpawnT * animPlayer.fTimeBetweenFrames)
			{
				if (bCanSpawnProjectile)
				{
					cDynamicProjectile* p = CreateProjectile((fPlayerPosX + fFaceDir), fPlayerPosY - 1.0f, true, cfJesusCrossVelX * fFaceDir, cfJesusCrossVelY, cfJesusCrossDuration, "jesuscross", true, cnJesusCrossDmg, true);
					AddProjectile(p);
					bCanSpawnProjectile = false;
				}
			}
		}

		// Vacuuming
		if (bVacuuming)
		{
			if (fAnimationTimer >= cfVacuumAnimT * animPlayer.fTimeBetweenFrames)
			{
				animPlayer.ChangeState("vacuum");
			}
		}

		// Swallowing
		if (bSwallowing)
		{
			animPlayer.ChangeState("swallow");
		}

		// Stop the attack when it's finished
		if (fAnimationTimer >= animPlayer.mapStates[animPlayer.sCurrentState].size() * animPlayer.fTimeBetweenFrames)
		{
			fAnimationTimer = 0.0f;
			StopAnyAttack();
		}
	}

	if (bPlayerDamaged && !bDead)
	{
		// calculate elapsed time after damage
		fAnimationTimer += fElapsedTime;
		bFlying = false;
		StopAnyAttack();

		if (fAnimationTimer >= animPlayer.mapStates["damaged"].size() * animPlayer.fTimeBetweenFrames)
		{
			fAnimationTimer = 0.0f;
			bPlayerDamaged = false;
		}
	}

	if (bDead)
	{
		fPlayerVelX = 0.0f;
		fPlayerVelY = 0.0f;
		StopAnyAttack();

		fDeadAnimation += fElapsedTime;
		if (fDeadAnimation != fElapsedTime)
		{
			t.Rotate(fDeadAnimation * cfDeadRotationAnimation);
			// animation based on a 2nd degree polynome to simulate kirbo's death animation
			t.Translate(0.0f, (4.0f * fDeadAnimation - 2.0f) * 64.0f * (4.0f * fDeadAnimation - 2.0f) - 4 * 64.0f);
		}

		// Return to the map after dead animation
		if (fDeadAnimation >= cfDeadAnimation)
		{
			// if you die in boss level, you reappear in the boss room
			if (bInBossLvl)
			{
				ResetVariables();
				fHealth = cfMaxHealth;
				nGameState = GS_LOADBOSSLEVEL;
				return true;
			}

			nGameState = GS_WORLDMAP;
			animPlayer.ChangeState("riding_star");
			return true;
		}
	}

	if (bBossKilled)
	{
		fPlayerVelX = 0.0f;
		fPlayerVelY = 0.0f;
		StopAnyAttack();

		fWinTimer += fElapsedTime;

		// after the dance, kirbo goes away
		if (fWinTimer >= animPlayer.mapStates["boss_killed"].size() * animPlayer.fTimeBetweenFrames)
		{
			fKirboGoesAwayTimer += fElapsedTime;
			animPlayer.ChangeState("kirbo_goes_away");

			fFaceDir = 1.0f;
			t.Scale(-1.0f, 1.0f); // Scale the sprite because riding_star00 sprite is facing left
			t.Rotate(-fKirboGoesAwayTimer * cfGoAwayRotationAnimation);
			t.Translate(fKirboGoesAwayTimer * cfGoAwayTranslationAnimation, -fKirboGoesAwayTimer * cfGoAwayTranslationAnimation);
		}
		else
		{
			fFaceDir = 1.0f;
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

	// Wind effect
	if (bWind)
		fPlayerVelX += fWindDirection * fWindPower * fElapsedTime;

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

	// Check hole
	if (fPlayerPosY > nLevelHeight)
	{
		fHealth = 0.0f;
		bDead = true;
		animPlayer.ChangeState("dead");
	}

	// Collision
	if (fPlayerVelX <= 0) // Moving Left
	{
		if (fNewPlayerPosX <= 1) fNewPlayerPosX = 1; // Prevent from being brutally moved to 0 only when reaching -1

		if (IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f)) || IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + fPlayerCollisionUpperLimit)))
		{
			fNewPlayerPosX = (int)fNewPlayerPosX + 1;
			fPlayerVelX = 0;
		}
	}
	else // Moving Right
	{
		if (fNewPlayerPosX >= nLevelWidth - 2) fNewPlayerPosX = nLevelWidth - 2; // Kirbo can't cross the edge of the map

		if (IsSolidTile(GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f)) || IsSolidTile(GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + fPlayerCollisionUpperLimit)))
		{
			fNewPlayerPosX = (int)fNewPlayerPosX;
			fPlayerVelX = 0;
		}
	}

	bPlayerOnGround = false;
	if (fPlayerVelY <= 0) // Moving Up
	{
		if (fNewPlayerPosY <= 1) fNewPlayerPosY = 1; // Prevent from being brutally moved to 0 only when reaching -1

		if (IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY)) || IsSolidTile(GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY)))
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
		if (IsSolidTile(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) || IsSolidTile(GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f)) ||
			((IsSemiSolidTile(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) || IsSemiSolidTile(GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f))) && fPlayerPosY + 1.0f < (float)((int)fNewPlayerPosY + 1.0f) + 0.1f))
		{
			fNewPlayerPosY = (int)fNewPlayerPosY + cfGrdDynamicOverlay; // Remove this line to create shifting sand
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
	float fOffsetY = fCameraPosY - (float)nVisibleTilesY * fCameraLookingDown;

	// Clamp camera to game boundaries
	if (fOffsetX < 1) fOffsetX = 1;
	if (fOffsetY < 1) fOffsetY = 1;
	if (fOffsetX > nLevelWidth - nVisibleTilesX - 1) fOffsetX = nLevelWidth - nVisibleTilesX - 1;
	if (fOffsetY > nLevelHeight - nVisibleTilesY - 1) fOffsetY = nLevelHeight - nVisibleTilesY - 1;

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
	float fBackgroundOffsetX = fOffsetX * (float)nTileWidth * ((float)(sprBackground->width - ScreenWidth()) / (float)(nLevelWidth * nTileWidth - ScreenWidth()));
	float fBackgroundOffsetY = fOffsetY * (float)nTileHeight * ((float)(sprBackground->height - ScreenHeight()) / (float)(nLevelHeight * nTileHeight - ScreenHeight()));
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
			if (bVacuuming)
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
					(fPlayerPosX + (fFaceDir > 0.0f ? 1.75f : -0.75f) - fOffsetX) * (float)nTileWidth,
					(fPlayerPosY + 0.5f - fOffsetY) * (float)nTileHeight
				}; // 1 block ahead the player's looking direction
				sVacuum.angle = 0.0f;
				sVacuum.o.push_back({ -(float)nTileWidth * 1.25f, -(float)nTileHeight / (fFaceDir > 0.0f ? 2.0f : 1.0f) });
				sVacuum.o.push_back({ -(float)nTileWidth * 1.25f, +(float)nTileHeight / (fFaceDir > 0.0f ? 2.0f : 1.0f) });
				sVacuum.o.push_back({ +(float)nTileWidth * 1.25f, +(float)nTileHeight / (fFaceDir > 0.0f ? 1.0f : 2.0f) });
				sVacuum.o.push_back({ +(float)nTileWidth * 1.25f, -(float)nTileHeight / (fFaceDir > 0.0f ? 1.0f : 2.0f) });
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

					// if one ennemy is under fSwallowDistance from kirbo, every swallowable ennemy is killed and kirbo starts swallowing animation
					float fTargetX = fPlayerPosX - object->px;
					float fTargetY = fPlayerPosY - object->py;
					float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

					if (fDistance <= fSwallowDistance)
					{
						bSwallowing = true;
						bAttacking = true;
					}
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
		if (bIsPlayerAttackable && !bSwallowing && !object->bVacuumed)
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
				if (bIsPlayerAttackable)
				{
					CheckIfPlayerIsDamaged(object, atan2f(object->vy, object->vx), fOffsetX, fOffsetY);
				}
			}
		}
	}

	for (auto& object : vecEnnemies)
	{
		object->Update(fElapsedTime, fPlayerPosX, fPlayerPosY, this);
	}

	for (auto& object : vecProjectiles)
	{
		object->Update(fElapsedTime, fPlayerPosX, fPlayerPosY, this);
	}

	// Remove dead ennemies
	vecEnnemies.erase(remove_if(vecEnnemies.begin(), vecEnnemies.end(), [](const cDynamicCreature* d)
	{
		return ((cDynamicCreature*)d)->bDead;
	}), vecEnnemies.end());

	// Remove swallowed ennemies
	if (bSwallowing)
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

	// Check invulnerability frame
	fInvulnerabilityTimer -= fElapsedTime;
	if (fInvulnerabilityTimer <= 0.0f)
	{
		fInvulnerabilityTimer = 0.0f;
		bShowKirbo = true;
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
				bShowKirbo = !bShowKirbo;
		}
	}

	t.Translate((fPlayerPosX - fOffsetX) * nTileWidth + (nTileWidth / 2), (fPlayerPosY - fOffsetY) * nTileHeight + (nTileHeight / 2));

	if (bShowKirbo)
	{
		SetPixelMode(olc::Pixel::ALPHA);
		animPlayer.DrawSelf(this, t);
		SetPixelMode(olc::Pixel::NORMAL);
	}

#pragma region HUD

	HUD->HealthBar(this, sprHealthBar);
	HUD->HealthPoints(this, sprHealthPoint, fHealth);

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

	if (currentLvl->LoadLevel(bossLevels[nCurrentLevel]))
	{
		LoadLevelProperties();

		currentLvl->PopulateBoss(vecEnnemies, nCurrentLevel);

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
	nLevelWidth = currentLvl->GetWidth();
	nLevelHeight = currentLvl->GetHeight();
	fPlayerPosX = currentLvl->GetInitPlayerPosX();
	fPlayerPosY = currentLvl->GetInitPlayerPoxY();
	sLevel = currentLvl->GetLevel();
}

void OneLoneCoder_Platformer::StopAnyAttack()
{
	bAttacking = false;
	bSlapping = false;
	bLaunchingJesusCross = false;
	bSwallowing = false;
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
		(fPlayerPosX + 0.5f - fOffsetX) * (float)nTileWidth,
		(fPlayerPosY + 0.5f - fOffsetY) * (float)nTileHeight
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
	{
		// Player is damaged
		animPlayer.ChangeState("damaged");
		fInvulnerabilityTimer = cfInvulnerabilityFrame;
		bPlayerDamaged = true;
		bIsPlayerAttackable = false;
		fHealth -= object->nDamage;

		if (fHealth <= 0.0f)
		{
			bDead = true;
			animPlayer.ChangeState("dead");
		}

		if (!bDead)
		{
			// Knockback the player out of the ennemy
			if (object->px < fPlayerPosX)
			{
				fPlayerVelX = cfDamageEjectionVelX;
				fPlayerVelY = -cfDamageEjectionVelY;
			}
			else
			{
				fPlayerVelX = -cfDamageEjectionVelX;
				fPlayerVelY = -cfDamageEjectionVelY;
			}
		}
	}
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
		float tx = victim->px - fPlayerPosX;
		float ty = victim->py - fPlayerPosY;
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
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
		return sLevel[y * nLevelWidth + x];
	else
		return L' ';
}

void OneLoneCoder_Platformer::SetTile(int x, int y, wchar_t c)
{
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
		sLevel[y * nLevelWidth + x] = c;
}

void OneLoneCoder_Platformer::ResetVariables()
{
	fPlayerVelX = 0.0f;
	fPlayerVelY = 0.0f;
	fInvulnerabilityTimer = 0.0f;
	fStopTimebeforeDeadAnim = 0.0f;
	bDead = false;
	fDeadAnimation = 0.0f;
	bPlayerDamaged = false;
	bBossKilled = false;
	bShake = false;
	fWinTimer = 0.0f;
	fKirboGoesAwayTimer = 0.0f;
	fWaitBeforeWinAnimation = 0.0f;
	StopAnyAttack();
}

bool OneLoneCoder_Platformer::CanInteract()
{
	return !bPlayerDamaged && !bDead && !bBossKilled;
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

	// Check 47 the configurations
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
