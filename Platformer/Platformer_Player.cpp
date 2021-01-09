#include "Platformer_Player.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cPlayer::engine = nullptr;

cPlayer::cPlayer(cAnimator* animator)
{
	animPlayer = animator;
}

void cPlayer::InitialiseKirboHealth()
{
	fHealth = cfMaxHealth;
}

bool cPlayer::IsDead()
{
	return bDead;
}

void cPlayer::HandleInput(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	if (engine->IsFocused() && CanInteract())
	{
		// Fly, enter a door
		if (gfx->GetKey(olc::Key::UP).bHeld)
		{
			if (!bAttacking && !bVacuuming)
			{
				if (engine->GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f) == L'w' && bPlayerOnGround)
					engine->SetGameState("GS_LOADBOSSLEVEL");

				fPlayerVelY = -cfPlayerVelY;
				bFlying = true;
				animPlayer->ChangeState("flying");
			}
		}

		// Go down when flying, cross semi solid platform and control camera when onground
		if (gfx->GetKey(olc::Key::DOWN).bHeld)
		{
			if (bFlying)
				fPlayerVelY = cfPlayerVelY;

			// If player is on semi solid platform, pass through the platform. cheat a little bit, modify the position of the player to cross it
			if ((engine->IsSemiSolidTile(engine->GetTile(fPlayerPosX + 0.0f, fPlayerPosY + 1.0f)) ||
				 engine->IsSemiSolidTile(engine->GetTile(fPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f))) && bPlayerOnGround)
			{
				fPlayerPosY += 0.15;
			}

			if ((engine->IsSolidTile(engine->GetTile(fPlayerPosX + 0.0f, fPlayerPosY + 1.0f)) ||
				 engine->IsSolidTile(engine->GetTile(fPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f))) && bPlayerOnGround)
			{
				engine->LowerCameraPosition();
			}
		}
		else
		{
			engine->RaiseCameraPosition();
		}

		// Go left
		if (gfx->GetKey(olc::Key::LEFT).bHeld)
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
		if (gfx->GetKey(olc::Key::RIGHT).bHeld)
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
		if (gfx->GetKey(olc::Key::SPACE).bPressed)
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
		if (gfx->GetKey(olc::Key::SPACE).bHeld)
		{
			if (bChargeJump)
			{
				fJumpTimer += fElapsedTime;
				fPlayerVelY = cfJumpAcceleration - (cfJumpDecelerationRatio * fJumpTimer * cfJumpAcceleration);
				if (fJumpTimer >= cfJumpTime)
				{
					fJumpTimer = 0.0f;
					bChargeJump = false;
				}
			}
			else if (bChargeDoubleJump)
			{
				fJumpTimer += fElapsedTime;
				fPlayerVelY = cfJumpAcceleration - (cfJumpDecelerationRatio * fJumpTimer * cfJumpAcceleration);
				if (fJumpTimer >= cfDblJumpTime)
				{
					fJumpTimer = 0.0f;
					bChargeDoubleJump = false;
				}
			}
		}

		// if you release space, jump is cancelled so you can't spam space to glide
		if (gfx->GetKey(olc::Key::SPACE).bReleased)
		{
			if (bChargeJump)
			{
				fJumpTimer = 0.0f;
				bChargeJump = false;
			}
			if (bChargeDoubleJump)
			{
				fJumpTimer = 0.0f;
				bChargeDoubleJump = false;
			}
		}

		// Slap attack
		if (gfx->GetKey(olc::Key::F).bPressed)
		{
			// Can't spam slap, can't slap when player is flying
			if (!bAttacking && !bFlying)
			{
				animPlayer->ChangeState("slap");
				bAttacking = true;
				bSlapping = true;
				bCanSpawnProjectile = true;
				fAnimationTimer = 0.0f;
			}
		}

		// Pause
		if (gfx->GetKey(olc::Key::P).bPressed)
		{
			engine->SetPlayerChoice(0);

			// You can't use pause when you are hit.
			// like you can't pause when you jump in mario sunshine so you can't leave the level when you are falling
			engine->SetGameState("GS_PAUSE");
			engine->BreakLoop();
			return;
		}

		// Launch a Jesus Cross
		if (gfx->GetKey(olc::Key::R).bPressed)
		{
			// Can't spam Launching cross, can't launch when player is flying
			if (!bAttacking && !bFlying)
			{
				animPlayer->ChangeState("jesus_christ");
				bAttacking = true;
				bLaunchingJesusCross = true;
				bCanSpawnProjectile = true;
				fAnimationTimer = 0.0f;
			}
		}

		// Vacuum attack
		if (gfx->GetKey(olc::Key::E).bHeld)
		{
			// can't Vacuum when player is attacking, swallowing or flying
			if (!bFlying && !bSwallowing)
			{
				if (!bVacuuming && !bAttacking)
				{
					animPlayer->ChangeState("begin_vacuum");
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
}

bool cPlayer::CanInteract()
{
	return !bPlayerDamaged && !bDead && !engine->IsBossKilled();
}

void cPlayer::ApplyGravity(float fElapsedTime)
{
	fPlayerVelY += engine->GetGravityValue() * fElapsedTime;
}

void cPlayer::Update(float fElapsedTime)
{
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

			fPlayerVelX += engine->GetDragValue() * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < cfMinPlayerVelX)
			{
				fPlayerVelX = 0.0f;

				if (CanInteract())
					animPlayer->ChangeState("idle");
			}
			else
			{
				if (CanInteract())
					animPlayer->ChangeState("run");
			}
		}
		else
		{
			if (CanInteract())
			{
				if (!bFlying)
				{
					if (fPlayerVelY < 0)
						animPlayer->ChangeState("jump");
					else
						animPlayer->ChangeState("fall");
				}
			}
		}
	}
}

float cPlayer::GetFaceDir()
{
	return fFaceDir;
}

void cPlayer::OneCycleAnimations(float fElapsedTime, olc::GFX2D::Transform2D* t, std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles)
{
	if (bAttacking && !bDead)
	{
		// calculate elapsed time during attack
		fAnimationTimer += fElapsedTime;

		// Slap Attack
		if (bSlapping)
		{
			if (fAnimationTimer >= cfSlapSpawnT * animPlayer->fTimeBetweenFrames)
			{
				if (bCanSpawnProjectile)
				{
					// must offset the AOE so it goes from kirbo's hand
					float fProjectilePosX = fPlayerPosX + (fFaceDir > 0.0f ? 1.0f : -(mapProjectiles["slapAOE"][0]->width / engine->GetTileWidth()));
					float fProjectilePosY = fPlayerPosY - ((mapProjectiles["slapAOE"][0]->height - engine->GetTileHeight()) / (2 * engine->GetTileHeight()));
					cDynamicProjectile* p = engine->CreateProjectile(fProjectilePosX, fProjectilePosY, true, fFaceDir, 0.0f, cfSlapDuration, "slapAOE", false, cnSlapDmg, false, false);
					p->bOneHit = false;
					engine->AddProjectile(p);
					bCanSpawnProjectile = false;
				}
			}
		}

		// Launch a Jesus Cross
		if (bLaunchingJesusCross)
		{
			if (fAnimationTimer >= cfJesusCrossSpawnT * animPlayer->fTimeBetweenFrames)
			{
				if (bCanSpawnProjectile)
				{
					cDynamicProjectile* p = engine->CreateProjectile((fPlayerPosX + fFaceDir), fPlayerPosY - 1.0f, true, cfJesusCrossVelX * fFaceDir, cfJesusCrossVelY, cfJesusCrossDuration, "jesuscross", true, cnJesusCrossDmg, true);
					engine->AddProjectile(p);
					bCanSpawnProjectile = false;
				}
			}
		}

		// Vacuuming
		if (bVacuuming)
		{
			if (fAnimationTimer >= cfVacuumAnimT * animPlayer->fTimeBetweenFrames)
			{
				animPlayer->ChangeState("vacuum");
			}
		}

		// Swallowing
		if (bSwallowing)
		{
			animPlayer->ChangeState("swallow");
		}

		// Stop the attack when it's finished
		if (fAnimationTimer >= animPlayer->mapStates[animPlayer->sCurrentState].size() * animPlayer->fTimeBetweenFrames)
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

		if (fAnimationTimer >= animPlayer->mapStates["damaged"].size() * animPlayer->fTimeBetweenFrames)
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
			t->Rotate(fDeadAnimation * cfDeadRotationAnimation);
			// animation based on a 2nd degree polynome to simulate kirbo's death animation
			t->Translate(0.0f, (4.0f * fDeadAnimation - 2.0f) * 64.0f * (4.0f * fDeadAnimation - 2.0f) - 4 * 64.0f);
		}

		// Return to the map after dead animation
		if (fDeadAnimation >= cfDeadAnimation)
		{
			// if you die in boss level, you reappear in the boss room
			if (engine->IsInBossLevel())
			{
				engine->ResetVariables();
				fHealth = cfMaxHealth;
				engine->SetGameState("GS_LOADBOSSLEVEL");
				engine->BreakLoop();
				return;
			}

			engine->SetGameState("GS_WORLDMAP");
			animPlayer->ChangeState("riding_star");
			engine->BreakLoop();
			return;
		}
	}

	if (engine->IsBossKilled())
	{
		fPlayerVelX = 0.0f;
		fPlayerVelY = 0.0f;
		StopAnyAttack();

		engine->UpdateWinTimer(fElapsedTime);

		// after the dance, kirbo goes away
		if (engine->GetWinTimer() >= animPlayer->mapStates["boss_killed"].size() * animPlayer->fTimeBetweenFrames)
		{
			fKirboGoesAwayTimer += fElapsedTime;
			animPlayer->ChangeState("kirbo_goes_away");

			fFaceDir = 1.0f;
			t->Scale(-1.0f, 1.0f); // Scale the sprite because riding_star00 sprite is facing left
			t->Rotate(-fKirboGoesAwayTimer * cfGoAwayRotationAnimation);
			t->Translate(fKirboGoesAwayTimer * cfGoAwayTranslationAnimation, -fKirboGoesAwayTimer * cfGoAwayTranslationAnimation);
		}
		else
		{
			fFaceDir = 1.0f;
		}
	}
}

void cPlayer::StopAnyAttack()
{
	bAttacking = false;
	bSlapping = false;
	bLaunchingJesusCross = false;
	bSwallowing = false;
}

void cPlayer::ClampVelocities()
{
	if (fPlayerVelX > cfMaxPlayerVelX)  fPlayerVelX = cfMaxPlayerVelX;
	if (fPlayerVelX < -cfMaxPlayerVelX) fPlayerVelX = -cfMaxPlayerVelX;
	if (fPlayerVelY > cfMaxPlayerVelY)  fPlayerVelY = cfMaxPlayerVelY;
	if (fPlayerVelY < -cfMaxPlayerVelY) fPlayerVelY = -cfMaxPlayerVelY;

	if (bFlying)
	{
		if (fPlayerVelX > cfMaxPlayerFlyingVelX)  fPlayerVelX = cfMaxPlayerFlyingVelX;
		if (fPlayerVelX < -cfMaxPlayerFlyingVelX) fPlayerVelX = -cfMaxPlayerFlyingVelX;
		if (fPlayerVelY > cfMaxPlayerFlyingVelY)  fPlayerVelY = cfMaxPlayerFlyingVelY;
	}
}

void cPlayer::IncreaseVelocities(float dvx, float dvy)
{
	fPlayerVelX += dvx;
	fPlayerVelY += dvy;
}

void cPlayer::DecreaseVelocities(float dvx, float dvy)
{
	fPlayerVelX -= dvx;
	fPlayerVelY -= dvy;
}

void cPlayer::Collisions(float fElapsedTime, cLevel* lvl)
{
	float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
	float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

	// Check for pickups !
	if (engine->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'o')
		engine->SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');

	if (engine->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == L'o')
		engine->SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');

	if (engine->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'o')
		engine->SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');

	if (engine->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == L'o')
		engine->SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.');

	// Check hole
	if (fPlayerPosY > lvl->GetHeight())
	{
		fHealth = 0.0f;
		bDead = true;
		animPlayer->ChangeState("dead");
	}

	// Collision
	if (fPlayerVelX <= 0) // Moving Left
	{
		if (fNewPlayerPosX <= 1) fNewPlayerPosX = 1; // Prevent from being brutally moved to 0 only when reaching -1

		if (engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f)) ||
			engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + fPlayerCollisionUpperLimit)))
		{
			fNewPlayerPosX = (int)fNewPlayerPosX + 1;
			fPlayerVelX = 0;
		}
	}
	else // Moving Right
	{
		if (fNewPlayerPosX >= lvl->GetWidth() - 2) fNewPlayerPosX = lvl->GetWidth() - 2; // Kirbo can't cross the edge of the map

		if (engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f)) ||
			engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + fPlayerCollisionUpperLimit)))
		{
			fNewPlayerPosX = (int)fNewPlayerPosX;
			fPlayerVelX = 0;
		}
	}

	bPlayerOnGround = false;
	if (fPlayerVelY <= 0) // Moving Up
	{
		if (fNewPlayerPosY <= 1) fNewPlayerPosY = 1; // Prevent from being brutally moved to 0 only when reaching -1

		if (engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY)) ||
			engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY)))
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
		if (engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) ||
			engine->IsSolidTile(engine->GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f)) ||
			((engine->IsSemiSolidTile(engine->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) ||
			  engine->IsSemiSolidTile(engine->GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f))) && fPlayerPosY + 1.0f < (float)((int)fNewPlayerPosY + 1.0f) + 0.1f))
		{
			fNewPlayerPosY = (int)fNewPlayerPosY + engine->GetGrdDynamicOverlay(); // Remove this line to create shifting sand
			fPlayerVelY = 0;
			bPlayerOnGround = true;
			bDoubleJump = true;
		}
	}

	fPlayerPosX = fNewPlayerPosX;
	fPlayerPosY = fNewPlayerPosY;
}

float cPlayer::GetPlayerPosX()
{
	return fPlayerPosX;
}

float cPlayer::GetPlayerPosY()
{
	return fPlayerPosY;
}

void cPlayer::SetPlayerPosX(float px)
{
	fPlayerPosX = px;
}

void cPlayer::SetPlayerPosY(float py)
{
	fPlayerPosY = py;
}

bool cPlayer::IsVacuuming()
{
	return bVacuuming;
}

void cPlayer::VacuumEnnemy(cDynamicCreature* object)
{
	// if one ennemy is under fSwallowDistance from kirbo, every swallowable ennemy is killed and kirbo starts swallowing animation
	float fTargetX = GetPlayerPosX() - object->px;
	float fTargetY = GetPlayerPosY() - object->py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	if (fDistance <= fSwallowDistance)
	{
		bSwallowing = true;
		bAttacking = true;
	}
}

bool cPlayer::IsAttackable()
{
	return bIsPlayerAttackable;
}

bool cPlayer::IsSwallowing()
{
	return bSwallowing;
}

void cPlayer::UpdateInvulnerability(float fElapsedTime)
{
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
}

void cPlayer::DrawKirbo(olc::PixelGameEngine* gfx, olc::GFX2D::Transform2D t)
{
	if (bShowKirbo)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		animPlayer->DrawSelf(gfx, t);
		gfx->SetPixelMode(olc::Pixel::NORMAL);
	}
}

float cPlayer::GetHealth()
{
	return fHealth;
}

void cPlayer::Damage(cDynamic* object)
{
	animPlayer->ChangeState("damaged");
	fInvulnerabilityTimer = cfInvulnerabilityFrame;
	bPlayerDamaged = true;
	bIsPlayerAttackable = false;
	fHealth -= object->nDamage;

	if (fHealth <= 0.0f)
	{
		bDead = true;
		animPlayer->ChangeState("dead");
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

void cPlayer::ResetVariables()
{
	fPlayerVelX = 0.0f;
	fPlayerVelY = 0.0f;
	fInvulnerabilityTimer = 0.0f;
	bDead = false;
	fDeadAnimation = 0.0f;
	bPlayerDamaged = false;
	fKirboGoesAwayTimer = 0.0f;
	fJumpTimer = 0.0f;
	StopAnyAttack();
}
