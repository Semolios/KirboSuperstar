#include "Platformer_DynamicCreatureFrosty.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureFrosty::engine = nullptr;

cDynamicCreatureFrosty::cDynamicCreatureFrosty(cLevel* l) : cDynamicCreature("frosty", cAssets::get().GetSprite("frosty"), 8)
{
	fDynWidth = 128.0f;
	fDynHeight = 128.0f;
	fSpriteW = 256.0f;
	fSpriteH = 192.0f;
	fSpriteOffsetX = -64.0f;
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

cDynamicCreatureFrosty::~cDynamicCreatureFrosty()
{
	delete hitbox;
	delete sSprite;
	delete level;
}

void cDynamicCreatureFrosty::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	switch (nAIState)
	{
		case AI_IDLE:
		{
			vx = 0.0f; vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			bCanSpawnAOE = true;

			if (fBehaviourTimer >= fWaitingTime)
			{
				// if kirbo is far away from frosty, he doesn't use the ice blow,
				if (abs(px - playerX) > 5.0f && nChosenAttack == 3)
					nChosenAttack = rand() % (cnNumberOfAttack - 1);
				else
					nChosenAttack = rand() % cnNumberOfAttack;

				if (nChosenAttack == 0) ChangeState(AI_WALKING);
				if (nChosenAttack == 1) ChangeState(AI_ICECUBE);
				if (nChosenAttack == 2) ChangeState(AI_JUMPING);
				if (nChosenAttack == 3) ChangeState(AI_ICEBLOW);
			}
		}
		break;
		case AI_WALKING:
		{
			vx = (playerX < px) ? -fWalkingSpeed : fWalkingSpeed;
			vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer >= fWalkingTime)
			{
				fGlideDirection = (playerX < px) ? -1.0f : 1.0f;
				ChangeState(AI_GLIDING);
			}
		}
		break;
		case AI_GLIDING:
		{
			if (!olc::SOUND::IsSamplePlaying(engine->GetSound("superstar")))
				olc::SOUND::PlaySample(engine->GetSound("superstar"));

			vx = fGlidingSpeed * fGlideDirection;
			nGraphicState = SIDEATTACK;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer >= fGlidingTime || px <= cfMinPosX || px >= cfMaxPosX)
			{
				olc::SOUND::StopSample(engine->GetSound("superstar"));
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_ICECUBE:
		{
			vx = 0.0f; vy = 0.0f;
			nGraphicState = UPATTACK;

			// Frosty can't turn around if kirbo jump over his back
			if (fBehaviourTimer == 0.0f)
			{
				olc::SOUND::PlaySample(engine->GetSound("patpat"));

				fIceCubeDirection = (playerX < px) ? -1.0f : 1.0f;
				nFaceDir = (playerX < px) ? 0 : 1;
			}

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer >= fIceCubeSpawnTime && bCanSpawnAOE)
			{
				if (sIceCube == csIceCubeLoHp)
				{
					if (nFaceDir == 0) sIceCube = "hugeIceCubeLeft";
					if (nFaceDir == 1) sIceCube = "hugeIceCubeRight";
					fIceCubeOffset = 100.0f; // Offset the huge ice cube so it does not bug against tiles
				}

				olc::SOUND::PlaySample(engine->GetSound("itemLaunch"));

				engine->AddProjectile(engine->CreateProjectile(px, py - (fIceCubeOffset / engine->GetTileHeight()), false, fIceCubeDirection * fIceCubeVX, fIceCubeVY, fIceCubeDuration, sIceCube, true, nIceCubeDmg, true, true, 0, false, fIceCubeDrag));
				bCanSpawnAOE = false;
			}

			if (fBehaviourTimer >= fIceCubeAnimationDuration)
				ChangeState(AI_IDLE);
		}
		break;
		case AI_ICEBLOW:
		{
			vx = 0.0f; vy = 0.0f;
			nGraphicState = DOWNATTACK;

			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fWaitBeforeBlow)
			{
				nGraphicCounter = 0;
			}
			else if (fBehaviourTimer > fWaitBeforeBlow && fBehaviourTimer <= (fWaitBeforeBlow + fBlowTime))
			{
				nGraphicCounter = 1;

				if (bCanSpawnAOE)
				{
					olc::SOUND::PlaySample(engine->GetSound("iceBlow"));

					// Spawn the wind around Frosty
					engine->ActivateShakeEffect(true, 15, 15);
					engine->AddProjectile(engine->CreateProjectile(px + fWindOffsetX, py, false, 0.0f, 0.0f, fBlowTime, "frostyWind", false, nWindDmg, false));
					bCanSpawnAOE = false;
				}
			}
			else
			{
				olc::SOUND::StopSample(engine->GetSound("iceBlow"));

				engine->ActivateShakeEffect(false);
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_JUMPING:
		{
			fBehaviourTimer += fElapsedTime;
			nGraphicState = JUMPING;

			if (fBehaviourTimer <= fWaitBeforeJump && nRemainingJumps == nMaxJumpsNumber)
			{
				vx = 0.0f; vy = 0.0f;
				nGraphicCounter = 0;
			}
			else
			{
				olc::SOUND::PlaySample(engine->GetSound("frostyJump"));

				vx = (playerX < px) ? -fJumpVX : fJumpVX;
				vy = fJumpVY;

				nRemainingJumps--;
				ChangeState(AI_JUMPED);
			}
		}
		break;
		case AI_JUMPED:
		{
			nGraphicState = JUMPING;

			if (vy <= 0)
				nGraphicCounter = 1;
			else if (vy > 0)
				nGraphicCounter = 2;

			if (px <= playerX && px + 2 > playerX + 1)
				ChangeState(AI_FALLING);
			else if (OnGround())
				JumpAgainOrReturnIdleState();
		}
		break;
		case AI_FALLING:
		{
			nGraphicState = JUMPING;
			fBehaviourTimer += fElapsedTime;

			if (fBehaviourTimer <= fWaitBeforeFall)
			{
				vx = 0.0f; vy = 0.0f;
				nGraphicCounter = 3;
			}
			else
			{
				nGraphicCounter = 4;

				if (OnGround())
				{
					if (!olc::SOUND::IsSamplePlaying(engine->GetSound("frostyGroundPound")))
						olc::SOUND::PlaySample(engine->GetSound("frostyGroundPound"));

					fGroundPoundTimer += fElapsedTime;
					if (fGroundPoundTimer <= fGroundPoundShakeEffectTimer)
					{
						engine->ActivateShakeEffect(true, 15, 15);
					}
					else
					{
						fGroundPoundTimer = 0.0f;
						engine->ActivateShakeEffect(false);
						JumpAgainOrReturnIdleState();
					}
				}
				else
				{
					vy = fFallVY;
				}
			}
		}
		break;
	}

	UpdateTimers();

	nAIState = nAINextState;
}

void cDynamicCreatureFrosty::ChangeState(AI_STATE state)
{
	nGraphicCounter = 0;
	fBehaviourTimer = 0.0f;
	nAINextState = state;
}

void cDynamicCreatureFrosty::UpdateTimers()
{
	if (nHealth >= cnHiHP)
	{
		fWaitingTime = cfWaitingTimeMaxHp;
		fWalkingSpeed = cfWalkingSpeedMaxHp;
		fGlidingSpeed = cfGlidingSpeedMaxHp;
		fWaitBeforeBlow = cfWaitingBeforeBlowMaxHp;
		fWaitBeforeFall = cfWaitingBeforeFallMaxHp;
		nMaxJumpsNumber = cnMaxJumpNumberMaxHp;
		sIceCube = csIceCubeMaxHp;
	}
	else if (nHealth < cnHiHP && nHealth >= cnMiHP)
	{
		fWaitingTime = cfWaitingTimeHiHp;
		fWalkingSpeed = cfWalkingSpeedHiHp;
		fGlidingSpeed = cfGlidingSpeedHiHp;
		fWaitBeforeBlow = cfWaitingBeforeBlowHiHp;
		fWaitBeforeFall = cfWaitingBeforeFallHiHp;
		nMaxJumpsNumber = cnMaxJumpNumberHiHp;
		sIceCube = csIceCubeHiHp;
	}
	else if (nHealth < cnMiHP && nHealth >= cnLoHP)
	{
		fWaitingTime = cfWaitingTimeMiHp;
		fWalkingSpeed = cfWalkingSpeedMiHp;
		fGlidingSpeed = cfGlidingSpeedMiHp;
		fWaitBeforeBlow = cfWaitingBeforeBlowMiHp;
		fWaitBeforeFall = cfWaitingBeforeFallMiHp;
		nMaxJumpsNumber = cnMaxJumpNumberMiHp;
		sIceCube = csIceCubeMiHp;
	}
	else
	{
		fWaitingTime = cfWaitingTimeLoHp;
		fWalkingSpeed = cfWalkingSpeedLoHp;
		fGlidingSpeed = cfGlidingSpeedLoHp;
		fWaitBeforeBlow = cfWaitingBeforeBlowLoHp;
		fWaitBeforeFall = cfWaitingBeforeFallLoHp;
		nMaxJumpsNumber = cnMaxJumpNumberLoHp;
		sIceCube = csIceCubeLoHp;
	}
}

bool cDynamicCreatureFrosty::OnGround()
{
	return (engine->IsSolidTile(level->GetTile(px, py + 2)) || engine->IsSolidTile(level->GetTile(px + 2, py + 2))) && vy >= 0;
}

void cDynamicCreatureFrosty::JumpAgainOrReturnIdleState()
{
	if (nRemainingJumps == 0)
	{
		nRemainingJumps = nMaxJumpsNumber;
		ChangeState(AI_IDLE);
	}
	else
	{
		ChangeState(AI_JUMPING);
	}
}
