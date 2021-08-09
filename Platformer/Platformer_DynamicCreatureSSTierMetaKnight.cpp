#include "Platformer_DynamicCreatureSSTierMetaKnight.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureSSTierMetaKnight::engine = nullptr;

cDynamicCreatureSSTierMetaKnight::cDynamicCreatureSSTierMetaKnight(cLevel* l) : cDynamicCreature("SSTierMetaKnight", cAssets::get().GetSprite("SSTierMetaKnight"), 32)
{
	fDynWidth = 64.0f;
	fDynHeight = 64.0f;
	fSpriteW = 320.0f;
	fSpriteH = 128.0f;
	fSpriteOffsetX = -128.0f;
	fSpriteOffsetY = -64.0f;
	bFriendly = false;
	nHealth = 100;
	nHealthMax = 100;
	level = l;
	bSolidVsMap = true;
	bAffectedByGravity = true;
	nDamage = 3;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureSSTierMetaKnight::~cDynamicCreatureSSTierMetaKnight()
{
	delete hitbox;
	delete sSprite;
	delete level;
}

void cDynamicCreatureSSTierMetaKnight::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	switch (nAIState)
	{
		case AI_IDLE:
		{
			if (!olc::SOUND::IsSamplePlaying(engine->GetSound("meleeControllerRape")))
				olc::SOUND::PlaySample(engine->GetSound("meleeControllerRape"));

			vx = 0.0f; vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			bCanSpawnAOE = true;
			bUseTPAttack = false;

			if (fBehaviourTimer >= fWaitingTime)
			{
				// Select attack
				if (abs(px - playerX) >= fKirboIsFarPoint)
				{
					int chosenAttack = rand() % 2;

					if (chosenAttack == 0)
					{
						if (px < playerX - fStayAwayDistance || px > playerX + fStayAwayDistance)
							olc::SOUND::PlaySample(engine->GetSound("dash"));

						ChangeState(AI_DASHING);
					}
					else
					{
						bUseTPAttack = true;
						olc::SOUND::PlaySample(engine->GetSound("behold"));
						ChangeState(AI_DISAPPEARING);
					}
				}
				else
				{
					// if kirbo is near, MK use tornado or down tilt
					int chosenAttack = rand() % 2;

					if (chosenAttack == 0)
						ChangeState(AI_TORNADO);
					else
						ChangeState(AI_DOWNTILT);
				}
			}
		}
		break;
		case AI_DASHING:
		{
			if (px < playerX - fStayAwayDistance)
			{
				vx = 50.0f;
				vy = 0.0f;
			}
			else if (px > playerX + fStayAwayDistance)
			{
				vx = -50.0f;
				vy = 0.0f;
			}
			else
			{
				if (abs(py - playerY) <= fKirboOnSameLine)
					ChangeState(AI_HIYAYAYAYA);
				else
					ChangeState(AI_DISAPPEARING);
			}
		}
		break;
		case AI_HIYAYAYAYA:
		{
			fBehaviourTimer += fElapsedTime;
			nGraphicState = UPATTACK; // UPATTACK Row contains most of the animations because they are short

			if (fBehaviourTimer <= fWaitBeforeAttack)
			{
				vx = 0.0f; vy = 0.0f;
				nGraphicCounter = 0;
			}
			else if (fBehaviourTimer <= fWaitBeforeAttack + fAttackTime)
			{
				LoopAnimation(nFirstFrameHiyayayaya, nLastFrameHiyayayaya);
				vx = 0.0f; vy = 0.0f;

				if (bCanSpawnAOE)
				{
					olc::SOUND::PlaySample(engine->GetSound("hiyayayaya"));
					olc::SOUND::PlaySample(engine->GetSound("multipleSwordStrike"));

					float aoeX = 0.0f, aoeY = py + cfMKWidth - fAOEHeight;
					if (nFaceDir == 0)
						aoeX = px - fAOEWidth;
					else
						aoeX = px + cfMKWidth;

					engine->ActivateShakeEffect(true, 50, 50);
					engine->AddProjectile(engine->CreateProjectile(aoeX, aoeY, false, 0.0f, 0.0f, fAttackTime, "SSTierMKHiyayaAOE", false, cnSSTierMKDmg, false));
					bCanSpawnAOE = false;
				}
			}
			else
			{
				engine->ActivateShakeEffect(false);
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_DISAPPEARING:
		{
			vx = 0.0f; vy = 0.0f;

			fBehaviourTimer += fElapsedTime;
			nGraphicState = JUMPING; // JUMP Row contains teleport animations

			if (fBehaviourTimer <= fDisapearingTime)
			{
				SingleAnimation(nFirstFrameDisappear, nLastFrameDisappear);

				// Teleport Meta Knight in the air when disappear before reappearing near kirbo
				if (nGraphicCounter == nLastFrameDisappear)
					py = -10.0f;
			}
			else
			{
				if (!bUseTPAttack)
				{
					// Randomly choose to attack from above or below if kirbo is not on ground
					int chosenAttack = rand() % 2;

					if (chosenAttack == 0)
						ChangeState(AI_UPATTACK);
					else
						ChangeState(AI_DOWNAERIAL);

					// If Kirbo is too near from the ground, MK appear above, if he's too high in the air, MK appears below
					if (playerY >= fLowerLimitToAppear)
						ChangeState(AI_DOWNAERIAL);
					if (playerY <= fHigherLimitToAppear)
						ChangeState(AI_UPATTACK);
				}
				else
				{
					fKirboPosX = playerX;
					fKirboPosY = playerY;
					ChangeState(AI_TELEPORT);
				}
			}
		}
		break;
		case AI_UPATTACK:
		{
			if (fBehaviourTimer == 0.0f)
			{
				px = playerX;
				py = playerY + fReappearingDistance;
			}

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fWaitBeforeAttack)
			{
				vx = 0.0f; vy = 0.0f;

				nGraphicState = JUMPING;
				SingleAnimation(nFirstFrameReappear, nLastFrameReappear);
			}
			else if (fBehaviourTimer <= fWaitBeforeAttack + fAttackTime)
			{
				nGraphicState = UPATTACK;

				LoopAnimation(nFirstFrameUpAttack, nLastFrameUpAttack);
				vx = 0.0f; vy = 0.0f;

				if (bPlaySound)
				{
					olc::SOUND::PlaySample(engine->GetSound("hiyayayaya"));
					bPlaySound = false;
				}

				fSwordAttackTimer += fElapsedTime;

				if (fSwordAttackTimer >= fTimeBetweenUpStrikes)
				{
					olc::SOUND::PlaySample(engine->GetSound("singleSwordStrike"));

					fSwordAttackTimer = 0.0f;

					engine->AddProjectile(engine->CreateProjectile(px - 1.5f, py - 1.0f, false, 0.0f, 0.0f, cfOneSSTierMKFrame, "swordAttack", false, cnSSTierMKDmg, false));
				}
			}
			else
			{
				fSwordAttackTimer = cfOneSSTierMKFrame;
				bPlaySound = true;
				py = fGroundY;
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_TORNADO:
		{
			vx = 0.0f; vy = 0.0f;

			if (fBehaviourTimer == 0.0f)
				olc::SOUND::PlaySample(engine->GetSound("loudScreaming"));

			fBehaviourTimer += fElapsedTime;
			nGraphicState = SIDEATTACK;

			if (fBehaviourTimer <= fWaitBeforeAttack)
			{
				LoopAnimation(nFirstFramePreTornado, nLastFramePreTornado);
			}
			else if (fBehaviourTimer <= fWaitBeforeAttack + fAttackTime)
			{
				SingleAnimation(nFirstFrameTornado, nLastFrameTornado);

				if (fBehaviourTimer >= fWaitBeforeAttack + ((float)nNbFramesBeforeTornado * (1.0f / (float)nFramesPerSecond)) && bCanSpawnAOE)
				{
					bCanSpawnAOE = false;
					olc::SOUND::PlaySample(engine->GetSound("tornado"));

					engine->ActivateShakeEffect(true, 50, 50);
					engine->AddProjectile(engine->CreateProjectile(px - cfMKWidth, py - 2.0f * cfMKWidth, false, 0.0f, 0.0f, fAttackTime, "tornado", false, cnSSTierMKDmg, false));
				}
			}
			else
			{
				engine->ActivateShakeEffect(false);
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_TELEPORT:
		{
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fWaitBeforeAttack)
			{
				// stay invisible a little time
				nGraphicCounter = nFirstFrameReappear;
			}
			else if (fBehaviourTimer <= fWaitBeforeAttack + fAppearingTime)
			{
				px = fKirboPosX;
				py = fKirboPosY;

				// Reappears and attack at the 4th frame
				nGraphicState = SIDEATTACK;
				SingleAnimation(nFirstFrameTeleport, nLastFrameTeleport);

				if (fBehaviourTimer >= fWaitBeforeAttack + ((float)nNbFramesBeforeTPAtk * (1.0f / (float)nFramesPerSecond)) && bCanSpawnAOE)
				{
					if (px <= playerX)
						nFaceDir = 1;
					else
						nFaceDir = 0;

					bCanSpawnAOE = false;
					olc::SOUND::PlaySample(engine->GetSound("singleSwordStrike"));
					olc::SOUND::PlaySample(engine->GetSound("ha"));

					float fOffsetX = 0.0f;
					if (nFaceDir == 0)
						fOffsetX = -2.0f;

					engine->AddProjectile(engine->CreateProjectile(px + fOffsetX * cfMKWidth, py - 1.0f * cfMKWidth, false, 0.0f, 0.0f, fAOEDuration, "SSTierMKTPAttack", false, cnSSTierMKDmg, false));
				}
			}
			else
			{
				py = fGroundY;
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_DOWNAERIAL:
		{
			if (fBehaviourTimer == 0.0f)
			{
				px = playerX;
				py = playerY - fReappearingDistance;
			}

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fWaitBeforeAttack)
			{
				vx = 0.0f; vy = 0.0f;

				nGraphicState = JUMPING;
				SingleAnimation(nFirstFrameReappear, nLastFrameReappear);
			}
			else if (fBehaviourTimer <= fWaitBeforeAttack + fAttackTime)
			{
				nGraphicState = UPATTACK;

				LoopAnimation(nFirstFrameDownAttack, nLastFrameDownAttack);
				vx = 0.0f; vy = 0.0f;

				fSwordAttackTimer += fElapsedTime;

				if (fSwordAttackTimer >= fTimeBetweenDownStrikes)
				{
					olc::SOUND::PlaySample(engine->GetSound("singleSwordStrike"));
					olc::SOUND::PlaySample(engine->GetSound("ha"));

					fSwordAttackTimer = 0.0f;

					// slightly move the aoe to the left to turn the sprite to the down
					engine->AddProjectile(engine->CreateProjectile(px - 1.5f, py + 1.0f, false, -0.05f, 0.0f, cfOneSSTierMKFrame, "swordAttack", false, cnSSTierMKDmg, false));
				}
			}
			else
			{
				fSwordAttackTimer = cfOneSSTierMKFrame;
				py = fGroundY;
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_DOWNTILT:
		{
			nGraphicState = DOWNATTACK;

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fWaitBeforeAttack)
			{
				vx = 0.0f; vy = 0.0f;
				nGraphicCounter = 0;
			}
			else if (fBehaviourTimer <= fWaitBeforeAttack + fAttackTime)
			{
				LoopAnimation(nFirstFrameDownTilt, nLastFrameDownTilt);
				vx = 0.0f; vy = 0.0f;

				fSwordAttackTimer += fElapsedTime;

				if (fSwordAttackTimer >= fTimeBetweenDownStrikes)
				{
					olc::SOUND::PlaySample(engine->GetSound("singleSwordStrike"));
					olc::SOUND::PlaySample(engine->GetSound("ha"));

					fSwordAttackTimer = 0.0f;

					if (px <= playerX)
						nFaceDir = 1;
					else
						nFaceDir = 0;

					float fOffsetX = 1.0f;
					float fVX = 1.0f;
					if (nFaceDir == 0)
					{
						fOffsetX = -2.0f;
						fVX = -1.0f;
					}

					engine->AddProjectile(engine->CreateProjectile(px + fOffsetX, py + 0.5f, false, fVX, 0.0f, cfOneSSTierMKFrame, "downTilt", false, cnSSTierMKDmg, false));
				}
			}
			else
			{
				fSwordAttackTimer = cfOneSSTierMKFrame;
				py = fGroundY;
				ChangeState(AI_IDLE);
			}
		}
		break;
	}

	UpdateTimers();

	nAIState = nAINextState;

	if (nHealth <= 0 && olc::SOUND::IsSamplePlaying(engine->GetSound("meleeControllerRape")))
		olc::SOUND::StopSample(engine->GetSound("meleeControllerRape"));
}

void cDynamicCreatureSSTierMetaKnight::ChangeState(AI_STATE state, bool resetBehaviourTimer)
{
	nGraphicCounter = 0;
	if (resetBehaviourTimer)
		fBehaviourTimer = 0.0f;
	nAINextState = state;

	if (olc::SOUND::IsSamplePlaying(engine->GetSound("meleeControllerRape")))
		olc::SOUND::StopSample(engine->GetSound("meleeControllerRape"));
}

void cDynamicCreatureSSTierMetaKnight::UpdateTimers()
{
	if (nHealth >= cnHiHP)
	{
		fWaitingTime = cfWaitingTimeMaxHp;
		fWaitBeforeAttack = cfWaitBeforeAttackMaxHp;
	}
	else if (nHealth < cnHiHP && nHealth >= cnMiHP)
	{
		fWaitingTime = cfWaitingTimeHiHp;
		fWaitBeforeAttack = cfWaitBeforeAttackHiHp;
	}
	else if (nHealth < cnMiHP && nHealth >= cnLoHP)
	{
		fWaitingTime = cfWaitingTimeMiHp;
		fWaitBeforeAttack = cfWaitBeforeAttackMiHp;
	}
	else
	{
		fWaitingTime = cfWaitingTimeLoHp;
		fWaitBeforeAttack = cfWaitBeforeAttackLoHp;
	}
}

void cDynamicCreatureSSTierMetaKnight::LoopAnimation(int firstFrame, int lastFrame)
{
	if (nGraphicCounter < firstFrame || nGraphicCounter > lastFrame)
		nGraphicCounter = firstFrame;
}

void cDynamicCreatureSSTierMetaKnight::SingleAnimation(int firstFrame, int lastFrame)
{
	if (nGraphicCounter < firstFrame) nGraphicCounter = firstFrame;
	if (nGraphicCounter > lastFrame) nGraphicCounter = lastFrame;
}