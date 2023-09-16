#include "Platformer_Animator.h"

void cAnimator::ChangeState(const std::string& s, olc::Sprite& playerSprite, olc::Decal& playerDecal)
{
	if (s != sCurrentState)
	{
		sCurrentState = s;
		fTimeCounter = 0;
		nCurrentFrame = 0;
		playerSprite.LoadFromFile(mapStates[sCurrentState][nCurrentFrame]);
		playerDecal.Update();
	}
}

void cAnimator::Update(const float& fElapsedTime, olc::Sprite& playerSprite, olc::Decal& playerDecal)
{
	fTimeCounter += fElapsedTime;
	if (fTimeCounter >= fTimeBetweenFrames)
	{
		fTimeCounter -= fTimeBetweenFrames;
		nCurrentFrame++;
		if (nCurrentFrame >= mapStates[sCurrentState].size())
			nCurrentFrame = 0;

		playerSprite.LoadFromFile(mapStates[sCurrentState][nCurrentFrame]);
		playerDecal.Update();
	}
}

void cAnimator::DrawSelf(const float& posx, const float& posy, const float& angle, const float& faceDir, olc::PixelGameEngine& gfx, olc::Decal& playerDecal)
{
	olc::vf2d pos; pos.x = posx; pos.y = posy;
    gfx.DrawRotatedDecal(pos, &playerDecal, angle, { playerDecal.sprite->width / 2.0f, playerDecal.sprite->height / 2.0f }, { faceDir, 1.0f });
}

void cAnimator::LoadAnimations()
{
	mapStates["idle"].push_back("assets/gfx/kirbo00.png");
	mapStates["idle"].push_back("assets/gfx/kirbo00.png");
	mapStates["idle"].push_back("assets/gfx/kirbo01.png");
	mapStates["idle"].push_back("assets/gfx/kirbo01.png");
	mapStates["idle"].push_back("assets/gfx/kirbo02.png");
	mapStates["idle"].push_back("assets/gfx/kirbo02.png");
	mapStates["idle"].push_back("assets/gfx/kirbo03.png");
	mapStates["idle"].push_back("assets/gfx/kirbo03.png");
	mapStates["idle"].push_back("assets/gfx/kirbo04.png");
	mapStates["idle"].push_back("assets/gfx/kirbo04.png");
	mapStates["idle"].push_back("assets/gfx/kirbo05.png");
	mapStates["idle"].push_back("assets/gfx/kirbo05.png");
	mapStates["idle"].push_back("assets/gfx/kirbo04.png");
	mapStates["idle"].push_back("assets/gfx/kirbo04.png");
	mapStates["idle"].push_back("assets/gfx/kirbo03.png");
	mapStates["idle"].push_back("assets/gfx/kirbo03.png");
	mapStates["idle"].push_back("assets/gfx/kirbo02.png");
	mapStates["idle"].push_back("assets/gfx/kirbo02.png");
	mapStates["idle"].push_back("assets/gfx/kirbo01.png");
	mapStates["idle"].push_back("assets/gfx/kirbo01.png");

	mapStates["riding_star"].push_back("assets/gfx/riding_star00.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star00.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star01.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star01.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star02.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star02.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star03.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star03.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star04.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star04.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star05.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star05.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star06.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star06.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star07.png");
	mapStates["riding_star"].push_back("assets/gfx/riding_star07.png");

	mapStates["run"].push_back("assets/gfx/kirboRunning00.png");
	mapStates["run"].push_back("assets/gfx/kirboRunning00.png");
	mapStates["run"].push_back("assets/gfx/kirboRunning01.png");
	mapStates["run"].push_back("assets/gfx/kirboRunning01.png");
	mapStates["run"].push_back("assets/gfx/kirboRunning02.png");
	mapStates["run"].push_back("assets/gfx/kirboRunning02.png");
	mapStates["run"].push_back("assets/gfx/kirboRunning03.png");
	mapStates["run"].push_back("assets/gfx/kirboRunning03.png");

	mapStates["slap"].push_back("assets/gfx/slap00.png");
	mapStates["slap"].push_back("assets/gfx/slap00.png");
	mapStates["slap"].push_back("assets/gfx/slap01.png");
	mapStates["slap"].push_back("assets/gfx/slap02.png");
	mapStates["slap"].push_back("assets/gfx/slap02.png");
	mapStates["slap"].push_back("assets/gfx/slap02.png");
	mapStates["slap"].push_back("assets/gfx/slap02.png");

	mapStates["jump"].push_back("assets/gfx/kirboJump.png");

	mapStates["fall"].push_back("assets/gfx/kirboFall.png");

	mapStates["damaged"].push_back("assets/gfx/kirboDamaged00.png");
	mapStates["damaged"].push_back("assets/gfx/kirboDamaged01.png");
	mapStates["damaged"].push_back("assets/gfx/kirboDamaged02.png");
	mapStates["damaged"].push_back("assets/gfx/kirboDamaged03.png");
	mapStates["damaged"].push_back("assets/gfx/kirboDamaged04.png");
	mapStates["damaged"].push_back("assets/gfx/kirboDamaged05.png");
	mapStates["damaged"].push_back("assets/gfx/kirboDamaged06.png");
	mapStates["damaged"].push_back("assets/gfx/kirboDamaged07.png");

	mapStates["dead"].push_back("assets/gfx/kirboDead.png");

	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying01.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying02.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying01.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");
	mapStates["flying"].push_back("assets/gfx/kirboFlying00.png");

	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross00.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross00.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross01.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross01.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross00.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross00.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross01.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross01.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross02.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross02.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross03.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross03.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross04.png");
	mapStates["jesus_christ"].push_back("assets/gfx/kirbolaunchingcross04.png");

	mapStates["begin_vacuum"].push_back("assets/gfx/kirboBeginVacuum00.png");
	mapStates["begin_vacuum"].push_back("assets/gfx/kirboBeginVacuum00.png");
	mapStates["begin_vacuum"].push_back("assets/gfx/kirboBeginVacuum01.png");
	mapStates["begin_vacuum"].push_back("assets/gfx/kirboBeginVacuum01.png");

	mapStates["vacuum"].push_back("assets/gfx/kirboVacuum00.png");
	mapStates["vacuum"].push_back("assets/gfx/kirboVacuum01.png");
	mapStates["vacuum"].push_back("assets/gfx/kirboVacuum00.png");
	mapStates["vacuum"].push_back("assets/gfx/kirboVacuum01.png");

	mapStates["swallow"].push_back("assets/gfx/kirboSwallow00.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow00.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow01.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow01.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow02.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow02.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow03.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow03.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");
	mapStates["swallow"].push_back("assets/gfx/kirboSwallow04.png");

	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed00.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed01.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed01.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed02.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed02.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed03.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed03.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed04.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed04.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed05.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed05.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed06.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed06.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed07.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed07.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed08.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed08.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed09.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed09.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed10.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed10.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed11.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed11.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed12.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed12.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed13.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed13.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed14.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed14.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed15.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed15.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed16.png");
	mapStates["boss_killed"].push_back("assets/gfx/boss_killed16.png");

	mapStates["kirbo_goes_away"].push_back("assets/gfx/boss_killed16.png");

	mapStates["poyo"].push_back("assets/gfx/poyo00.png");
	mapStates["poyo"].push_back("assets/gfx/poyo01.png");
	mapStates["poyo"].push_back("assets/gfx/poyo01.png");
	mapStates["poyo"].push_back("assets/gfx/poyo01.png");
	mapStates["poyo"].push_back("assets/gfx/poyo01.png");
	mapStates["poyo"].push_back("assets/gfx/poyo02.png");
	mapStates["poyo"].push_back("assets/gfx/poyo02.png");
	mapStates["poyo"].push_back("assets/gfx/poyo03.png");
	mapStates["poyo"].push_back("assets/gfx/poyo03.png");

	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor00.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor00.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor01.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor01.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor02.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor02.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor03.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor03.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor04.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor04.png");
	mapStates["enterDoor"].push_back("assets/gfx/kirboEnterDoor04.png");

	mapStates["joy"].push_back("assets/gfx/kirboJoy00.png");
	mapStates["joy"].push_back("assets/gfx/kirboJoy01.png");

	mapStates["grabCamera"].push_back("assets/gfx/kirboCamera00.png");
	mapStates["grabCamera"].push_back("assets/gfx/kirboCamera00.png");
	mapStates["grabCamera"].push_back("assets/gfx/kirboCamera01.png");
	mapStates["grabCamera"].push_back("assets/gfx/kirboCamera01.png");
	mapStates["grabCamera"].push_back("assets/gfx/kirboCamera02.png");
	mapStates["grabCamera"].push_back("assets/gfx/kirboCamera02.png");

	mapStates["holdCamera"].push_back("assets/gfx/kirboCamera03.png");
}
