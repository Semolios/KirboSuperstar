#include "Platformer_DynamicCreatureKingDDD.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureKingDDD::engine = nullptr;

cDynamicCreatureKingDDD::cDynamicCreatureKingDDD(cLevel* l) : cDynamicCreature("kingDDD", cAssets::get().GetSprite("kingDDD"), 30)
{
	fDynWidth = 128.0f;
	fDynHeight = 128.0f;
	fSpriteW = 384.0f;
	fSpriteH = 384.0f;
	fSpriteOffsetX = -128.0f;
	fSpriteOffsetY = -128.0f;
	bFriendly = false;
	nHealth = 100;
	nHealthMax = 100;
	level = l;
	bSolidVsMap = true;
	bAffectedByGravity = true;
	nDamage = 4;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureKingDDD::~cDynamicCreatureKingDDD()
{
	delete hitbox;
	delete sSprite;
	delete level;
}

void cDynamicCreatureKingDDD::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	switch (nAIState)
	{
		case AI_IDLE:
		{
			vx = 0.0f; vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer >= fWaitingTime)
			{
				ChangeState(AI_MOVING);
			}
		}
		break;
		case AI_MOVING:
		{
			// Run in kirbo's direction
			vx = (playerX < px) ? -cfRunSpeed : cfRunSpeed;

			LoopAnimation(nFirstFrameRunning, nLastFrameRunning);

			// if kirbo is near on x axis but too high from king DDD, he jumps
			if (abs(px + fKingDDDSemiWidth - playerX + 0.5f) <= fKirboNearDistance && py - playerY >= fKirboTooHighDistance)
			{
				// One time out of 4, king DDD choose to UP B instead of jumping
				int chosenJump = rand() % 4;

				if (chosenJump == 0)
					ChangeState(AI_UPB);
				else
					ChangeState(AI_JUMPING);
			}

			// Each frame, a random number is generated. If this number % 2 / fElapsedTime is equal to 0 then the event occurs
			int useRangedAttack = rand() % (int)(2 / fElapsedTime);
			if (useRangedAttack == 0)
			{
				// if king DDD decide to use a ranged attack, he can throw a spike ball or charge a Down B Attack and walk toward kirbo
				int chosenRangedAttack = rand() % 2;

				if (chosenRangedAttack == 0)
					ChangeState(AI_SIDEB);
				else
					ChangeState(AI_DOWNB);
			}

			// if kirbo is close enough from the attack, king DDD uses UP SMASH
			if (KirboIsInUpArea(playerX, playerY))
			{
				ChangeState(AI_UPSMASH);
			}
			else if (KirboIsInLeftArea(playerX, playerY) || KirboIsInRightArea(playerX, playerY))
			{
				// King DDD can either choose to Side SMASH, Down SMASH, Vacuum or Jump.
				int chosenAttack = rand() % 4;

				if (chosenAttack == 0)
				{
					nSmashSide = nFaceDir;
					ChangeState(AI_SIDESMASH);
				}
				else if (chosenAttack == 1)
				{
					ChangeState(AI_DOWNSMASH);
				}
				else if (chosenAttack == 2)
				{
					nVacuumState = PREPARE;
					nVacuumNextState = PREPARE;
					ChangeState(AI_VACUUM);
				}
				else
				{
					ChangeState(AI_JUMPING);
				}
			}

			// when King DDD is walking in a hole, he falls
			if (!OnGround())
			{
				bHasNotAlreadyJumped = false;
				ChangeState(AI_JUMPING);
			}

			// If kirbo is Below, king DDD cross the semi-solid platforms
			if (OnSemiSolidPlatform() && playerY >= py + fKingDDDHeight)
			{
				CrossPlatform();
			}
		}
		break;
		case AI_JUMPING:
		{
			if (vy <= 0)
				nGraphicCounter = nJumpingFrame;
			else
				nGraphicCounter = nFallingFrame;

			nGraphicState = JUMPING;

			if (bHasNotAlreadyJumped)
			{
				engine->PlaySample("kingDDDJump");
				vy = cfJumpSpeed;
				bHasNotAlreadyJumped = false;
			}
			else
			{
				// Check kirbo position to chose the right attack
				// First Check if kirbo is above to use UP Air
				if (KirboIsInUpArea(playerX, playerY))
				{
					ChangeState(AI_UPAIR);
				}

				// Second Check if kirbo is below to use DOWN Air
				if (KirboIsInDownArea(playerX, playerY))
				{
					ChangeState(AI_DOWNAIR);
				}

				// Third Check if kirbo is ahead to use FORWARD Air
				if (KirboIsInFrontOfDDD(playerX, playerY))
				{
					nAirAttackSide = nFaceDir;
					ChangeState(AI_FORWARDAIR);
				}

				// Fourth Check if kirbo is behind to use BACKWARD Air
				if (KirboIsBehindDDD(playerX, playerY))
				{
					nAirAttackSide = nFaceDir;
					ChangeState(AI_BACKWARDAIR);
				}

				ReturnMovingStateIfOnGround(fElapsedTime);
			}
		}
		break;
		case AI_UPSMASH:
		{
			vx = 0.0f;
			nGraphicState = MOVE1;

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fSmashChargeTime)
			{
				LoopAnimation(nFirstFrameChargeUpSmash, nLastFrameChargeUpSmash);
			}
			else if (fBehaviourTimer <= fSmashChargeTime + fWaitAfterSmashTime)
			{
				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDUpSmash");
					engine->PlaySample("kingDDDUpSmashHammer");
					engine->AddProjectile(px + fUpSmashPosX, py + fUpSmashPosY, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDUpSmash", false, cnAttacksDmg, false);
				}
				SingleAnimation(nFirstFrameUpSmash, nLastFrameUpSmash);
			}
			else
			{
				ChangeState(AI_MOVING);
			}
		}
		break;
		case AI_SIDESMASH:
		{
			vx = 0.0f;
			nGraphicState = MOVE2;

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fSmashChargeTime)
			{
				LoopAnimation(nFirstFrameChargeSideSmash, nLastFrameChargeSideSmash);
			}
			else if (fBehaviourTimer <= fSmashChargeTime + fWaitAfterSmashTime)
			{
				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDSideSmash");
					if (nSmashSide == 0)
						engine->AddProjectile(px + fLeftSideSmashPosX, py + fSideSmashPosY, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDLeftSideSmash", false, cnAttacksDmg, false);
					else
						engine->AddProjectile(px + fRightSideSmashPosX, py + fSideSmashPosY, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDRightSideSmash", false, cnAttacksDmg, false);
				}
				SingleAnimation(nFirstFrameSideSmash, nLastFrameSideSmash);
			}
			else
			{
				ChangeState(AI_MOVING);
			}
		}
		break;
		case AI_DOWNSMASH:
		{
			vx = 0.0f;
			nGraphicState = MOVE3;

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fSmashChargeTime)
			{
				LoopAnimation(nFirstFrameChargeDownSmash, nLastFrameChargeDownSmash);
			}
			else if (fBehaviourTimer <= fSmashChargeTime + fWaitAfterSmashTime)
			{
				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDDownSmashHammer");
					engine->AddProjectile(px + fDownSmashPosX, py + fDownSmashPosY, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDDownSmash", false, cnAttacksDmg, false);
				}
				SingleAnimation(nFirstFrameDownSmash, nLastFrameDownSmash);
			}
			else
			{
				ChangeState(AI_MOVING);
			}
		}
		break;
		case AI_SIDEB:
		{
			vx = 0.0f;
			nGraphicState = MOVE4;

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fSideBSearchSpikeTime)
			{
				LoopAnimation(nFirstFrameSearchSpike, nLastFrameSearchSpike);
			}
			else if (fBehaviourTimer <= fSideBSearchSpikeTime + fSideBSpikeFoundTime)
			{
				nGraphicCounter = nFrameSpikeFound;
			}
			else if (fBehaviourTimer <= fSideBSearchSpikeTime + fSideBSpikeFoundTime + fSideBSpikeThrownTime)
			{
				SingleAnimation(nFirstFrameSpikeThrown, nLastFrameSpikeThrown);

				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDSideB");
					// spawn bouncy spike
					engine->AddProjectile((nFaceDir == 0 ? fSpikeLeftPX : fSpikeRightPX) + px, py, false, nFaceDir == 0 ? fSpikeLeftVX : fSpikeRightVX, fSpikeVY, fSpikeDuration, "spike", true, cnAttacksDmg, true, true, 0, false, -3.0f, "", true, "spikeBallBounce");
				}
			}
			else
			{
				ChangeState(AI_MOVING);
			}
		}
		break;
		case AI_DOWNB:
		{
			vx = (playerX < px) ? -fDownBWalkSpeed : fDownBWalkSpeed;
			nGraphicState = MOVE5;

			fBehaviourTimer += fElapsedTime;

			if (KirboIsInLeftArea(playerX, playerY) || KirboIsInRightArea(playerX, playerY))
			{
				bCloseFromKirbo = true;
			}

			if (!bCloseFromKirbo && fBehaviourTimer <= fDownBChaseTime)
			{
				engine->PlaySample("kingDDDDownBCharge", false, true);
				LoopAnimation(nFirstFrameChargeDownB, nLastFrameChargeDownB);
			}
			else
			{
				engine->StopSample("kingDDDDownBCharge");

				vx = 0.0f;
				fBehaviourTimer2 += fElapsedTime;

				if (fBehaviourTimer2 <= fDownBPrepareTime)
				{
					engine->PlaySample("kingDDDDownBPrepare", false, true);
					LoopAnimation(nFirstFramePrepareDownB, nLastFramePrepareDownB);
				}
				else if (fBehaviourTimer2 <= fDownBPrepareTime + fDownBAttackTime)
				{
					SingleAnimation(nFirstFrameDownBAttack, nLastFrameDownBAttack);

					engine->StopSample("kingDDDDownBPrepare");

					if (bCanSpawnAOE)
					{
						bCanSpawnAOE = false;

						engine->PlaySample("kingDDDDownBAttack");
						engine->AddProjectile(px + fDownBAOEPosX, py + fDownBAOEPosY, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDDownB", false, cnAttacksDmg, false);
					}
				}
				else
				{
					bCloseFromKirbo = false;
					ChangeState(AI_MOVING);
				}
			}
		}
		break;
		case AI_UPB:
		{
			// Only one side is drawn to reduce the spritesheet
			nFaceDir = 0;
			nGraphicState = MOVE6;
			fBehaviourTimer2 += fElapsedTime;

			if (fBehaviourTimer2 <= fCrouchedDownTime)
			{
				vx = 0.0f; vy = 0.0f;

				if (playerX <= px + fUpBLeftLimit)
					nJumpSide = LEFT;
				else if (playerX >= px + fUpBRightLimit)
					nJumpSide = RIGHT;
				else
					nJumpSide = MIDDLE;
			}
			else
			{
				if (bHasNotAlreadyJumped)
				{
					vy = fUpBJumpVelY;
					bHasNotAlreadyJumped = false;
					engine->PlaySample("kingDDDUpBJump");
				}
				else
				{
					// During Up B, king DDD falls faster
					vy += engine->GetGravityValue() * fElapsedTime;

					// the jump follows kirbo
					switch (nJumpSide)
					{
						case MIDDLE: vx = 0.0f;			 break;
						case LEFT:	 vx = -fUpBJumpVelX; break;
						case RIGHT:	 vx = fUpBJumpVelX;	 break;
					}

					ReturnMovingStateIfOnGround(fElapsedTime, true, fCrouchedDownTime, MOVE6);
				}
			}

			if (vy == 0)
				nGraphicCounter = nCrouchedDownFrame;
			else
				nGraphicCounter = nUpBJumpFrame;
		}
		break;
		case AI_VACUUM:
		{
			nGraphicState = MOVE7;
			vx = 0.0f; vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			switch (nVacuumState)
			{
				case PREPARE:
				{
					nGraphicCounter = nPrepareVacuumFrame;

					if (fBehaviourTimer >= fPrepareVacuumTime)
						ChangeVacuumState(VACUUM);
				}
				break;
				case VACUUM:
				{
					engine->PlaySample("kingDDDVacuum", false, true);
					LoopAnimation(nFirstFrameVacuum, nLastFrameVacuum);

					if (KirboIsInFrontOfDDD(playerX, playerY) && engine->IsKirboAttackable())
						ChangeVacuumState(KIRBOCAUGHT);

					if (fBehaviourTimer >= fPrepareVacuumTime)
					{
						engine->StopSample("kingDDDVacuum");
						ChangeState(AI_MOVING);
					}
				}
				break;
				case KIRBOCAUGHT:
				{
					LoopAnimation(nFirstFrameVacuum, nLastFrameVacuum);

					engine->ChangeKirboVelocities((px - playerX) * fVacuumSpeed, (py - playerY) * fVacuumSpeed);
					engine->SetKirboGrabbed(true);
					engine->ChangeKirboAnimation("damaged");
					engine->SetKirboAttackable(false);

					if (engine->CheckIfKirboCollisionWithEnnemy(this))
					{
						engine->SetKirboVisible(false);
						engine->StopSample("kingDDDVacuum");
						ChangeVacuumState(SWALLOW);
					}
				}
				break;
				case SWALLOW:
				{
					nGraphicCounter = nSwallowKirboFrame;
					engine->SetKirboPositions(px, py);

					if (fBehaviourTimer <= fSwallowKirboTime)
					{
						engine->SetKirboAttackable(false);
					}
					else
					{
						engine->SetKirboVisible(true);
						engine->SetKirboAttackable(true);
						engine->SetKirboPositions(px + (nFaceDir ? 1.5f : -0.5f), py);
						engine->SetKirboGrabbed(false);
						ChangeVacuumState(SPIT);
					}
				}
				break;
				case SPIT:
				{
					nGraphicCounter = nSpitKirboFrame;

					if (fBehaviourTimer >= fSpitKirboTime)
						ChangeState(AI_MOVING);
				}
				break;
			}

			nVacuumState = nVacuumNextState;
		}
		break;
		case AI_UPAIR:
		{
			nGraphicState = MOVE8;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fPrepareUpAirAttackTime)
			{
				nGraphicCounter = nPrepareUpAirAttackFrame;
			}
			else if (fBehaviourTimer <= fPrepareUpAirAttackTime + fUpAirAttackTime)
			{
				LoopAnimation(nfirstFrameUpAirAttack, nLastFrameUpAirAttack);

				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDUpAir");
					engine->AddProjectile(px, py + fUpAirAOEPosY, false, vx, vy, fUpAirAOEDuration, "kingDDDUpAir", true, cnAttacksDmg, false);
				}
			}
			else
			{
				ChangeState(AI_JUMPING);
			}

			ReturnMovingStateIfOnGround(fElapsedTime);
		}
		break;
		case AI_FORWARDAIR:
		{
			nGraphicState = MOVE9;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fPrepareForwardAirAttackTime)
			{
				nGraphicCounter = nPrepareForwardAirAttackFrame;
			}
			else if (fBehaviourTimer <= fPrepareForwardAirAttackTime + fForwardAirAttackTime)
			{
				nGraphicCounter = nForwardAirAttackFrame;

				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDAirAtkVoice");
					engine->PlaySample("kingDDDAirAtkHammer");
					engine->AddProjectile(px + fForwardAirAOEPosX, py, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDForwardAir", false, cnAttacksDmg, false);
				}
			}
			else
			{
				ChangeState(AI_JUMPING);
			}
			ReturnMovingStateIfOnGround(fElapsedTime);
		}
		break;
		case AI_BACKWARDAIR:
		{
			nGraphicState = MOVE10;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fPrepareBackwardAirAttackTime)
			{
				nGraphicCounter = nPrepareBackwardAirAttackFrame;
			}
			else if (fBehaviourTimer <= fPrepareBackwardAirAttackTime + fBackwardAirAttackTime)
			{
				nGraphicCounter = nBackwardAirAttackFrame;

				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDAirAtkVoice");
					engine->PlaySample("kingDDDAirAtkHammer");
					engine->AddProjectile(px + fBackwardAirAOEPosX, py, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDBackwardAir", false, cnAttacksDmg, false);
				}
			}
			else
			{
				ChangeState(AI_JUMPING);
			}
			ReturnMovingStateIfOnGround(fElapsedTime);
		}
		break;
		case AI_DOWNAIR:
		{
			// Only one side is drawn to reduce the spritesheet
			nFaceDir = 0;

			nGraphicState = MOVE11;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fPrepareDownAirAttackTime)
			{
				nGraphicCounter = nPrepareDownAirAttackFrame;
			}
			else if (fBehaviourTimer <= fPrepareDownAirAttackTime + fDownAirAttackTime)
			{
				nGraphicCounter = nDownAirAttackFrame;

				if (bCanSpawnAOE)
				{
					bCanSpawnAOE = false;

					engine->PlaySample("kingDDDAirAtkVoice");
					engine->PlaySample("kingDDDAirAtkHammer");
					engine->AddProjectile(px, py + fDownAirAOEPosY, false, 0.0f, 0.0f, fAOEsDuration, "kingDDDDownAir", false, cnAttacksDmg, false);
				}
			}
			else
			{
				ChangeState(AI_JUMPING);
			}

			ReturnMovingStateIfOnGround(fElapsedTime);
		}
		break;
		case AI_RECOVERY:
		{
			// In case king DDD falls while doing down B
			engine->StopSample("kingDDDDownBCharge");
			engine->StopSample("kingDDDDownBPrepare");
			engine->StopSample("kingDDDDownBAttack");

			if (vy <= 0)
				nGraphicCounter = nJumpingFrame;
			else
				nGraphicCounter = nFallingFrame;

			if (px <= fMiddleOfStage)
				vx = cfRunSpeed;
			else
				vx = -cfRunSpeed;

			nGraphicState = JUMPING;

			if (bJumpForRecover)
			{
				vy = cfRecoverJumpSpeed;
				bJumpForRecover = false;
			}
			else
			{
				ReturnMovingStateIfOnGround(fElapsedTime, true, fWaitAfterRecoveryTime);
			}
		}
		break;
	}

	// The spritesheet is very big, so some animations are only drawn once
	if (IsOneFrameAnimation())
		nGraphicCounter = 0;

	// If King DDD falls, he comes back with a jump
	CheckIfHasFallen();

	UpdateTimers();

	nAIState = nAINextState;
}

bool cDynamicCreatureKingDDD::IsOneFrameAnimation()
{
	return nGraphicState == DAMAGED || nGraphicState == STANDING;
}

void cDynamicCreatureKingDDD::ChangeState(AI_STATE state, bool resetBehaviourTimer)
{
	nGraphicCounter = 0;
	fBehaviourTimer = 0.0f;
	fBehaviourTimer2 = 0.0f;
	nAINextState = state;
	bCanSpawnAOE = true;
	bPlayLandingSound = true;
}

void cDynamicCreatureKingDDD::ChangeVacuumState(VACUUMSTATE state)
{
	fBehaviourTimer = 0.0f;
	nVacuumNextState = state;
}

void cDynamicCreatureKingDDD::UpdateTimers()
{
	if (nHealth >= cnHiHP)
	{
		fWaitingTime = cfWaitingTimeMaxHp;
		fSmashChargeTime = cfSmashChargeTimeMaxHp;
		fWaitAfterSmashTime = cfWaitAfterSmashTimeMaxHp;
	}
	else if (nHealth < cnHiHP && nHealth >= cnMiHP)
	{
		fWaitingTime = cfWaitingTimeHiHp;
		fSmashChargeTime = cfSmashChargeTimeHiHp;
		fWaitAfterSmashTime = cfWaitAfterSmashTimeHiHp;
	}
	else if (nHealth < cnMiHP && nHealth >= cnLoHP)
	{
		fWaitingTime = cfWaitingTimeMiHp;
		fSmashChargeTime = cfSmashChargeTimeMiHp;
		fWaitAfterSmashTime = cfWaitAfterSmashTimeMiHp;
	}
	else
	{
		fWaitingTime = cfWaitingTimeLoHp;
		fSmashChargeTime = cfSmashChargeTimeLoHp;
		fWaitAfterSmashTime = cfWaitAfterSmashTimeLoHp;
	}
}

void cDynamicCreatureKingDDD::LoopAnimation(int firstFrame, int lastFrame)
{
	if (nGraphicCounter < firstFrame || nGraphicCounter > lastFrame)
		nGraphicCounter = firstFrame;
}

void cDynamicCreatureKingDDD::SingleAnimation(int firstFrame, int lastFrame)
{
	if (nGraphicCounter < firstFrame) nGraphicCounter = firstFrame;
	if (nGraphicCounter > lastFrame) nGraphicCounter = lastFrame;
}

bool cDynamicCreatureKingDDD::OnGround()
{
	return
		engine->IsSolidTile(level->GetTile(px + cfCollisionLowerLimit, py + fKingDDDHeight)) ||
		engine->IsSolidTile(level->GetTile(px + cfCollisionUpperLimit, py + fKingDDDHeight)) ||
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionLowerLimit, py + fKingDDDHeight)) ||
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionUpperLimit, py + fKingDDDHeight));
}

bool cDynamicCreatureKingDDD::OnSemiSolidPlatform()
{
	return
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionLowerLimit, py + fKingDDDHeight)) ||
		engine->IsSemiSolidTile(level->GetTile(px + cfCollisionUpperLimit, py + fKingDDDHeight));
}

void cDynamicCreatureKingDDD::ReturnMovingStateIfOnGround(float fElapsedTime, bool waitAfterJump, float timeToWait, GraphicState graphicState)
{
	if (OnGround() && vy >= 0)
	{
		if (waitAfterJump)
		{
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer >= timeToWait)
			{
				bHasNotAlreadyJumped = true;
				ChangeState(AI_MOVING);
			}
			else
			{
				vx = 0.0f; vy = 0.0f;
				nGraphicState = graphicState;

				// On landing after Up B, the landing sounds occurs
				if (graphicState == MOVE6 && bPlayLandingSound)
				{
					engine->PlaySample("kingDDDSideSmash");
					engine->PlaySample("kingDDDUpBLanding");

					bPlayLandingSound = false;
				}
			}
		}
		else
		{
			bHasNotAlreadyJumped = true;
			ChangeState(AI_MOVING);
		}
	}
}

void cDynamicCreatureKingDDD::CheckIfHasFallen()
{
	if (py >= fLowerLimitOfStage)
	{
		if (px <= fMiddleOfStage)
			px = fLeftRecoveryX;
		else
			px = fRightRecoveryX;

		py = fLowerLimitOfStage;

		bJumpForRecover = true;

		ChangeState(AI_RECOVERY);
	}
}

bool cDynamicCreatureKingDDD::KirboIsInUpArea(float playerX, float playerY)
{
	// Check all boudaries
	return
		(
			(playerX + 0.0f >= px + cfUpAreaLeftPosX && playerX + 0.0f <= px + cfUpAreaRightPosX) ||
			(playerX + 1.0f >= px + cfUpAreaLeftPosX && playerX + 1.0f <= px + cfUpAreaRightPosX)) &&
		(
			(playerY + 0.0f >= py + cfUpAreaTopPosY && playerY + 0.0f <= py + cfUpAreaBotPosY) ||
			(playerY + 1.0f >= py + cfUpAreaTopPosY && playerY + 1.0f <= py + cfUpAreaBotPosY));
}

bool cDynamicCreatureKingDDD::KirboIsInLeftArea(float playerX, float playerY)
{
	// Check all boudaries
	return
		(
			(playerX + 0.0f >= px + cfLeftAreaLeftPosX && playerX + 0.0f <= px + cfLeftAreaRightPosX) ||
			(playerX + 1.0f >= px + cfLeftAreaLeftPosX && playerX + 1.0f <= px + cfLeftAreaRightPosX)) &&
		(
			(playerY + 0.0f >= py + cfLeftAreaTopPosY && playerY + 0.0f <= py + cfLeftAreaBotPosY) ||
			(playerY + 1.0f >= py + cfLeftAreaTopPosY && playerY + 1.0f <= py + cfLeftAreaBotPosY));
}

bool cDynamicCreatureKingDDD::KirboIsInRightArea(float playerX, float playerY)
{
	// Check all boudaries
	return
		(
			(playerX + 0.0f >= px + cfRightAreaLeftPosX && playerX + 0.0f <= px + cfRightAreaRightPosX) ||
			(playerX + 1.0f >= px + cfRightAreaLeftPosX && playerX + 1.0f <= px + cfRightAreaRightPosX)) &&
		(
			(playerY + 0.0f >= py + cfRightAreaTopPosY && playerY + 0.0f <= py + cfRightAreaBotPosY) ||
			(playerY + 1.0f >= py + cfRightAreaTopPosY && playerY + 1.0f <= py + cfRightAreaBotPosY));
}

bool cDynamicCreatureKingDDD::KirboIsInDownArea(float playerX, float playerY)
{
	// Check all boudaries
	return
		(
			(playerX + 0.0f >= px + cfDownAreaLeftPosX && playerX + 0.0f <= px + cfDownAreaRightPosX) ||
			(playerX + 1.0f >= px + cfDownAreaLeftPosX && playerX + 1.0f <= px + cfDownAreaRightPosX)) &&
		(
			(playerY + 0.0f >= py + cfDownAreaTopPosY && playerY + 0.0f <= py + cfDownAreaBotPosY) ||
			(playerY + 1.0f >= py + cfDownAreaTopPosY && playerY + 1.0f <= py + cfDownAreaBotPosY));
}

bool cDynamicCreatureKingDDD::KirboIsInFrontOfDDD(float playerX, float playerY)
{
	return (KirboIsInLeftArea(playerX, playerY) && nFaceDir == 0) || (KirboIsInRightArea(playerX, playerY) && nFaceDir == 1);
}

bool cDynamicCreatureKingDDD::KirboIsBehindDDD(float playerX, float playerY)
{
	return (KirboIsInLeftArea(playerX, playerY) && nFaceDir == 1) || (KirboIsInRightArea(playerX, playerY) && nFaceDir == 0);
}

void cDynamicCreatureKingDDD::CrossPlatform()
{
	py += cfCrossPlatformGap;
}
