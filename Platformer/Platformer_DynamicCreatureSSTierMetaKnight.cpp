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
	nDamage = 2;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureSSTierMetaKnight::~cDynamicCreatureSSTierMetaKnight()
{
	delete hitbox;
	delete dynSprite;
	delete level;
}

void cDynamicCreatureSSTierMetaKnight::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	switch (nAIState)
	{
		case AI_IDLE:
		{
			engine->PlaySample("meleeControllerRape", false, true);

			vx = 0.0f; vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			bCanSpawnAOE = true;
			bUseTPAttack = false;

			if (fBehaviourTimer >= fWaitingTime)
			{
				// Select attack
				if (abs(px - playerX) >= fKirboIsFarPoint)
				{
					// If kirbo is far, he only uses dashes and TP
					int chosenAttack = rand() % 2;

					if (chosenAttack == 0)
					{
						DashToKirbo(playerX);
					}
					else
					{
						TeleportToKirbo();
					}
				}
				else
				{
					int chosenAttack = rand() % 4;

					if (chosenAttack == 0)
					{
						ChangeState(AI_TORNADO);
					}
					else if (chosenAttack == 1)
					{
						ChangeState(AI_DOWNTILT);
					}
					else if (chosenAttack == 2)
					{
						DashToKirbo(playerX);
					}
					else
					{
						TeleportToKirbo();
					}
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
			nGraphicState = MOVE1; // UPATTACK Row contains most of the animations because they are short

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
					engine->PlaySample("hiyayayaya");
					engine->PlaySample("multipleSwordStrike");

					float aoeX = 0.0f, aoeY = py + cfMKWidth - fAOEHeight;
					if (nFaceDir == 0)
						aoeX = px - fAOEWidth;
					else
						aoeX = px + cfMKWidth;

					engine->ActivateShakeEffect(true, 50, 50);
					engine->AddProjectile(aoeX, aoeY, false, 0.0f, 0.0f, fAttackTime, "SSTierMKHiyayaAOE", false, cnSSTierMKDmg, false);
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
				nGraphicState = MOVE1;

				LoopAnimation(nFirstFrameUpAttack, nLastFrameUpAttack);
				vx = 0.0f; vy = 0.0f;

				if (bPlaySound)
				{
					engine->PlaySample("hiyayayaya");
					bPlaySound = false;
				}

				fSwordAttackTimer += fElapsedTime;

				if (fSwordAttackTimer >= fTimeBetweenUpStrikes)
				{
					engine->PlaySample("singleSwordStrike");

					fSwordAttackTimer = 0.0f;

					engine->AddProjectile(px - 1.5f, py - 1.0f, false, 0.0f, 0.0f, cfOneSSTierMKFrame, "swordAttack", false, cnSSTierMKDmg, false);
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
				engine->PlaySample("loudScreaming");

			fBehaviourTimer += fElapsedTime;
			nGraphicState = MOVE2;

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
					engine->PlaySample("tornado");

					engine->ActivateShakeEffect(true, 50, 50);
					engine->AddProjectile(px - cfTornadoOffsetX, py - cfTornadoOffsetY, false, 0.0f, 0.0f, fAttackTime, "tornado", false, cnSSTierMKDmg, false);
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
				nGraphicState = MOVE2;
				SingleAnimation(nFirstFrameTeleport, nLastFrameTeleport);

				if (fBehaviourTimer >= fWaitBeforeAttack + ((float)nNbFramesBeforeTPAtk * (1.0f / (float)nFramesPerSecond)) && bCanSpawnAOE)
				{
					if (px <= playerX)
						nFaceDir = 1;
					else
						nFaceDir = 0;

					bCanSpawnAOE = false;
					engine->PlaySample("singleSwordStrike");
					engine->PlaySample("ha");

					float fOffsetX = 0.0f;
					if (nFaceDir == 0)
						fOffsetX = -2.0f;

					engine->AddProjectile(px + fOffsetX * cfMKWidth, py - 1.0f * cfMKWidth, false, 0.0f, 0.0f, fAOEDuration, "SSTierMKTPAttack", false, cnSSTierMKLowDmg, false);
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
				nGraphicState = MOVE1;

				LoopAnimation(nFirstFrameDownAttack, nLastFrameDownAttack);
				vx = 0.0f; vy = 0.0f;

				fSwordAttackTimer += fElapsedTime;

				if (fSwordAttackTimer >= fTimeBetweenDownStrikes)
				{
					engine->PlaySample("singleSwordStrike");
					engine->PlaySample("ha");

					fSwordAttackTimer = 0.0f;

					// slightly move the aoe to the left to turn the sprite to the down
					engine->AddProjectile(px - 1.5f, py + 1.0f, false, -0.05f, 0.0f, cfOneSSTierMKFrame, "swordAttack", false, cnSSTierMKDmg, false);
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
			nGraphicState = MOVE3;

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
					engine->PlaySample("singleSwordStrike");
					engine->PlaySample("ha");

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

					engine->AddProjectile(px + fOffsetX, py + 0.5f, false, fVX, 0.0f, cfOneSSTierMKFrame, "downTilt", false, cnSSTierMKDmg, false);
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

	if (nHealth <= 0 && engine->IsSamplePlaying("meleeControllerRape"))
		engine->StopSample("meleeControllerRape");
}

void cDynamicCreatureSSTierMetaKnight::TeleportToKirbo()
{
	bUseTPAttack = true;
	engine->PlaySample("behold");
	ChangeState(AI_DISAPPEARING);
}

void cDynamicCreatureSSTierMetaKnight::DashToKirbo(float playerX)
{
	if (px < playerX - fStayAwayDistance || px > playerX + fStayAwayDistance)
		engine->PlaySample("dash");

	ChangeState(AI_DASHING);
}

void cDynamicCreatureSSTierMetaKnight::ChangeState(AI_STATE state, bool resetBehaviourTimer)
{
	nGraphicCounter = 0;
	if (resetBehaviourTimer)
		fBehaviourTimer = 0.0f;
	nAINextState = state;

	engine->StopSample("meleeControllerRape");
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
	if (nGraphicCounter > lastFrame)  nGraphicCounter = lastFrame;
}
