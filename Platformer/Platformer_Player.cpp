#include "Platformer_Player.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cPlayer::engine = nullptr;

cPlayer::cPlayer(cAnimator* animator)
{
	animPlayer = animator;
	hitbox = new cHitbox();
	vacuumHitbox = new cHitbox();
}

void cPlayer::InitialiseKirboHealth()
{
	fHealth = cfMaxHealth;
}

bool cPlayer::IsDead()
{
	return bDead;
}

void cPlayer::HandleInput(float fElapsedTime, cCamera* camera, cLevel* lvl)
{
	if (engine->IsFocused() && CanInteract())
	{
		// Fly, enter a door
		if (engine->GetKey(olc::Key::UP).bHeld || engine->GetController()->GetButton(UP).bHeld || engine->GetController()->GetLeftStickY() > 0.5f)
		{
			if (!bAttacking && !bVacuuming)
			{
				if (lvl->GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f) == L'w' && bOnGround)
					engine->SetGameState("GS_LOADBOSSLEVEL");

				fPlayerVelY = -cfPlayerVelY;
				bFlying = true;
				animPlayer->ChangeState("flying");
			}
		}

		// Go down when flying, cross semi solid platform and control camera when onground
		if (engine->GetKey(olc::Key::DOWN).bHeld || engine->GetController()->GetButton(DOWN).bHeld || engine->GetController()->GetLeftStickY() < -0.5f)
		{
			if (bFlying)
				fPlayerVelY = cfPlayerVelY;

			// If player is on semi solid platform, pass through the platform. cheat a little bit, modify the position of the player to cross it
			if ((engine->IsSemiSolidTile(lvl->GetTile(fPlayerPosX + 0.0f, fPlayerPosY + 1.0f)) ||
				 engine->IsSemiSolidTile(lvl->GetTile(fPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f))) && bOnGround)
			{
				fPlayerPosY += 0.15;
			}

			// If player is on Solid Ground, lower the camera
			if ((engine->IsSolidTile(lvl->GetTile(fPlayerPosX + 0.0f, fPlayerPosY + 1.0f)) ||
				 engine->IsSolidTile(lvl->GetTile(fPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f))) && bOnGround)
			{
				camera->LowerCameraPosition();
			}

			// Moving platforms collision
			for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
			{
				if (ptfm->TopCollision(fPlayerPosX + fPlayerCollisionLowerLimit, fPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f))
				{
					camera->LowerCameraPosition();
				}
			}
		}
		else
		{
			camera->RaiseCameraPosition();
		}

		// Go left
		if (engine->GetKey(olc::Key::LEFT).bHeld || engine->GetController()->GetButton(LEFT).bHeld || engine->GetController()->GetLeftStickX() < -0.25f)
		{
			if (!bAttacking && !bVacuuming)
			{
				// Init walking sound
				engine->PlaySample("kirboWalk", false, true);

				// Init speed by cfMinPlayerVelX + 0.05 or the player won't move when on ground
				if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX -= (cfMinPlayerVelX + 0.05f);

				fPlayerVelX += (bOnGround ? -cfPlayerAccGrdX : -cfPlayerAccAirX) * fElapsedTime;
				fFaceDir = -1.0f;
			}
		}

		// Go right
		if (engine->GetKey(olc::Key::RIGHT).bHeld || engine->GetController()->GetButton(RIGHT).bHeld || engine->GetController()->GetLeftStickX() > 0.25f)
		{
			if (!bAttacking && !bVacuuming)
			{
				// Init walking sound
				engine->PlaySample("kirboWalk", false, true);

				// Init speed by cfMinPlayerVelX + 0.05 or the player won't move when on ground
				if (fabs(fPlayerVelX) < cfMinPlayerVelX) fPlayerVelX += (cfMinPlayerVelX + 0.05f);

				fPlayerVelX += (bOnGround ? cfPlayerAccGrdX : cfPlayerAccAirX) * fElapsedTime;
				fFaceDir = 1.0f;
			}
		}

		// Stop kirbo walk sound
		if ((!engine->GetKey(olc::Key::LEFT).bHeld &&
			 !engine->GetKey(olc::Key::RIGHT).bHeld &&
			 engine->GetController()->GetLeftStickX() == 0.0f) || !bOnGround)
		{
			engine->StopSample("kirboWalk");
		}

		// Jump, double jump, stop flying
		if (engine->GetKey(olc::Key::SPACE).bPressed || engine->GetController()->GetButton(A).bPressed)
		{
			if (bFlying)
			{
				bFlying = false;
			}
			else if (bOnGround)
			{
				engine->PlaySample("kirboJump");
				bChargeJump = true;
			}
			else if (bDoubleJump && fPlayerVelY > 0)
			{
				engine->PlaySample("kirboJump");
				bDoubleJump = false;
				bChargeDoubleJump = true;
			}
		}

		// The more you hold, the higher you go
		if (engine->GetKey(olc::Key::SPACE).bHeld || engine->GetController()->GetButton(A).bHeld)
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
		if (engine->GetKey(olc::Key::SPACE).bReleased || engine->GetController()->GetButton(A).bReleased)
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
		if (engine->GetKey(olc::Key::F).bPressed || engine->GetController()->GetButton(B).bPressed)
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
		if (engine->GetKey(olc::Key::P).bPressed || engine->GetController()->GetButton(START).bPressed)
		{
			engine->SetPlayerChoice(0);

			// You can't use pause when you are hit.
			// like you can't pause when you jump in mario sunshine so you can't leave the level when you are falling
			engine->SetGameState("GS_PAUSE");
			engine->BreakLoop();
			return;
		}

		// Launch a Jesus Cross
		if (engine->GetKey(olc::Key::R).bPressed || engine->GetController()->GetButton(Y).bPressed)
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
		if (engine->GetKey(olc::Key::E).bHeld || engine->GetController()->GetButton(X).bHeld)
		{
			// can't Vacuum when player is attacking, swallowing or flying
			if (!bFlying && !bSwallowing)
			{
				if (!bVacuuming && !bAttacking)
				{
					engine->PlaySample("beginVacuum");
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
			if (bVacuuming)
			{
				engine->StopSample("beginVacuum");
				engine->StopSample("vacuum");
			}
			bVacuuming = false;
		}

		// Poyo
		if (engine->GetKey(olc::Key::O).bPressed || engine->GetController()->GetRightTrigger())
		{
			// Can only poyo on ground
			if (bOnGround)
			{
				animPlayer->nCurrentFrame = 0;

				int poyo = rand() % 2;
				if (poyo == 0) engine->PlaySample("poyo01");
				if (poyo == 1) engine->PlaySample("poyo02");

				animPlayer->ChangeState("poyo");
				bAttacking = true;
				bPoyo = true;
				fAnimationTimer = 0.0f;
			}
		}
	}
}

bool cPlayer::CanInteract()
{
	return !bPlayerDamaged && !bDead && !bIsGrabbedByEnnemy && !engine->IsBossKilled();
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
	}
	else
	{
		fPlayerVelX += engine->GetDragValue() * fPlayerVelX * fElapsedTime;
		if (bOnGround)
		{
			bFlying = false;
			engine->StopSample("kirboFly");

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
					engine->StopSample("kirboFly");

					if (fPlayerVelY < 0)
						animPlayer->ChangeState("jump");
					else
						animPlayer->ChangeState("fall");
				}
				else
				{
					engine->PlaySample("kirboFly", false, true);
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
					engine->AddProjectile(fProjectilePosX, fProjectilePosY, true, fFaceDir, 0.0f, cfSlapDuration, "slapAOE", false, cnSlapDmg * nDamageBooster, false, false, 0, true, -3.0f, "slap");
					bCanSpawnProjectile = false;
				}
			}
		}

		// Poyo
		if (bPoyo)
		{
			// Poyo
		}

		// Launch a Jesus Cross
		if (bLaunchingJesusCross)
		{
			if (fAnimationTimer >= cfJesusCrossSpawnT * animPlayer->fTimeBetweenFrames)
			{
				if (bCanSpawnProjectile)
				{
					engine->AddProjectile((fPlayerPosX + fFaceDir), fPlayerPosY - 1.0f, true, cfJesusCrossVelX * fFaceDir, cfJesusCrossVelY, cfJesusCrossDuration, "jesuscross", true, cnJesusCrossDmg * nDamageBooster, true);
					bCanSpawnProjectile = false;
				}
			}
		}

		// Vacuuming
		if (bVacuuming)
		{
			if (fAnimationTimer >= cfVacuumAnimT * animPlayer->fTimeBetweenFrames)
			{
				if (!engine->IsSamplePlaying("beginVacuum"))
					engine->PlaySample("vacuum", false, true);
				animPlayer->ChangeState("vacuum");
			}
		}

		// Swallowing
		if (bSwallowing)
		{
			engine->StopSample("beginVacuum");
			engine->StopSample("vacuum");

			if (bSwallowSound)
			{
				bSwallowSound = false;
				engine->PlaySample("swallow");
			}

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

		if (fAnimationTimer >= animPlayer->mapStates[animPlayer->sCurrentState].size() * animPlayer->fTimeBetweenFrames)
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

		if (fDeadAnimation == 0.0f)
			engine->PlaySample("loseLife");

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

			engine->ReturnToWorldMap();
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
	bPoyo = false;
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

void cPlayer::SetVelocities(float vx, float vy)
{
	fPlayerVelX = vx;
	fPlayerVelY = vy;
}

void cPlayer::Collisions(float fElapsedTime, cLevel* lvl)
{
	float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
	float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

	// Check for pickups !
	if (IsCollectibleItem(lvl->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f)))
	{
		SelectItem(lvl->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f));
		lvl->SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');
	}
	if (IsCollectibleItem(lvl->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)))
	{
		SelectItem(lvl->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f));
		lvl->SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');
	}
	if (IsCollectibleItem(lvl->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f)))
	{
		SelectItem(lvl->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f));
		lvl->SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');
	}
	if (IsCollectibleItem(lvl->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f)))
	{
		SelectItem(lvl->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f));
		lvl->SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.');
	}

	// Check hole
	if (fPlayerPosY > lvl->GetHeight())
	{
		if (!bDead)
			engine->PlaySample("kirboHit");
		Kill();
	}

	// Collision
	if (fPlayerVelX <= 0) // Moving Left
	{
		if (fNewPlayerPosX <= 1) fNewPlayerPosX = 1; // Prevent from being brutally moved to 0 only when reaching -1

		CheckLeftWall(lvl, fNewPlayerPosX);
		CheckRightWall(lvl, fNewPlayerPosX);
	}
	else // Moving Right
	{
		if (fNewPlayerPosX >= lvl->GetWidth() - 2) fNewPlayerPosX = lvl->GetWidth() - 2; // Kirbo can't cross the edge of the map

		CheckLeftWall(lvl, fNewPlayerPosX);
		CheckRightWall(lvl, fNewPlayerPosX);
	}

	bOnGround = false;
	if (fPlayerVelY <= 0) // Moving Up
	{
		if (fNewPlayerPosY <= 1) fNewPlayerPosY = 1; // Prevent from being brutally moved to 0 only when reaching -1

		CheckDynamicCeiling(fNewPlayerPosX, fNewPlayerPosY, lvl);
		CheckDynamicFloor(fNewPlayerPosX, fNewPlayerPosY, fElapsedTime, lvl);

		CheckSolidCeiling(lvl, fNewPlayerPosX, fNewPlayerPosY);
	}
	else // Moving Down
	{
		CheckDynamicCeiling(fNewPlayerPosX, fNewPlayerPosY, lvl);
		CheckDynamicFloor(fNewPlayerPosX, fNewPlayerPosY, fElapsedTime, lvl);

		CheckSolidFloor(lvl, fNewPlayerPosX, fNewPlayerPosY);
	}

	fPlayerPosX = fNewPlayerPosX;
	fPlayerPosY = fNewPlayerPosY;
}

void cPlayer::CheckSolidFloor(cLevel* lvl, float fNewPlayerPosX, float& fNewPlayerPosY)
{
	if (SolidFloor(lvl, fNewPlayerPosX, fNewPlayerPosY) || SemiSolidFloor(lvl, fNewPlayerPosX, fNewPlayerPosY))
	{
		if (DynamicCeiling(fNewPlayerPosX, fNewPlayerPosY) && fCrushingObjVY > 0.0f)
		{
			Crushed();
		}

		fNewPlayerPosY = (int)fNewPlayerPosY + engine->GetGroundDynamicOverlay(); // Remove this line to create shifting sand
		fPlayerVelY = 0;
		bOnGround = true;
		bDoubleJump = true;
	}
}

void cPlayer::CheckDynamicFloor(float& fNewPlayerPosX, float& fNewPlayerPosY, float fElapsedTime, cLevel* lvl)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->TopCollision(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f) ||
			ptfm->TopCollisionWithLag(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f, fNewPlayerPosY + 1.0f))
		{
			HarmfulBloc(ptfm);

			if (ptfm->GetVY() <= fPlayerVelY)
			{
				fNewPlayerPosY = ptfm->GetPY() - 1.0f;
				fNewPlayerPosX += ptfm->GetVX() * fElapsedTime;
			}

			// Check if a wall is there
			if (fNewPlayerPosX < fPlayerPosX)
			{
				CheckLeftWall(lvl, fNewPlayerPosX);
			}
			if (fNewPlayerPosX > fPlayerPosX)
			{
				CheckRightWall(lvl, fNewPlayerPosX);
			}

			if ((Ceiling(lvl, fNewPlayerPosX, fNewPlayerPosY) && ptfm->GetVY() < 0.0f) || (DynamicCeiling(fNewPlayerPosX, fNewPlayerPosY) && CeilingFloorCrushed(ptfm)))
			{
				Crushed();
			}

			fPlayerVelY = 0;
			bOnGround = true;
			bDoubleJump = true;
			ptfm->TriggerMovement();
		}
	}
}

bool cPlayer::CeilingFloorCrushed(cDynamicMovingPlatform*& ptfm)
{
	return fCrushingObjVY > 0.0f && ptfm->GetVY() < 0.0f ||
		fCrushingObjVY > 0.0f && ptfm->GetVY() > 0.0f && fCrushingObjVY > ptfm->GetVY() ||
		fCrushingObjVY < 0.0f && ptfm->GetVY() < 0.0f && fCrushingObjVY > ptfm->GetVY();
}

void cPlayer::CheckSolidCeiling(cLevel* lvl, float fNewPlayerPosX, float& fNewPlayerPosY)
{
	if (Ceiling(lvl, fNewPlayerPosX, fNewPlayerPosY))
	{
		fNewPlayerPosY = (int)fNewPlayerPosY + 1;
		fPlayerVelY = 0;

		for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
		{
			if (ptfm->GetVY() < 0 &&
				(ptfm->TopCollision(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f) ||
				 ptfm->TopCollisionWithLag(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f, fNewPlayerPosY + 1.0f)))
			{
				Crushed();
			}
		}
	}
}

void cPlayer::CheckDynamicCeiling(float fNewPlayerPosX, float& fNewPlayerPosY, cLevel* lvl)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->BotCollision(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY) ||
			ptfm->BotCollisionWithLag(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY, fNewPlayerPosY))
		{
			HarmfulBloc(ptfm);

			if (ptfm->GetVY() >= fPlayerVelY)
				fNewPlayerPosY = ptfm->GetPY() + ptfm->GetNormalizedHeight();

			if (SolidFloor(lvl, fNewPlayerPosX, fNewPlayerPosY) && ptfm->GetVY() > 0.0f ||
				SemiSolidFloor(lvl, fNewPlayerPosX, fNewPlayerPosY) && ptfm->GetVY() > 0.0f ||
				DynamicFloor(fNewPlayerPosX, fNewPlayerPosY) && FloorCeilingCrushed(ptfm))
			{
				Crushed();
			}
		}
	}
}

bool cPlayer::FloorCeilingCrushed(cDynamicMovingPlatform*& ptfm)
{
	return fCrushingObjVY < 0.0f && ptfm->GetVY() > 0.0f ||
		fCrushingObjVY > 0.0f && ptfm->GetVY() > 0.0f && fCrushingObjVY < ptfm->GetVY() ||
		fCrushingObjVY < 0.0f && ptfm->GetVY() < 0.0f && fCrushingObjVY < ptfm->GetVY();
}

void cPlayer::Crushed()
{
	if (!bDead)
		engine->PlaySample("kirboHit");
	Kill();
}

bool cPlayer::DynamicFloor(float fNewPlayerPosX, float fNewPlayerPosY)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->TopCollision(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f) ||
			ptfm->TopCollisionWithLag(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY + 1.0f, fNewPlayerPosY + 1.0f))
		{
			fCrushingObjVY = ptfm->GetVY();
			return true;
		}
	}
	return false;
}

bool cPlayer::DynamicCeiling(float fNewPlayerPosX, float fNewPlayerPosY)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->BotCollision(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY) ||
			ptfm->BotCollisionWithLag(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosX + fPlayerCollisionUpperLimit, fPlayerPosY, fNewPlayerPosY))
		{
			fCrushingObjVY = ptfm->GetVY();
			return true;
		}
	}
	return false;
}

bool cPlayer::SemiSolidFloor(cLevel* lvl, float fNewPlayerPosX, float fNewPlayerPosY)
{
	// This little trick (fPlayerPosY + 1.0f < (float)((int)fNewPlayerPosY + 1.0f) + 0.1f) checks if the player's feets are above the top of the semi-solid Block.
	// Otherwise the player is moved to the top of the block when his feets reach the bottom of the block
	// "fPlayerPosY + 1.0f" is the feets Y position, "(float)((int)fNewPlayerPosY + 1.0f) + 0.1f" takes the top of the block at the feets position and add a 0.1 delta, 
	// if the feets are above this delta, the player is moved on top of the block.
	return (engine->IsSemiSolidTile(lvl->GetTile(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosY + 1.0f)) ||
			engine->IsSemiSolidTile(lvl->GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f))) && fPlayerPosY + 1.0f < (float)((int)fNewPlayerPosY + 1.0f) + 0.1f;
}

bool cPlayer::SolidFloor(cLevel* lvl, float fNewPlayerPosX, float fNewPlayerPosY)
{
	return engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + fPlayerCollisionLowerLimit, fNewPlayerPosY + 1.0f)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY + 1.0f));
}

bool cPlayer::Ceiling(cLevel* lvl, float fNewPlayerPosX, float fNewPlayerPosY)
{
	return engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + fPlayerCollisionUpperLimit, fNewPlayerPosY));
}

void cPlayer::CheckRightWall(cLevel* lvl, float& fNewPlayerPosX)
{
	if (engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + fPlayerCollisionUpperLimit)))
	{
		if (DynamicLeftWall(fNewPlayerPosX) && fCrushingObjVX > 0.0f)
		{
			Crushed();
		}

		fNewPlayerPosX = (int)fNewPlayerPosX;
		fPlayerVelX = 0;
	}

	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->LeftCollision(fPlayerPosY, fPlayerPosY + 1.0f, fNewPlayerPosX + 1.0f) ||
			ptfm->LeftCollisionWithLag(fPlayerPosY, fPlayerPosY + 1.0f, fPlayerPosX + 1.0f, fNewPlayerPosX + 1.0f))
		{
			if (DynamicLeftWall(fNewPlayerPosX) && LeftRightCrushed(ptfm))
			{
				Crushed();
			}

			HarmfulBloc(ptfm);

			if (ptfm->GetVX() <= fPlayerVelX)
			{
				fNewPlayerPosX = ptfm->GetPX() - 1.0f;
			}
		}
	}
}

bool cPlayer::LeftRightCrushed(cDynamicMovingPlatform*& ptfm)
{
	return fCrushingObjVX > 0.0f && ptfm->GetVX() < 0.0f ||
		fCrushingObjVX > 0.0f && ptfm->GetVX() > 0.0f && fCrushingObjVX > ptfm->GetVX() ||
		fCrushingObjVX < 0.0f && ptfm->GetVX() < 0.0f && fCrushingObjVX > ptfm->GetVX();
}

bool cPlayer::DynamicLeftWall(float fNewPlayerPosX)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->RightCollision(fPlayerPosY, fPlayerPosY + 1.0f, fNewPlayerPosX) ||
			ptfm->RightCollisionWithLag(fPlayerPosY, fPlayerPosY + 1.0f, fPlayerPosX, fNewPlayerPosX))
		{
			fCrushingObjVX = ptfm->GetVX();
			return true;
		}
	}
	return false;
}

void cPlayer::CheckLeftWall(cLevel* lvl, float& fNewPlayerPosX)
{
	if (engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + fPlayerCollisionUpperLimit)))
	{
		if (DynamicRightWall(fNewPlayerPosX) && fCrushingObjVX < 0.0f)
		{
			Crushed();
		}

		fNewPlayerPosX = (int)fNewPlayerPosX + 1;
		fPlayerVelX = 0;
	}

	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->RightCollision(fPlayerPosY, fPlayerPosY + 1.0f, fNewPlayerPosX) ||
			ptfm->RightCollisionWithLag(fPlayerPosY, fPlayerPosY + 1.0f, fPlayerPosX, fNewPlayerPosX))
		{
			if (DynamicRightWall(fNewPlayerPosX) && RightLeftCrushed(ptfm))
			{
				Crushed();
			}

			HarmfulBloc(ptfm);

			if (ptfm->GetVX() >= fPlayerVelX)
			{
				fNewPlayerPosX = ptfm->GetPX() + ptfm->GetNormalizedWidth();
			}
		}
	}
}

void cPlayer::HarmfulBloc(cDynamicMovingPlatform*& ptfm)
{
	if (!ptfm->IsFriendly() && bIsPlayerAttackable)
	{
		Damage(ptfm);
	}
}

bool cPlayer::RightLeftCrushed(cDynamicMovingPlatform*& ptfm)
{
	return fCrushingObjVX > 0.0f && ptfm->GetVX() < 0.0f ||
		fCrushingObjVX > 0.0f && ptfm->GetVX() > 0.0f && fCrushingObjVX < ptfm->GetVX() ||
		fCrushingObjVX < 0.0f && ptfm->GetVX() < 0.0f && fCrushingObjVX < ptfm->GetVX();
}

bool cPlayer::DynamicRightWall(float fNewPlayerPosX)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPlayerPosX, fPlayerPosY))
	{
		if (ptfm->LeftCollision(fPlayerPosY, fPlayerPosY + 1.0f, fNewPlayerPosX + 1.0f) ||
			ptfm->LeftCollisionWithLag(fPlayerPosY, fPlayerPosY + 1.0f, fPlayerPosX + 1.0f, fNewPlayerPosX + 1.0f))
		{
			fCrushingObjVX = ptfm->GetVX();
			return true;
		}
	}
	return false;
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
	float fTargetX = GetPlayerPosX() - object->GetPX();
	float fTargetY = GetPlayerPosY() - object->GetPY();
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	if (fDistance <= fSwallowDistance)
	{
		bSwallowing = true;
		bSwallowSound = true;
		bAttacking = true;
	}
}

bool cPlayer::IsAttackable()
{
	return bIsPlayerAttackable;
}

bool cPlayer::HasCandyPower()
{
	return bHasCandyPower;
}

void cPlayer::SetAttackable(bool attackable)
{
	bIsPlayerAttackable = attackable;
	bForceInvincible = !attackable;
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
		if (bHasCandyPower)
		{
			bHasCandyPower = false;
			SetAttackable(true);
			engine->PlayLevelMusic();
		}

		fInvulnerabilityTimer = 0.0f;

		// For King DeeDeeDee vacuum attack
		if (!bForceInvisible)
		{
			bShowKirbo = true;
		}
		if (!bForceInvincible)
		{
			bIsPlayerAttackable = true;
		}
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

void cPlayer::DrawKirbo(olc::GFX2D::Transform2D t)
{
	if (bShowKirbo)
	{
		engine->SetPixelMode(olc::Pixel::ALPHA);
		animPlayer->DrawSelf(engine, t);
		engine->SetPixelMode(olc::Pixel::NORMAL);
	}
}

float cPlayer::GetHealth()
{
	return fHealth;
}

void cPlayer::Damage(cDynamic* object)
{
	engine->PlaySample("kirboHit");
	animPlayer->ChangeState("damaged");
	fInvulnerabilityTimer = cfInvulnerabilityFrame;
	bPlayerDamaged = true;
	bIsPlayerAttackable = false;
	fHealth -= round((float)(object->GetDamage() / (float)nDefenseBooster)); // Kirbo can't take 0 damage with defense boost

	if (fHealth <= 0.0f)
	{
		Kill();
	}

	if (!bDead)
	{
		// Knockback the player out of the ennemy
		if (object->GetPX() < fPlayerPosX)
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

void cPlayer::Kill()
{
	fHealth = 0.0f;
	bDead = true;
	animPlayer->ChangeState("dead");
	nDamageBooster = 1;
	nDefenseBooster = 1;
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
	bIsGrabbedByEnnemy = false;
	StopAnyAttack();
}

void cPlayer::VacuumHitbox(float cameraOffsetX, float cameraOffsetY)
{
	vacuumHitbox->SetPos(
		(fPlayerPosX + (fFaceDir > 0.0f ? 1.75f : -0.75f) - cameraOffsetX) * engine->GetTileWidth(),
		(fPlayerPosY + 0.5f - cameraOffsetY) * engine->GetTileHeight()
	); // 1 block ahead the player's looking direction
	vacuumHitbox->SetAngle(0.0f);
	vacuumHitbox->AddPoint(-engine->GetTileWidth() * 1.25f, -engine->GetTileHeight() / (fFaceDir > 0.0f ? 2.0f : 1.0f));
	vacuumHitbox->AddPoint(-engine->GetTileWidth() * 1.25f, +engine->GetTileHeight() / (fFaceDir > 0.0f ? 2.0f : 1.0f));
	vacuumHitbox->AddPoint(+engine->GetTileWidth() * 1.25f, +engine->GetTileHeight() / (fFaceDir > 0.0f ? 1.0f : 2.0f));
	vacuumHitbox->AddPoint(+engine->GetTileWidth() * 1.25f, -engine->GetTileHeight() / (fFaceDir > 0.0f ? 1.0f : 2.0f));
	vacuumHitbox->ResizeP(4);

	for (int i = 0; i < vacuumHitbox->GetOSize(); i++)
	{
		// 2D Rotation Transform + 2D Translation (angle is always 0 here, no rotation allowed)
		vacuumHitbox->SetP(i,
						   (vacuumHitbox->GetOIX(i) * cosf(vacuumHitbox->GetAngle())) - (vacuumHitbox->GetOIY(i) * sinf(vacuumHitbox->GetAngle())) + vacuumHitbox->GetPosX(),
						   (vacuumHitbox->GetOIX(i) * sinf(vacuumHitbox->GetAngle())) + (vacuumHitbox->GetOIY(i) * cosf(vacuumHitbox->GetAngle())) + vacuumHitbox->GetPosY()
		);
	}

	vacuumHitbox->ClearO();

	// debug AOE
	//vacuumHitbox->Draw(engine, olc::GREEN);
}

cHitbox* cPlayer::GetVacuumHitbox()
{
	return vacuumHitbox;
}

void cPlayer::UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
{
	hitbox->SetPos(
		(fPlayerPosX + 0.5f - cameraOffsetX) * engine->GetTileWidth(),
		(fPlayerPosY + 0.5f - cameraOffsetY) * engine->GetTileHeight()
	); // Center of the player
	hitbox->SetAngle(0.0f);
	hitbox->AddPoint(-engine->GetTileWidth() / 2.2f, -engine->GetTileHeight() / 2.2f);	// little reduction of the player hitbox to allow a little overlap with attack
	hitbox->AddPoint(-engine->GetTileWidth() / 2.2f, +engine->GetTileHeight() / 2.2f);	// little reduction of the player hitbox to allow a little overlap with attack
	hitbox->AddPoint(+engine->GetTileWidth() / 2.2f, +engine->GetTileHeight() / 2.2f);	// little reduction of the player hitbox to allow a little overlap with attack
	hitbox->AddPoint(+engine->GetTileWidth() / 2.2f, -engine->GetTileHeight() / 2.2f);	// little reduction of the player hitbox to allow a little overlap with attack
	hitbox->ResizeP(4);

	for (int i = 0; i < hitbox->GetOSize(); i++)
	{
		// 2D Rotation Transform + 2D Translation (angle is always 0 here, no rotation allowed)
		hitbox->SetP(i,
					 (hitbox->GetOIX(i) * cosf(hitbox->GetAngle())) - (hitbox->GetOIY(i) * sinf(hitbox->GetAngle())) + hitbox->GetPosX(),
					 (hitbox->GetOIX(i) * sinf(hitbox->GetAngle())) + (hitbox->GetOIY(i) * cosf(hitbox->GetAngle())) + hitbox->GetPosY()
		);
	}

	hitbox->ClearO();

	// Debug AOE
	//hitbox->Draw(engine, olc::BLUE);
}

cHitbox* cPlayer::GetHitbox()
{
	return hitbox;
}

void cPlayer::Attack(cDynamicCreature* victim, int damage)
{
	if (victim != nullptr)
	{
		// Restart the animation at begining when hit
		victim->SetGraphicCounter(0);

		// Attack victim with damage
		victim->TakeDamage(damage);

		// Knock victim back
		float tx = victim->GetPX() - fPlayerPosX;
		float ty = victim->GetPY() - fPlayerPosY;
		float d = sqrtf(tx * tx + ty * ty);
		if (d < 1) d = 1.0f;

		// After a hit, the object experiences knock back, where it is temporarily
		// under system control. This delivers two functions, the first being
		// a visual indicator to the player that something has happened, and the second
		// it stops the ability to spam attacks on a single creature
		if (victim->IsKnockable())
			victim->KnockBack(tx / d, ty / d, cfKnockBackDuration);
		else
			victim->KnockBack(0.0f, 0.0f, cfKnockBackDuration);
	}
}

void cPlayer::Vacuum(cDynamicCreature* object, float cameraOffsetX, float cameraOffsetY)
{
	object->UpdateHitbox(cameraOffsetX, cameraOffsetY);
	VacuumHitbox(cameraOffsetX, cameraOffsetY);

	if (cHitbox::ShapeOverlap_DIAG(object->GetHitbox(), GetVacuumHitbox()))
	{
		object->Vacuumed(true);
		Attack(object, 0);

		VacuumEnnemy(object);
	}
	else
	{
		object->Vacuumed(false);
		object->SetSwallowable(false);
	}
}

void cPlayer::CheckKirboCollisionWithEnemy(cDynamic* object, float cameraOffsetX, float cameraOffsetY)
{
	object->UpdateHitbox(cameraOffsetX, cameraOffsetY);
	UpdateHitbox(cameraOffsetX, cameraOffsetY);

	if (cHitbox::ShapeOverlap_DIAG(object->GetHitbox(), GetHitbox()))
	{
		if (!HasCandyPower())
			Damage(object);
		else
			Attack((cDynamicCreature*)object, GetCandyDmg());
	}
}

bool cPlayer::CheckIfEnnemyCollision(cDynamic* object, float cameraOffsetX, float cameraOffsetY)
{
	object->UpdateHitbox(cameraOffsetX, cameraOffsetY);
	UpdateHitbox(cameraOffsetX, cameraOffsetY);

	return cHitbox::ShapeOverlap_DIAG(object->GetHitbox(), GetHitbox());
}

void cPlayer::SetGrabbedByEnnemy(bool grabbed)
{
	bIsGrabbedByEnnemy = grabbed;
}

void cPlayer::ChangeAnimation(std::string animation)
{
	animPlayer->ChangeState(animation);
}

void cPlayer::SetVisible(bool visible)
{
	bShowKirbo = visible;
	bForceInvisible = !visible;
}

bool cPlayer::IsCollectibleItem(wchar_t c)
{
	return c == L't' || c == L'c' || c == L's' || c == L'd';
}

void cPlayer::SelectItem(wchar_t item)
{
	cItem* pickedItem;

	switch (item)
	{
		case L't': pickedItem = new cItemTomato();	break;
		case L'c': pickedItem = new cItemCandy();	break;
		case L's': pickedItem = new cItemDamage();  break;
		case L'd': pickedItem = new cItemDefense(); break;
		default:   pickedItem = new cItem();		break;
	}

	pickedItem->PickItem();
}

void cPlayer::Heal()
{
	fHealth = cfMaxHealth;
}

void cPlayer::SetInvincible(float time)
{
	fInvulnerabilityTimer = time;
	SetAttackable(false);
}

void cPlayer::SetCandyPower(bool candy)
{
	bHasCandyPower = candy;
}

int cPlayer::GetCandyDmg()
{
	return cnCandyPowerDmg;
}

void cPlayer::SetDamageBooster(int boost)
{
	nDamageBooster = boost;
}

bool cPlayer::HasDamageBooster()
{
	return nDamageBooster > 1;
}

void cPlayer::SetDefenseBooster(int boost)
{
	nDefenseBooster = boost;
}

bool cPlayer::HasDefenseBooster()
{
	return nDefenseBooster > 1;
}
