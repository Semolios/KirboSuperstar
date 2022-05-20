#include "Platformer_DynamicCreatureKracko.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureKracko::engine = nullptr;

cDynamicCreatureKracko::cDynamicCreatureKracko(cLevel* l) : cDynamicCreature("kracko", cAssets::get().GetSprite("kracko"), 16)
{
	fDynWidth = 192.0f;
	fDynHeight = 128.0f;
	fSpriteW = 320.0f;
	fSpriteH = 192.0f;
	fSpriteOffsetX = -64.0f;
	fSpriteOffsetY = -32.0f;
	bFriendly = false;
	nHealth = 100;
	nHealthMax = 100;
	level = l;
	bSolidVsMap = false;
	bAffectedByGravity = false;
	nDamage = 3;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureKracko::~cDynamicCreatureKracko()
{
	delete hitbox;
	delete sSprite;
	delete level;
}

void cDynamicCreatureKracko::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// don't loop Damaged animation
	bLoopAnimation = nGraphicState != DAMAGED;

	if (nGraphicState != DAMAGED)
		LookKirbo(playerX);

	switch (nAIState)
	{
		case AI_IDLE:
		{
			vx = 0.0f; vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			bCanSpawnAOE = true;

			AimForKirboUnder(playerX, playerY);

			if (fBehaviourTimer >= fWaitingTime)
			{
				// if kirbo is far away from kracko, he doesn't use the LIGHTNING AROUND or THUNDER STORM,
				if (abs(px + cfMiddleOfKracko - playerX) > cfKirboIsNearDistance)
					nChosenAttack = rand() % (cnNumberOfAttack - 2);
				else
					nChosenAttack = rand() % cnNumberOfAttack;

				if (nChosenAttack == 0) ChangeState(AI_MOVING);
				if (nChosenAttack == 1) ChangeState(AI_SWIPEAREA);
				if (nChosenAttack == 2) ChangeState(AI_AIMKIRBO);
				if (nChosenAttack == 3) ChangeState(AI_THUNDERSTORM);
				if (nChosenAttack == 4) ChangeState(AI_LIGHTNINGAROUND);
			}
		}
		break;
		case AI_MOVING:
		{
			vx = (playerX < px + cfMiddleOfKracko) ? -fMovingSpeed : fMovingSpeed;
			vy = 0.0f;
			fBehaviourTimer += fElapsedTime;

			AimForKirboUnder(playerX, playerY);
		}
		break;
		case AI_THUNDERUNDER:
		{
			vx = 0.0f; vy = 0.0f;
			fBehaviourTimer2 += fElapsedTime;

			if (fBehaviourTimer2 >= fThunderSpawnTime && bCanSpawnAOE)
			{
				// after 0.5 seconds, spawn a thunder under kracko
				engine->AddProjectile(px + fThunderOffsetX, py + fThunderOffsetY, false, 0.0f, 0.0f, fThunderUnderSpawnTime, "thunderUnder", false, cnAttacksDmg, false, false, 0, false);
				engine->PlaySample("lightning");

				bCanSpawnAOE = false;
				ChangeState(AI_IDLE, false);
			}
		}
		break;
		case AI_SWIPEAREA:
		{
			// Define kirbo position to plunge on him
			if (fBehaviourTimer == 0.0f)
			{
				fPlungeDirection = (playerX < px + 1.0f) ? -1.0f : 1.0f;
			}

			fBehaviourTimer += fElapsedTime;
			vx = sinf(fBehaviourTimer * pi) * fSwipeAmplitude * fPlungeDirection;
			vy = sinf(fBehaviourTimer * pi * 2.0f) * fSwipeAmplitude;

			// Control kracko don't go out of bound
			if (vx <= 0.0f && px <= cfLBoundOfStage) vx = 0.0f;
			if (vx >= 0.0f && px >= cfRBoundOfStage) vx = 0.0f;

			if (fBehaviourTimer >= fSwipeTime)
			{
				py = cfInitPX;
				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_AIMKIRBO:
		{
			vx = 0.0f; vy = 0.0f;

			if (fBehaviourTimer == 0.0f)
			{
				engine->AddProjectile(px + cfMiddleOfKracko, py + cfMiddleOfKracko, true, 0.0f, 0.0f, fAimingTime + fFireTime, "chargeLightning", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
				engine->PlaySample("electricity");
			}

			fBehaviourTimer += fElapsedTime;

			nGraphicState = MOVE3;
			nGraphicCounter = 0;

			if (fBehaviourTimer <= fAimingTime)
			{
				// during aiming time, a target spawn on kirbo to indicate the player to move
				engine->AddProjectile(playerX, playerY, true, 0.0f, 0.0f, 0.05f, "aim", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
				fPlayerPosX = playerX;
				fPlayerPosY = playerY;
			}
			else if (fBehaviourTimer <= fAimingTime + fFireTime)
			{
				engine->AddProjectile(fPlayerPosX, fPlayerPosY, true, 0.0f, 0.0f, 0.05f, "aim", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
			}
			else
			{
				float fLightningX = 0.0f;
				float fLightningY = 0.0f;

				// to determine where to fire the lightning, I defined a circle of 600px radius (half lightning.png width)
				// then I define the equation of the line with (px + 1.5, py + 1.0) and (fPlayerPosX + 0.5, fPlayerPosY + 0.5)
				// and finally the position of the lightning will be the intersection between the line and the circle.
				// First, if playerX == px, the slope of the line is infinite, kirbo is above kracko or below, I can fix the X and Y values
				// else, define if kirbo is on the left or on the right from kracko, to determine which intersection to take
				if (px + 1.5f == fPlayerPosX + 0.5f)
				{
					fLightningX = px + 1.5f;
					fLightningY = fPlayerPosY + 0.5f >= py + 1.0f
						? py + 1.0f + (fLightningSemiWidth / engine->GetTileWidth())
						: py + 1.0f - (fLightningSemiWidth / engine->GetTileWidth());
				}
				else
				{
					/*
					equation of the circle :
						(x - px)² + (y - py)² = r²

					equation of the line :
						y = slope.x + b

						replacement of x and y by a known point (kracko's coordinates)
							py = slope.px + b => b = py - slope.px
						y = slope.x + py - slope.px

					replacement in the equation of the circle of y by his equation in the line :
						(x - px)² + (slope.x + py - slope.px - py)² = r²
						(x - px)² + (slope.x - slope.px)² = r²
						x² + slope².x² - 2.px.x - 2slope².px.x + px² + slope².px² - r² = 0
						(slope² + 1).x² + (-2.px - 2.slope².px).x + (px² + slope².px² - r²) = 0
					*/

					float x = px + 1.5f;	// middle of kracko
					float y = py + 1.0f;	// middle of kracko

					float slope = (y - (fPlayerPosY + 0.5)) / (x - (fPlayerPosX + 0.5));				   // slope of the line formed by the player last position and kracko position
					float radius = fLightningSemiWidth / engine->GetTileWidth();						   // radius of the circle around kracko (semi width of the lightning)

					float a = ((slope * slope) + 1.0f);													   // a factor of the equation to resolve the intersection between the circle and the line
					float b = ((-2.0f * x) - (2.0f * (slope * slope) * x));								   // b factor of the equation to resolve the intersection between the circle and the line
					float c = ((x * x) + (slope * slope * x * x) - (radius * radius));					   // c factor of the equation to resolve the intersection between the circle and the line
					float deltasqrt = sqrtf((b * b) - (4.0f * a * c));									   // b²-4ac

					float x1 = (-b + deltasqrt) / (2.0f * a);											   // first solution of the equation
					float x2 = (-b - deltasqrt) / (2.0f * a);											   // second solution of the equation

					fLightningX = px + 1.5f >= fPlayerPosX + 0.5f ? std::min(x1, x2) : std::max(x1, x2);   // if kirbo is on the left from kracko, the x coordinate of the intersection is the lowest
					fLightningY = (slope * fLightningX) + y - (slope * x);
				}

				engine->StopSample("electricity");

				engine->AddProjectile(fLightningX - (fLightningSemiWidth / engine->GetTileWidth()), fLightningY - (fLightningSemiHeight / engine->GetTileWidth()), false, fPlayerPosX - (px + cfMiddleOfKracko), fPlayerPosY - (py + cfMiddleOfKracko), fAimingLightningDuration, "lightning", false, cnAttacksDmg, false, false);
				engine->PlaySample("lightning");

				ChangeState(AI_IDLE);
			}
		}
		break;
		case AI_THUNDERSTORM:
		{
			vx = 0.0f; vy = 0.0f;

			if (fBehaviourTimer == 0.0f)
			{
				fLightningPosX1 = px + cfMiddleOfKracko;
				fLightningPosX2 = px + cfMiddleOfKracko;
				engine->AddProjectile(fLightningPosX1, fGroundPositionY, true, 0.0f, 0.0f, fFireTime, "chargeLightning", false, 0, false);
				engine->PlaySample("electricity");
			}

			fBehaviourTimer += fElapsedTime;
			fBehaviourTimer2 += fElapsedTime;

			if (fBehaviourTimer2 >= fFireTime)
			{
				// Sometimes, the last lightning don't fire because fBehaviourTimer > fNumberofAttacks * fFireTime but fBehaviour < fFireTime so I force the firing later
				bFireLastLightning = false;

				engine->StopSample("electricity");

				engine->AddProjectile(fLightningPosX1 - (fLightningSemiWidth / engine->GetTileWidth()), fLightningPosY, false, 0.0f, fLightningSpdY, fLightningDuration, "lightning", false, cnAttacksDmg, false);
				engine->AddProjectile(fLightningPosX2 - (fLightningSemiWidth / engine->GetTileWidth()), fLightningPosY, false, 0.0f, fLightningSpdY, fLightningDuration, "lightning", false, cnAttacksDmg, false);
				engine->PlaySample("lightning");

				fBehaviourTimer2 = 0.0f;

				fLightningPosX1 += fLightningOffsetX;
				fLightningPosX2 -= fLightningOffsetX;

				if (fBehaviourTimer < fNumberOfAttacks * fFireTime)
				{
					engine->AddProjectile(fLightningPosX1, fGroundPositionY, true, 0.0f, 0.0f, fFireTime, "chargeLightning", false, 0, false);
					engine->AddProjectile(fLightningPosX2, fGroundPositionY, true, 0.0f, 0.0f, fFireTime, "chargeLightning", false, 0, false);
					engine->PlaySample("electricity");
				}
			}

			nGraphicState = MOVE3;
			nGraphicCounter = 0;

			if (fBehaviourTimer >= fNumberOfAttacks * fFireTime)
			{
				if (bFireLastLightning)
				{
					engine->StopSample("electricity");

					engine->AddProjectile(fLightningPosX1 - (fLightningSemiWidth / engine->GetTileWidth()), fLightningPosY, false, 0.0f, fLightningSpdY, fLightningDuration, "lightning", false, cnAttacksDmg, false);
					engine->AddProjectile(fLightningPosX2 - (fLightningSemiWidth / engine->GetTileWidth()), fLightningPosY, false, 0.0f, fLightningSpdY, fLightningDuration, "lightning", false, cnAttacksDmg, false);
					engine->PlaySample("lightning");
				}

				fBehaviourTimer2 = 0.0f;
				ChangeState(AI_IDLE);
			}

			bFireLastLightning = true;
		}
		break;
		case AI_LIGHTNINGAROUND:
		{
			vx = 0.0f; vy = 0.0f;

			nGraphicState = MOVE3;
			nGraphicCounter = 0;

			if (fBehaviourTimer2 == 0)
			{
				engine->AddProjectile(px + fThunderOffsetX, py, false, 0.0f, 0.0f, fFireTime, "thunderShot", false, cnAttacksDmg, false, false, 0, false);
			}

			fBehaviourTimer2 += fElapsedTime;

			if (fBehaviourTimer2 >= fFireTime && bCanSpawnAOE)
			{
				bCanSpawnAOE = false;

				// define positions of the four lightnings around
				float fLightning1X = px + fInitLightning1X - fLightningSprSemiW;
				float fLightning1Y = py + fInitLightning1Y - fLightningSprSemiH;

				float fLightning2X = px + fInitLightning2X - fLightningSprSemiW;
				float fLightning2Y = py + fInitLightning2Y - fLightningSprSemiH;

				float fLightning3X = px + fInitLightning3X - fLightningSprSemiW;
				float fLightning3Y = py + fInitLightning3Y - fLightningSprSemiH;

				float fLightning4X = px + fInitLightning4X - fLightningSprSemiW;
				float fLightning4Y = py + fInitLightning4Y - fLightningSprSemiH;

				float x = px + 1.5f - fLightningSprSemiW;	// middle of kracko
				float y = py + 1.0f - fLightningSprSemiH;	// middle of kracko

				engine->PlaySample("electricity2");
				engine->AddOrbital(fLightning1X, fLightning1Y, false, fLightningAroundTime, "lightningAround", cnAttacksDmg, false, false, 0, x, y, fAngrot);
				engine->AddOrbital(fLightning2X, fLightning2Y, false, fLightningAroundTime, "lightningAround", cnAttacksDmg, false, false, 0, x, y, fAngrot);
				engine->AddOrbital(fLightning3X, fLightning3Y, false, fLightningAroundTime, "lightningAround", cnAttacksDmg, false, false, 0, x, y, fAngrot);
				engine->AddOrbital(fLightning4X, fLightning4Y, false, fLightningAroundTime, "lightningAround", cnAttacksDmg, false, false, 0, x, y, fAngrot);
			}

			if (fBehaviourTimer2 >= fFireTime + fLightningAroundTime)
			{
				engine->StopSample("electricity2");
				ChangeState(AI_IDLE, false);
			}
		}
		break;
	}

	UpdateTimers();

	nAIState = nAINextState;
}

void cDynamicCreatureKracko::AimForKirboUnder(float playerX, float playerY)
{
	if (playerX >= px + 0.5f && playerX <= px + cfMiddleOfKracko && playerY >= py)
	{
		// Spawn a little lightning under kracko to tell the player something is gonna happen
		engine->AddProjectile(px + fThunderOffsetX, py + fThunderOffsetY, false, 0.0f, 0.0f, fThunderSpawnTime, "thunderShot", false, cnAttacksDmg, false, false, 0, false);
		ChangeState(AI_THUNDERUNDER, false);
	}
	else if (playerX >= px + 0.5f && playerX <= px + cfMiddleOfKracko && playerY <= py)
	{
		ChangeState(AI_LIGHTNINGAROUND, false);
	}
}

void cDynamicCreatureKracko::ChangeState(AI_STATE state, bool resetBehaviourTimer)
{
	nGraphicCounter = 0;
	if (resetBehaviourTimer)
		fBehaviourTimer = 0.0f;
	fBehaviourTimer2 = 0.0f;
	nAINextState = state;
}

void cDynamicCreatureKracko::UpdateTimers()
{
	if (nHealth >= cnHiHP)
	{
		fWaitingTime = cfWaitingTimeMaxHp;
		fMovingSpeed = cfMovingSpeedMaxHp;
		fAimingTime = cfAimingTimeMaxHp;
		fFireTime = cfFireTimeMaxHp;
	}
	else if (nHealth < cnHiHP && nHealth >= cnMiHP)
	{
		fWaitingTime = cfWaitingTimeHiHp;
		fMovingSpeed = cfMovingSpeedHiHp;
		fAimingTime = cfAimingTimeHiHp;
		fFireTime = cfFireTimeHiHp;
	}
	else if (nHealth < cnMiHP && nHealth >= cnLoHP)
	{
		fWaitingTime = cfWaitingTimeMiHp;
		fMovingSpeed = cfMovingSpeedMiHp;
		fAimingTime = cfAimingTimeMiHp;
		fFireTime = cfFireTimeMiHp;
	}
	else
	{
		fWaitingTime = cfWaitingTimeLoHp;
		fMovingSpeed = cfMovingSpeedLoHp;
		fAimingTime = cfAimingTimeLoHp;
		fFireTime = cfFireTimeLoHp;
	}
}

void cDynamicCreatureKracko::LookKirbo(float playerX)
{
	nFaceDir = (playerX < px + cfMiddleOfKracko) ? 0 : 1;
}
