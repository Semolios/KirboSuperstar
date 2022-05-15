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
		if (engine->GetKey(engine->GetSavedControls("flyOrEnterDoor")).bHeld || engine->GetController()->GetButton(UP).bHeld || engine->GetController()->GetLeftStickY() > 0.5f)
		{
			bUsingFlyCmd = true;
			if (!bInteracting && !bVacuuming)
			{
				if (IsEnteringDoor(lvl))
				{
					animPlayer->ChangeState("enterDoor");
					bInteracting = true;
					bFlying = false;
					bBreakDoor = true;
					fAnimationTimer = 0.0f;
					bCanSpawnProjectile = true;
					SetAttackable(false);
				}

				if (!bBreakDoor)
				{
					fVelY = -cfVelY;
					bFlying = true;
					animPlayer->ChangeState("flying");
				}
			}
		}
		else
		{
			bUsingFlyCmd = false;
		}

		// Go down when flying, cross semi solid platform and control camera when onground
		if (engine->GetKey(engine->GetSavedControls("down")).bHeld || engine->GetController()->GetButton(DOWN).bHeld || engine->GetController()->GetLeftStickY() < -0.5f)
		{
			if (bFlying)
				fVelY = cfVelY;

			// If player is on semi solid platform, pass through the platform. cheat a little bit, modify the position of the player to cross it
			if ((engine->IsSemiSolidTile(lvl->GetTile(fPosX + 0.0f, fPosY + 1.0f)) ||
				 engine->IsSemiSolidTile(lvl->GetTile(fPosX + fCollisionUpperLimit, fPosY + 1.0f))) && bOnGround)
			{
				fPosY += 0.15;
			}

			// If player is on Solid Ground, lower the camera
			if ((engine->IsSolidTile(lvl->GetTile(fPosX + 0.0f, fPosY + 1.0f)) ||
				 engine->IsSolidTile(lvl->GetTile(fPosX + fCollisionUpperLimit, fPosY + 1.0f))) && bOnGround)
			{
				camera->LowerPosition();
			}

			// Moving platforms collision
			for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
			{
				if (ptfm->TopCollision(fPosX + fCollisionLowerLimit, fPosX + fCollisionUpperLimit, fPosY + 1.0f))
				{
					camera->LowerPosition();
				}
			}
		}
		else
		{
			camera->RaisePosition();
		}

		// Go left
		if (engine->GetKey(engine->GetSavedControls("moveLeft")).bHeld || engine->GetController()->GetButton(LEFT).bHeld || engine->GetController()->GetLeftStickX() < -0.25f)
		{
			if (!bInteracting && !bVacuuming)
			{
				// Init walking sound
				engine->PlaySample("kirboWalk", false, true);

				// Init speed by cfMinPlayerVelX + 0.05 or the player won't move when on ground
				if (fabs(fVelX) < cfMinVelX) fVelX -= (cfMinVelX + 0.05f);

				fVelX += (bOnGround ? -fAccGrdX : -fAccAirX) * fElapsedTime;
				fFaceDir = -1.0f;
			}
		}

		// Go right
		if (engine->GetKey(engine->GetSavedControls("moveRight")).bHeld || engine->GetController()->GetButton(RIGHT).bHeld || engine->GetController()->GetLeftStickX() > 0.25f)
		{
			if (!bInteracting && !bVacuuming)
			{
				// Init walking sound
				engine->PlaySample("kirboWalk", false, true);

				// Init speed by cfMinPlayerVelX + 0.05 or the player won't move when on ground
				if (fabs(fVelX) < cfMinVelX) fVelX += (cfMinVelX + 0.05f);

				fVelX += (bOnGround ? fAccGrdX : fAccAirX) * fElapsedTime;
				fFaceDir = 1.0f;
			}
		}

		// Stop kirbo walk sound
		if ((!engine->GetKey(engine->GetSavedControls("moveLeft")).bHeld &&
			 !engine->GetKey(engine->GetSavedControls("moveRight")).bHeld &&
			 engine->GetController()->GetLeftStickX() == 0.0f) || !bOnGround)
		{
			engine->StopSample("kirboWalk");
		}

		// Jump, double jump, stop flying
		if (engine->GetKey(engine->GetSavedControls("jump")).bPressed || engine->GetController()->GetButton(A).bPressed)
		{
			if (bFlying)
			{
				bFlying = false;
			}
			else if (bOnGround)
			{
				Jump();
				bChargeJump = true;
			}
			else if (bDoubleJump && fVelY > 0)
			{
				Jump();
				bDoubleJump = false;
				bChargeDoubleJump = true;
			}
		}

		// The more you hold, the higher you go
		if (engine->GetKey(engine->GetSavedControls("jump")).bHeld || engine->GetController()->GetButton(A).bHeld)
		{
			if (!bInteracting && !bVacuuming)
			{
				if (bChargeJump)
				{
					fJumpTimer += fElapsedTime;
					fVelY = cfJumpAcceleration - (cfJumpDecelerationRatio * fJumpTimer * cfJumpAcceleration);
					if (fJumpTimer >= cfJumpTime)
					{
						fJumpTimer = 0.0f;
						bChargeJump = false;
					}
				}
				else if (bChargeDoubleJump)
				{
					fJumpTimer += fElapsedTime;
					fVelY = cfJumpAcceleration - (cfJumpDecelerationRatio * fJumpTimer * cfJumpAcceleration);
					if (fJumpTimer >= cfDblJumpTime)
					{
						fJumpTimer = 0.0f;
						bChargeDoubleJump = false;
					}
				}
			}
		}

		// if you release space, jump is cancelled so you can't spam space to glide
		if (engine->GetKey(engine->GetSavedControls("jump")).bReleased || engine->GetController()->GetButton(A).bReleased)
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
		if (engine->GetKey(engine->GetSavedControls("slapAttack")).bPressed || engine->GetController()->GetButton(B).bPressed)
		{
			// Can't spam slap, can't slap when player is flying
			if (!bInteracting && !bFlying)
			{
				animPlayer->ChangeState("slap");
				bInteracting = true;
				bSlapping = true;
				bCanSpawnProjectile = true;
				fAnimationTimer = 0.0f;
			}
		}

		// Pause
		if (engine->GetKey(engine->GetSavedControls("pause")).bPressed || engine->GetController()->GetButton(START).bPressed)
		{
			engine->SetPlayerChoice(0);

			// You can't use pause when you are hit.
			// like you can't pause when you jump in mario sunshine so you can't leave the level when you are falling
			engine->TransitionTo("GS_PAUSE", true);
			engine->BreakLoop();
			return;
		}

		// Launch a Jesus Cross
		if (engine->GetKey(engine->GetSavedControls("launchJesusCross")).bPressed || engine->GetController()->GetButton(Y).bPressed)
		{
			// Can't spam Launching cross, can't launch when player is flying
			if (!bInteracting && !bFlying)
			{
				animPlayer->ChangeState("jesus_christ");
				bInteracting = true;
				bLaunchingJesusCross = true;
				bCanSpawnProjectile = true;
				fAnimationTimer = 0.0f;
			}
		}

		// Vacuum attack
		if (engine->GetKey(engine->GetSavedControls("vacuum")).bHeld || engine->GetController()->GetButton(X).bHeld)
		{
			// can't Vacuum when player is attacking, swallowing or flying
			if (!bFlying && !bSwallowing)
			{
				if (!bVacuuming && !bInteracting)
				{
					engine->PlaySample("beginVacuum");
					animPlayer->ChangeState("begin_vacuum");
					bVacuuming = true;
					fAnimationTimer = 0.0f;
				}
				bInteracting = true;
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
		if (engine->GetKey(engine->GetSavedControls("poyo")).bPressed || engine->GetController()->GetButton(RIGHT_THUMB).bPressed)
		{
			// Can only poyo on ground
			if (bOnGround)
			{
				animPlayer->nCurrentFrame = 0;

				int poyo = rand() % 2;
				if (poyo == 0) engine->PlaySample("poyo01");
				if (poyo == 1) engine->PlaySample("poyo02");

				animPlayer->ChangeState("poyo");
				bInteracting = true;
				bPoyo = true;
				fAnimationTimer = 0.0f;
			}
		}
	}
}

void cPlayer::Jump()
{
	engine->PlaySample("kirboJump");
	engine->AddProjectile(fPosX, fPosY, true, 0.0f, 0.1f, cfFartDuration, "kirboFart", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
}

bool cPlayer::IsEnteringDoor(cLevel* lvl)
{
	if (lvl->GetTile(fPosX + 0.5f, fPosY + 0.5f) == L'w' && bOnGround)
		return true;

	for (auto& TP : engine->GetCloseTeleport(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetHitbox()))
			return true;
	}

	for (auto& TP : engine->GetCloseTeleportDest(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetDestHitbox()))
			return true;
	}

	return false;
}

void cPlayer::EnterDoor(cLevel* lvl)
{
	if (lvl->GetTile(fPosX + 0.5f, fPosY + 0.5f) == L'w')
	{
		SetAttackable(true);
		engine->SetGameState("GS_LOADBOSSLEVEL");
	}

	EnterTP();
}

void cPlayer::EnterTP()
{
	for (auto& TP : engine->GetCloseTeleport(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetHitbox()))
		{
			Teleport(TP->GetDestX(), TP->GetDestY());
		}
	}

	for (auto& TP : engine->GetCloseTeleportDest(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetDestHitbox()))
		{
			Teleport(TP->GetPX(), TP->GetPY());
		}
	}
}

void cPlayer::Teleport(float px, float py)
{
	fTPX = px;
	fTPY = py;
	SetAttackable(true);
	bChangePos = true;

	engine->TransitionTo("GS_MAIN", true);
}

bool cPlayer::CanInteract()
{
	return !bDamaged && !bDead && !bIsGrabbedByEnnemy && !engine->IsBossKilled();
}

void cPlayer::ApplyGravity(float fElapsedTime)
{
	fVelY += engine->GetGravityValue() * fElapsedTime;
}

void cPlayer::Update(float fElapsedTime)
{
	if (bInteracting && CanInteract())
	{
		if (!bOnIcedGround)
			fVelX = 0.0f;
		else
			fVelX += fDrag * fVelX * fElapsedTime;

		if (bBreakDoor)
			fVelY = 0.0f;
	}
	else
	{
		fVelX += fDrag * fVelX * fElapsedTime;
		if (bOnGround)
		{
			bFlying = false;
			engine->StopSample("kirboFly");

			if (fabs(fVelX) < cfMinVelX)
			{
				fVelX = 0.0f;

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

					if (fVelY < 0)
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

void cPlayer::OneCycleAnimations(float fElapsedTime, olc::GFX2D::Transform2D* t, std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles, cLevel* lvl)
{
	if (bInteracting && !bDead)
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
					float fProjPosX = fPosX + (fFaceDir > 0.0f ? 1.0f : -(mapProjectiles["slapAOE"][0]->width / engine->GetTileWidth()));
					float fProjPosY = fPosY - ((mapProjectiles["slapAOE"][0]->height - engine->GetTileHeight()) / (2 * engine->GetTileHeight()));
					engine->AddProjectile(fProjPosX, fProjPosY, true, fFaceDir, 0.0f, cfSlapDuration, "slapAOE", false, cnSlapDmg * nDmgBoost, false, false, 0, true, -3.0f, "slap");
					bCanSpawnProjectile = false;
				}
			}
		}

		// Poyo
		if (bPoyo)
		{
			// Poyo
		}

		// Break door animation
		if (bBreakDoor)
		{
			if (fAnimationTimer >= cfBrokenDoorFrame * animPlayer->fTimeBetweenFrames)
			{
				if (bCanSpawnProjectile)
				{
					engine->PlaySample("enterDoor");
					float debrisDuration = animPlayer->mapStates[animPlayer->sCurrentState].size() * animPlayer->fTimeBetweenFrames;

					engine->AddProjectile(fPosX, fPosY, true, -6.0f, -6.0f, debrisDuration, "doorDebris1", true, 0, false, true, 0, false, 0.0f, "", false, "", true);
					engine->AddProjectile(fPosX, fPosY, true, +3.0f, -9.0f, debrisDuration, "doorDebris2", true, 0, false, true, 0, false, 0.0f, "", false, "", true);
					engine->AddProjectile(fPosX, fPosY, true, +9.0f, -3.0f, debrisDuration, "doorDebris3", true, 0, false, true, 0, false, 0.0f, "", false, "", true);

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
					engine->AddProjectile((fPosX + fFaceDir), fPosY - 1.0f, true, cfJesusCrossVelX * fFaceDir, cfJesusCrossVelY, cfJesusCrossDuration, "jesuscross", true, cnJesusCrossDmg * nDmgBoost, true);
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

		// Stop the action when it's finished
		float endAnimationTime = animPlayer->mapStates[animPlayer->sCurrentState].size() * animPlayer->fTimeBetweenFrames;
		if (bBreakDoor)
			endAnimationTime -= animPlayer->fTimeBetweenFrames;
		if (fAnimationTimer >= endAnimationTime)
		{
			fAnimationTimer = 0.0f;
			if (bBreakDoor)
				EnterDoor(lvl);
			StopAnyAction();
		}
	}

	if (bDamaged && !bDead)
	{
		// calculate elapsed time after damage
		fAnimationTimer += fElapsedTime;
		bFlying = false;
		StopAnyAction();

		if (fAnimationTimer >= animPlayer->mapStates[animPlayer->sCurrentState].size() * animPlayer->fTimeBetweenFrames)
		{
			fAnimationTimer = 0.0f;
			bDamaged = false;
		}
	}

	if (bDead)
	{
		fVelX = 0.0f;
		fVelY = 0.0f;
		StopAnyAction();

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
		fVelX = 0.0f;
		fVelY = 0.0f;
		StopAnyAction();

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

void cPlayer::StopAnyAction()
{
	bInteracting = false;
	bSlapping = false;
	bLaunchingJesusCross = false;
	bSwallowing = false;
	bPoyo = false;
	bBreakDoor = false;
}

void cPlayer::ClampVelocities()
{
	if (fVelX > cfMaxVelX)  fVelX = cfMaxVelX;
	if (fVelX < -cfMaxVelX) fVelX = -cfMaxVelX;
	if (fVelY > cfMaxVelY)  fVelY = cfMaxVelY;
	if (fVelY < -cfMaxVelY) fVelY = -cfMaxVelY;

	if (bFlying)
	{
		if (fVelX > cfMaxFlyingVelX)  fVelX = cfMaxFlyingVelX;
		if (fVelX < -cfMaxFlyingVelX) fVelX = -cfMaxFlyingVelX;
		if (fVelY > cfMaxFlyingVelY)  fVelY = cfMaxFlyingVelY;
	}
}

void cPlayer::IncreaseVelocities(float dvx, float dvy)
{
	fVelX += dvx;
	fVelY += dvy;
}

void cPlayer::DecreaseVelocities(float dvx, float dvy)
{
	fVelX -= dvx;
	fVelY -= dvy;
}

void cPlayer::SetVelocities(float vx, float vy)
{
	fVelX = vx;
	fVelY = vy;
}

void cPlayer::Collisions(float fElapsedTime, cLevel* lvl)
{
	float fNewVelX = fVelX;
	float fNewVelY = fVelY;

	for (auto& wind : engine->GetCloseWinds(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, wind->GetHitbox()))
		{
			if (wind->GetDirection() == "up")    fNewVelY = fVelY - wind->GetPower();
			if (wind->GetDirection() == "down")  fNewVelY = fVelY + wind->GetPower();
			if (wind->GetDirection() == "left")  fNewVelX = fVelX - wind->GetPower();
			if (wind->GetDirection() == "right") fNewVelX = fVelX + wind->GetPower();
		}
	}

	float fNewPosX = fPosX + fNewVelX * fElapsedTime;
	float fNewPosY = fPosY + fNewVelY * fElapsedTime;

	CheckPickUp(lvl, fNewPosX + 0.0f, fNewPosY + 0.0f);
	CheckPickUp(lvl, fNewPosX + 0.0f, fNewPosY + 1.0f);
	CheckPickUp(lvl, fNewPosX + 1.0f, fNewPosY + 0.0f);
	CheckPickUp(lvl, fNewPosX + 1.0f, fNewPosY + 1.0f);

	CheckHole(lvl);

	// Collision
	if (fNewVelX <= 0) // Moving Left
	{
		if (fNewPosX <= 1) fNewPosX = 1; // Prevent from being brutally moved to 0 only when reaching -1

		CheckLeftWall(lvl, fNewPosX);
		CheckRightWall(lvl, fNewPosX);
	}
	else // Moving Right
	{
		if (fNewPosX >= lvl->GetWidth() - 2) fNewPosX = lvl->GetWidth() - 2; // Kirbo can't cross the edge of the map

		CheckLeftWall(lvl, fNewPosX);
		CheckRightWall(lvl, fNewPosX);
	}

	bOnGround = false;
	if (fNewVelY <= 0) // Moving Up
	{
		if (fNewPosY <= 1) fNewPosY = 1; // Prevent from being brutally moved to 0 only when reaching -1

		CheckDynamicCeiling(fNewPosX, fNewPosY, lvl);
		CheckDynamicFloor(fNewPosX, fNewPosY, fElapsedTime, lvl);

		CheckSolidCeiling(lvl, fNewPosX, fNewPosY);
	}
	else // Moving Down
	{
		CheckDynamicCeiling(fNewPosX, fNewPosY, lvl);
		CheckDynamicFloor(fNewPosX, fNewPosY, fElapsedTime, lvl);

		CheckSolidFloor(lvl, fNewPosX, fNewPosY);
	}

	fPosX = fNewPosX;
	fPosY = fNewPosY;
}

void cPlayer::CheckHole(cLevel* lvl)
{
	if (fPosY > lvl->GetHeight())
	{
		if (!bDead)
			engine->PlaySample("kirboHit");
		Kill();
	}
}

void cPlayer::CheckPickUp(cLevel* lvl, float fNewPosX, float fNewPosY)
{
	if (IsCollectibleItem(lvl->GetTile(fNewPosX, fNewPosY)))
	{
		SelectItem(lvl->GetTile(fNewPosX, fNewPosY));
		lvl->SetTile(fNewPosX, fNewPosY, L'.');
	}
}

void cPlayer::CheckSolidFloor(cLevel* lvl, float fNewPosX, float& fNewPosY)
{
	if (SolidFloor(lvl, fNewPosX, fNewPosY) || SemiSolidFloor(lvl, fNewPosX, fNewPosY))
	{
		if (DynamicCeiling(fNewPosX, fNewPosY) && fCrushingObjVY > 0.0f)
		{
			Crushed();
		}

		NormalDrag();

		fNewPosY = (int)fNewPosY + engine->GetGroundDynamicOverlay(); // Remove this line to create shifting sand
		fVelY = 0;
		bOnGround = true;
		bDoubleJump = true;
	}
}

void cPlayer::NormalDrag()
{
	fDrag = cfNormalDrag;
	fAccGrdX = cfAccGrdX;
	fAccAirX = cfAccAirX;
	bOnIcedGround = false;
}

void cPlayer::CheckDynamicFloor(float& fNewPosX, float& fNewPosY, float fElapsedTime, cLevel* lvl)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->TopCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY + 0.8f) ||
			ptfm->TopCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY + 0.8f, fNewPosY + 0.8f))
		{
			HarmfulBloc(ptfm);

			if (ptfm->IsIced())
				IcedDrag();
			else
				NormalDrag();

			if (!bChargeDoubleJump && !bChargeJump && !bUsingFlyCmd || (ptfm->GetVY() <= 0.0f && ptfm->GetVY() < fVelY))
			{
				fNewPosY = ptfm->GetPY() - 0.8f;
				fNewPosX += ptfm->GetVX() * fElapsedTime;
			}

			// Check if a wall is there
			if (fNewPosX < fPosX)
			{
				CheckLeftWall(lvl, fNewPosX);
			}
			if (fNewPosX > fPosX)
			{
				CheckRightWall(lvl, fNewPosX);
			}

			if ((Ceiling(lvl, fNewPosX, fNewPosY) && ptfm->GetVY() < 0.0f) || (DynamicCeiling(fNewPosX, fNewPosY) && CeilingFloorCrushed(ptfm)))
			{
				Crushed();
			}

			fVelY = 0;
			bOnGround = true;
			bDoubleJump = true;
			ptfm->TriggerMovement();
		}
	}
}

void cPlayer::IcedDrag()
{
	fDrag = cfIcedDrag;
	fAccGrdX = cfAccIcedGrdX;
	fAccAirX = cfAccIcedAirX;
	bOnIcedGround = true;
}

bool cPlayer::CeilingFloorCrushed(cDynamicMovingPlatform*& ptfm)
{
	return fCrushingObjVY > 0.0f && ptfm->GetVY() < 0.0f ||
		fCrushingObjVY > 0.0f && ptfm->GetVY() > 0.0f && fCrushingObjVY > ptfm->GetVY() ||
		fCrushingObjVY < 0.0f && ptfm->GetVY() < 0.0f && fCrushingObjVY > ptfm->GetVY();
}

void cPlayer::CheckSolidCeiling(cLevel* lvl, float fNewPosX, float& fNewPosY)
{
	if (Ceiling(lvl, fNewPosX, fNewPosY))
	{
		fNewPosY = (int)fNewPosY + 1;
		fVelY = 0;

		for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
		{
			if (ptfm->GetVY() < 0 &&
				(ptfm->TopCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY + 1.0f) ||
				 ptfm->TopCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY + 1.0f, fNewPosY + 1.0f)))
			{
				Crushed();
			}
		}
	}
}

void cPlayer::CheckDynamicCeiling(float fNewPosX, float& fNewPosY, cLevel* lvl)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->BotCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY) ||
			ptfm->BotCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY, fNewPosY))
		{
			HarmfulBloc(ptfm);

			if (ptfm->GetVY() >= fVelY)
				fNewPosY = ptfm->GetPY() + ptfm->GetNormalizedHeight();

			if (SolidFloor(lvl, fNewPosX, fNewPosY) && ptfm->GetVY() > 0.0f ||
				SemiSolidFloor(lvl, fNewPosX, fNewPosY) && ptfm->GetVY() > 0.0f ||
				DynamicFloor(fNewPosX, fNewPosY) && FloorCeilingCrushed(ptfm))
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

bool cPlayer::DynamicFloor(float fNewPosX, float fNewPosY)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->TopCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY + 1.0f) ||
			ptfm->TopCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY + 1.0f, fNewPosY + 1.0f))
		{
			fCrushingObjVY = ptfm->GetVY();
			return true;
		}
	}
	return false;
}

bool cPlayer::DynamicCeiling(float fNewPosX, float fNewPosY)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->BotCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY) ||
			ptfm->BotCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY, fNewPosY))
		{
			fCrushingObjVY = ptfm->GetVY();
			return true;
		}
	}
	return false;
}

bool cPlayer::SemiSolidFloor(cLevel* lvl, float fNewPosX, float fNewPosY)
{
	// This little trick (fPlayerPosY + 1.0f < (float)((int)fNewPosY + 1.0f) + 0.1f) checks if the player's feets are above the top of the semi-solid Block.
	// Otherwise the player is moved to the top of the block when his feets reach the bottom of the block
	// "fPlayerPosY + 1.0f" is the feets Y position, "(float)((int)fNewPosY + 1.0f) + 0.1f" takes the top of the block at the feets position and add a 0.1 delta, 
	// if the feets are above this delta, the player is moved on top of the block.
	return (engine->IsSemiSolidTile(lvl->GetTile(fNewPosX + fCollisionLowerLimit, fNewPosY + 1.0f)) ||
			engine->IsSemiSolidTile(lvl->GetTile(fNewPosX + fCollisionUpperLimit, fNewPosY + 1.0f))) && fPosY + 1.0f < (float)((int)fNewPosY + 1.0f) + 0.1f;
}

bool cPlayer::SolidFloor(cLevel* lvl, float fNewPosX, float fNewPosY)
{
	return engine->IsSolidTile(lvl->GetTile(fNewPosX + fCollisionLowerLimit, fNewPosY + 1.0f)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPosX + fCollisionUpperLimit, fNewPosY + 1.0f));
}

bool cPlayer::Ceiling(cLevel* lvl, float fNewPosX, float fNewPosY)
{
	return engine->IsSolidTile(lvl->GetTile(fNewPosX + 0.0f, fNewPosY)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPosX + fCollisionUpperLimit, fNewPosY));
}

void cPlayer::CheckRightWall(cLevel* lvl, float& fNewPosX)
{
	if (engine->IsSolidTile(lvl->GetTile(fNewPosX + 1.0f, fPosY + 0.0f)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPosX + 1.0f, fPosY + fCollisionUpperLimit)))
	{
		if (DynamicLeftWall(fNewPosX) && fCrushingObjVX > 0.0f)
		{
			Crushed();
		}

		fNewPosX = (int)fNewPosX;
		fVelX = 0;
	}

	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->LeftCollision(fPosY, fPosY + 1.0f, fNewPosX + 1.0f) ||
			ptfm->LeftCollisionWithLag(fPosY, fPosY + 1.0f, fPosX + 1.0f, fNewPosX + 1.0f))
		{
			if (DynamicLeftWall(fNewPosX) && LeftRightCrushed(ptfm))
			{
				Crushed();
			}

			HarmfulBloc(ptfm);

			if (ptfm->GetVX() <= fVelX)
			{
				fNewPosX = ptfm->GetPX() - 1.0f;
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

bool cPlayer::DynamicLeftWall(float fNewPosX)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->RightCollision(fPosY, fPosY + 1.0f, fNewPosX) ||
			ptfm->RightCollisionWithLag(fPosY, fPosY + 1.0f, fPosX, fNewPosX))
		{
			fCrushingObjVX = ptfm->GetVX();
			return true;
		}
	}
	return false;
}

void cPlayer::CheckLeftWall(cLevel* lvl, float& fNewPosX)
{
	if (engine->IsSolidTile(lvl->GetTile(fNewPosX + 0.0f, fPosY + 0.0f)) ||
		engine->IsSolidTile(lvl->GetTile(fNewPosX + 0.0f, fPosY + fCollisionUpperLimit)))
	{
		if (DynamicRightWall(fNewPosX) && fCrushingObjVX < 0.0f)
		{
			Crushed();
		}

		fNewPosX = (int)fNewPosX + 1;
		fVelX = 0;
	}

	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->RightCollision(fPosY, fPosY + 1.0f, fNewPosX) ||
			ptfm->RightCollisionWithLag(fPosY, fPosY + 1.0f, fPosX, fNewPosX))
		{
			if (DynamicRightWall(fNewPosX) && RightLeftCrushed(ptfm))
			{
				Crushed();
			}

			HarmfulBloc(ptfm);

			if (ptfm->GetVX() >= fVelX)
			{
				fNewPosX = ptfm->GetPX() + ptfm->GetNormalizedWidth();
			}
		}
	}
}

void cPlayer::HarmfulBloc(cDynamicMovingPlatform*& ptfm)
{
	if (!ptfm->IsFriendly() && bIsAttackable)
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

bool cPlayer::DynamicRightWall(float fNewPosX)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->LeftCollision(fPosY, fPosY + 1.0f, fNewPosX + 1.0f) ||
			ptfm->LeftCollisionWithLag(fPosY, fPosY + 1.0f, fPosX + 1.0f, fNewPosX + 1.0f))
		{
			fCrushingObjVX = ptfm->GetVX();
			return true;
		}
	}
	return false;
}

float cPlayer::GetPosX()
{
	return fPosX;
}

float cPlayer::GetPosY()
{
	return fPosY;
}

void cPlayer::SetPosX(float px)
{
	fPosX = px;
}

void cPlayer::SetPosY(float py)
{
	fPosY = py;
}

bool cPlayer::IsVacuuming()
{
	return bVacuuming;
}

void cPlayer::VacuumEnnemy(cDynamicCreature* object)
{
	// if one ennemy is under fSwallowDistance from kirbo, every swallowable ennemy is killed and kirbo starts swallowing animation
	float fTargetX = GetPosX() - object->GetPX();
	float fTargetY = GetPosY() - object->GetPY();
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	if (fDistance <= fSwallowDistance)
	{
		bSwallowing = true;
		bSwallowSound = true;
		bInteracting = true;
	}
}

bool cPlayer::IsAttackable()
{
	return bIsAttackable;
}

bool cPlayer::HasCandyPower()
{
	return bHasCandyPower;
}

void cPlayer::SetAttackable(bool attackable)
{
	bIsAttackable = attackable;
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
			bIsAttackable = true;
		}
	}
	else
	{
		fInvulnerabilityTickingTimer += fElapsedTime;
		if (fInvulnerabilityTickingTimer >= cfInvulnerabilityTickingSpeed)
		{
			fInvulnerabilityTickingTimer -= cfInvulnerabilityTickingSpeed;

			// Start ticking only after damage animation
			if (!bDamaged)
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
	bDamaged = true;
	bIsAttackable = false;
	fHealth -= round((float)(object->GetDamage() / (float)nDefBoost)); // Kirbo can't take 0 damage with defense boost

	if (fHealth <= 0.0f)
	{
		Kill();
	}

	if (!bDead)
	{
		// Knockback the player out of the ennemy
		if (object->GetPX() < fPosX)
		{
			fVelX = cfDamageEjectionVelX;
			fVelY = -cfDamageEjectionVelY;
		}
		else
		{
			fVelX = -cfDamageEjectionVelX;
			fVelY = -cfDamageEjectionVelY;
		}
	}
}

void cPlayer::Kill()
{
	fHealth = 0.0f;
	bDead = true;
	animPlayer->ChangeState("dead");
	nDmgBoost = 1;
	nDefBoost = 1;
}

void cPlayer::ResetVariables()
{
	fVelX = 0.0f;
	fVelY = 0.0f;
	fInvulnerabilityTimer = 0.0f;
	bDead = false;
	fDeadAnimation = 0.0f;
	bDamaged = false;
	fKirboGoesAwayTimer = 0.0f;
	fJumpTimer = 0.0f;
	bIsGrabbedByEnnemy = false;
	StopAnyAction();
}

void cPlayer::VacuumHitbox(float cameraOffsetX, float cameraOffsetY)
{
	vacuumHitbox->SetPos(
		(fPosX + (fFaceDir > 0.0f ? 1.75f : -0.75f) - cameraOffsetX) * engine->GetTileWidth(),
		(fPosY + 0.5f - cameraOffsetY) * engine->GetTileHeight()
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
		(fPosX + 0.5f - cameraOffsetX) * engine->GetTileWidth(),
		(fPosY + 0.5f - cameraOffsetY) * engine->GetTileHeight()
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
		float tx = victim->GetPX() - fPosX;
		float ty = victim->GetPY() - fPosY;
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

void cPlayer::EnemyCollision(cDynamic* object, float cameraOffsetX, float cameraOffsetY)
{
	if (EnemyTouched(object, cameraOffsetX, cameraOffsetY))
	{
		if (!HasCandyPower())
			Damage(object);
		else
			Attack((cDynamicCreature*)object, GetCandyDmg());
	}
}

bool cPlayer::EnemyTouched(cDynamic* object, float cameraOffsetX, float cameraOffsetY)
{
	object->UpdateHitbox(cameraOffsetX, cameraOffsetY);

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

void cPlayer::ChangePosAfterTP()
{
	if (bChangePos)
	{
		fPosX = fTPX;
		fPosY = fTPY;
		bChangePos = false;
	}
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
	nDmgBoost = boost;
}

bool cPlayer::HasDamageBooster()
{
	return nDmgBoost > 1;
}

void cPlayer::SetDefenseBooster(int boost)
{
	nDefBoost = boost;
}

bool cPlayer::HasDefenseBooster()
{
	return nDefBoost > 1;
}
