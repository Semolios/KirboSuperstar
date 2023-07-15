#include "Platformer_Player.h"

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

void cPlayer::HandleInput(float fElapsedTime, cCamera* camera, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (engine->IsFocused() && CanInteract(engine))
	{
		// Fly, enter a door
		if (engine->GetKey(engine->GetSavedControls("flyOrEnterDoor")).bHeld || engine->GetController()->GetButton(UP).bHeld || engine->GetController()->GetLeftStickY() > 0.5f)
		{
			bUsingFlyCmd = true;
			if (!bInteracting && !bVacuuming && !bHoldingCamera)
			{
				if (IsEnteringDoor(lvl, engine))
				{
					animPlayer->ChangeState("enterDoor", playerSprite, playerDecal);
					bInteracting = true;
					bFlying = false;
					bBreakDoor = true;
					fAnimationTimer = 0.0f;
					fWaitBeforeReenterDoor = cfWaitBeforeReenterDoor;
					bCanSpawnProjectile = true;
					SetAttackable(false);
				}

				if (!bBreakDoor)
				{
					fVelY = -cfVelY;
					bFlying = true;
					animPlayer->ChangeState("flying", playerSprite, playerDecal);
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
			if ((engine->IsSemiSolidTile(lvl->GetTile(fPosX + 0.0f,					fPosY + 1.0f)) ||
				 engine->IsSemiSolidTile(lvl->GetTile(fPosX + fCollisionUpperLimit, fPosY + 1.0f))) && bOnGround)
			{
				fPosY += 0.15;
			}

			// If player is on Solid Ground, lower the camera
			if ((engine->IsSolidTile(lvl->GetTile(fPosX + 0.0f,					fPosY + 1.0f)) ||
				 engine->IsSolidTile(lvl->GetTile(fPosX + fCollisionUpperLimit, fPosY + 1.0f))) && bOnGround)
			{
				if (!bVacuuming && !bInteracting && !bHoldingCamera)
				{
					animPlayer->ChangeState("grabCamera", playerSprite, playerDecal);
					bHoldingCamera = true;
					fAnimationTimer = 0.0f;
				}
				bInteracting = true;
				camera->LowerPosition();
			}

			// Moving platforms collision
			for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
			{
				if (ptfm->TopCollision(fPosX + fCollisionLowerLimit, fPosX + fCollisionUpperLimit, fPosY + 0.8f))
				{
					if (!bVacuuming && !bInteracting && !bHoldingCamera)
					{
						animPlayer->ChangeState("grabCamera", playerSprite, playerDecal);
						bHoldingCamera = true;
						fAnimationTimer = 0.0f;
					}
					bInteracting = true;
					camera->LowerPosition();
				}
			}
		}
		else
		{
			bHoldingCamera = false;
			camera->RaisePosition();
		}

		// Go left
		if (engine->GetKey(engine->GetSavedControls("moveLeft")).bHeld || engine->GetController()->GetButton(LEFT).bHeld || engine->GetController()->GetLeftStickX() < -0.25f)
		{
			if (!bInteracting && !bVacuuming && !bHoldingCamera)
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
			if (!bInteracting && !bVacuuming && !bHoldingCamera)
			{
				// Init walking sound
				engine->PlaySample("kirboWalk", false, true);

				// Init speed by cfMinPlayerVelX + 0.05 or the player won't move when on ground
				if (fabs(fVelX) < cfMinVelX) fVelX += (cfMinVelX + 0.05f);

				fVelX += (bOnGround ? fAccGrdX : fAccAirX) * fElapsedTime;
				fFaceDir = 1.0f;
				bRunningRight = true;
			}
			else
			{
				bRunningRight = false;
			}
		}
		else
		{
			bRunningRight = false;
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
				Jump(engine);
				bChargeJump = true;
			}
			else if (bDoubleJump && fVelY > 0)
			{
				Jump(engine);
				bDoubleJump = false;
				bChargeDoubleJump = true;
			}
		}

		// The more you hold, the higher you go
		if (engine->GetKey(engine->GetSavedControls("jump")).bHeld || engine->GetController()->GetButton(A).bHeld)
		{
			if (!bInteracting && !bVacuuming && !bHoldingCamera)
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
				animPlayer->ChangeState("slap", playerSprite, playerDecal);
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
			engine->TransitionTo("GS_PAUSE", true, true);
			engine->BreakLoop();
			return;
		}

		// Launch a Jesus Cross
		if (engine->GetKey(engine->GetSavedControls("launchJesusCross")).bPressed || engine->GetController()->GetButton(Y).bPressed)
		{
			// Can't spam Launching cross, can't launch when player is flying
			if (!bInteracting && !bFlying)
			{
				animPlayer->ChangeState("jesus_christ", playerSprite, playerDecal);
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
				if (!bVacuuming && !bInteracting && !bHoldingCamera)
				{
					engine->PlaySample("beginVacuum");
					animPlayer->ChangeState("begin_vacuum", playerSprite, playerDecal);
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
                if (!HasCandyPower())
                {
                    SetInvincible(cfVacuumInvincibleFrame, false);
                    bVacuumInvincible = true;
                }
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

				animPlayer->ChangeState("poyo", playerSprite, playerDecal);
				bInteracting = true;
				bPoyo = true;
				fAnimationTimer = 0.0f;
			}
		}
	}
}

void cPlayer::Jump(OneLoneCoder_Platformer* engine)
{
	engine->PlaySample("kirboJump");
	engine->AddProjectile(fPosX, fPosY, true, 0.0f, 0.1f, cfFartDuration, "kirboFart", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
}

bool cPlayer::IsEnteringDoor(cLevel* lvl, OneLoneCoder_Platformer* engine)
{
	if (lvl->GetTile(fPosX + 0.5f, fPosY + 0.5f) == L'w' && bOnGround)
		return true;

	for (auto& TP : engine->GetCloseTeleport(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetHitbox()) && fWaitBeforeReenterDoor <= 0.0f)
			return true;
	}

	for (auto& TP : engine->GetCloseTeleportDest(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetDestHitbox()) && fWaitBeforeReenterDoor <= 0.0f)
			return true;
	}

	return false;
}

void cPlayer::EnterDoor(cLevel* lvl, OneLoneCoder_Platformer* engine)
{
	if (lvl->GetTile(fPosX + 0.5f, fPosY + 0.5f) == L'w')
	{
		SetAttackable(true);
		engine->SetGameState("GS_LOADBOSSLEVEL");
	}

	EnterTP(lvl, engine);
}

void cPlayer::EnterTP(cLevel* level, OneLoneCoder_Platformer* engine)
{
	for (auto& TP : engine->GetCloseTeleport(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetHitbox()))
		{
			Teleport(TP->GetDestX(), TP->GetDestY(), level,  engine);
		}
	}

	for (auto& TP : engine->GetCloseTeleportDest(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetDestHitbox()))
		{
			Teleport(TP->GetPX(), TP->GetPY(), level, engine);
		}
	}
}

void cPlayer::Teleport(float px, float py, cLevel* level, OneLoneCoder_Platformer* engine)
{
	fTPX = px;
	fTPY = py;
	SetAttackable(true);
	bChangePos = true;

	// respawn all ennemies of the level
	engine->RespawnEnnemies(level);

	engine->TransitionTo("GS_MAIN", true, true);
}

bool cPlayer::CanInteract(OneLoneCoder_Platformer* engine)
{
	return !bDamaged && !bDead && !bIsGrabbedByEnnemy && !engine->IsBossKilled();
}

void cPlayer::ApplyGravity(float fElapsedTime, OneLoneCoder_Platformer* engine)
{
	fVelY += engine->GetGravityValue() * fElapsedTime;
}

void cPlayer::Update(float fElapsedTime, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (bInteracting && CanInteract(engine))
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
		fWaitBeforeReenterDoor -= fElapsedTime;
		if (fWaitBeforeReenterDoor <= 0.0f)
			fWaitBeforeReenterDoor = 0.0f;

		fVelX += fDrag * fVelX * fElapsedTime;
		if (bOnGround)
		{
			bFlying = false;
			engine->StopSample("kirboFly");

			if (fabs(fVelX) < cfMinVelX)
			{
				fVelX = 0.0f;

				if (CanInteract(engine))
					animPlayer->ChangeState("idle", playerSprite, playerDecal);
			}
			else
			{
				if (CanInteract(engine))
					animPlayer->ChangeState("run", playerSprite, playerDecal);
			}
		}
		else
		{
			if (CanInteract(engine))
			{
				if (!bFlying)
				{
					engine->StopSample("kirboFly");

					if (fVelY < 0)
						animPlayer->ChangeState("jump", playerSprite, playerDecal);
					else
						animPlayer->ChangeState("fall", playerSprite, playerDecal);
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

void cPlayer::OneCycleAnimations(float fElapsedTime, float& angle, float& offsetX, float& offsetY, std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
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
					engine->AddProjectile((fPosX + fFaceDir), fPosY - 1.0f, true, cfJesusCrossVelX * fFaceDir, cfJesusCrossVelY, cfJesusCrossDuration, "jesuscross", true, cnJesusCrossDmg * nDmgBoost, true, true, 0, true, 0.0f, "explosion", false, "", false, "explosion", cfExplosionDuration);
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
				animPlayer->ChangeState("vacuum", playerSprite, playerDecal);
			}
		}

		// Holding camera
		if (bHoldingCamera)
		{
			if (fAnimationTimer >= cfGrabbingCameraAnimT * animPlayer->fTimeBetweenFrames)
			{
				animPlayer->ChangeState("holdCamera", playerSprite, playerDecal);
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

			animPlayer->ChangeState("swallow", playerSprite, playerDecal);
		}

		// Stop the action when it's finished
		float endAnimationTime = animPlayer->mapStates[animPlayer->sCurrentState].size() * animPlayer->fTimeBetweenFrames;
		if (bBreakDoor)
			endAnimationTime -= animPlayer->fTimeBetweenFrames;
		if (fAnimationTimer >= endAnimationTime)
		{
			if (bSwallowing && !HasCandyPower())
			{
				SetInvincible(cfVacuumInvincibleFrame, false);
				bVacuumInvincible = true;
			}

			fAnimationTimer = 0.0f;
			if (bBreakDoor)
				EnterDoor(lvl, engine);
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
		{
			engine->PlaySample("loseLife");
			bExplosion = true;
		}

		fDeadAnimation += fElapsedTime;
		if (fDeadAnimation != fElapsedTime)
		{
			angle = fDeadAnimation * cfDeadRotationAnimation;
			// animation based on a 2nd degree polynome to simulate kirbo's death animation
			offsetX = 0.0f;
			offsetY = (4.0f * fDeadAnimation - 2.0f) * 64.0f * (4.0f * fDeadAnimation - 2.0f) - 4 * 64.0f;
		}

		if (bExplosion && fDeadAnimation >= cfExplosionAnimation)
		{
			bExplosion = false;
			engine->PlaySample("explosion");
			engine->AddProjectile(fPosX + cfExplosionOffsetX, fPosY + cfExplosionOffsetY, true, 0.0f, 0.0f, cfExplosionDuration, "explosion", false, 0, false, true, 0, true, 0.0f, "", false, "", true);
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

			engine->ReturnToWorldMap(true);
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
			animPlayer->ChangeState("kirbo_goes_away", playerSprite, playerDecal);

			angle = fKirboGoesAwayTimer * cfGoAwayRotationAnimation;
			offsetX = fKirboGoesAwayTimer * cfGoAwayTranslationAnimation;
			offsetY = -fKirboGoesAwayTimer * cfGoAwayTranslationAnimation;
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
    if (!bProjected)
    {
        if (fVelX > cfMaxVelX)  fVelX = cfMaxVelX;
        if (fVelX < -cfMaxVelX) fVelX = -cfMaxVelX;
        if (fVelY > cfMaxVelY)  fVelY = cfMaxVelY;
        if (fVelY < -cfMaxVelY) fVelY = -cfMaxVelY;
    }

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

void cPlayer::Collisions(float fElapsedTime, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
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

	// Permanent wind in Halberd level
	if (lvl->GetCurrentLvl() == 4 && !bBreakDoor && !engine->IsInBossLevel())
	{
		fNewVelX = fVelX - cfHalberdWindForce;

		if (bRunningRight && fNewVelX <= 0.0f)
		{
			fNewVelX = 0.1f;
		}
	}

	float fNewPosX = fPosX + fNewVelX * fElapsedTime;
	float fNewPosY = fPosY + fNewVelY * fElapsedTime;

	CheckPickUp(lvl, fNewPosX + 0.0f, fNewPosY + 0.0f, engine);
	CheckPickUp(lvl, fNewPosX + 0.0f, fNewPosY + 1.0f, engine);
	CheckPickUp(lvl, fNewPosX + 1.0f, fNewPosY + 0.0f, engine);
	CheckPickUp(lvl, fNewPosX + 1.0f, fNewPosY + 1.0f, engine);

	CheckHole(lvl, engine, playerSprite, playerDecal);

	// Collision
	if (fNewVelX <= 0) // Moving Left
	{
		if (fNewPosX <= 1) fNewPosX = 1; // Prevent from being brutally moved to 0 only when reaching -1

		CheckLeftWall(lvl, fNewPosX, engine, playerSprite, playerDecal);
		CheckRightWall(lvl, fNewPosX, engine, playerSprite, playerDecal);
	}
	else // Moving Right
	{
		if (fNewPosX >= lvl->GetWidth() - 2) fNewPosX = lvl->GetWidth() - 2; // Kirbo can't cross the edge of the map

		CheckLeftWall(lvl, fNewPosX, engine, playerSprite, playerDecal);
		CheckRightWall(lvl, fNewPosX, engine, playerSprite, playerDecal);
	}

	bOnGround = false;
	if (fNewVelY <= 0) // Moving Up
	{
		if (fNewPosY <= 1) fNewPosY = 1; // Prevent from being brutally moved to 0 only when reaching -1

		CheckDynamicCeiling(fNewPosX, fNewPosY, lvl, engine, playerSprite, playerDecal);
		CheckDynamicFloor(fNewPosX, fNewPosY, fElapsedTime, lvl, engine, playerSprite, playerDecal);

		CheckSolidCeiling(lvl, fNewPosX, fNewPosY, engine, playerSprite, playerDecal);
	}
	else // Moving Down
	{
		CheckDynamicCeiling(fNewPosX, fNewPosY, lvl, engine, playerSprite, playerDecal);
		CheckDynamicFloor(fNewPosX, fNewPosY, fElapsedTime, lvl, engine, playerSprite, playerDecal);

		CheckSolidFloor(lvl, fNewPosX, fNewPosY, engine, playerSprite, playerDecal);
	}

	fPosX = fNewPosX;
	fPosY = fNewPosY;
}

void cPlayer::CheckHole(cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (fPosY > lvl->GetHeight())
	{
		if (!bDead)
			engine->PlaySample("kirboHit");
		
		fVelY = -10.0f;
		if(IsAttackable())
            Damage(nullptr, engine, playerSprite, playerDecal);
	}
}

void cPlayer::CheckPickUp(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine)
{
	if (IsCollectibleItem(lvl->GetTile(fNewPosX, fNewPosY)))
	{
		SelectItem(lvl->GetTile(fNewPosX, fNewPosY), engine);
		lvl->SetTile(fNewPosX, fNewPosY, L'.');
	}
}

void cPlayer::CheckSolidFloor(cLevel* lvl, float fNewPosX, float& fNewPosY, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (SolidFloor(lvl, fNewPosX, fNewPosY, engine) || SemiSolidFloor(lvl, fNewPosX, fNewPosY, engine))
	{
		if (DynamicCeiling(fNewPosX, fNewPosY, engine) && fCrushingObjVY > 0.0f)
		{
			Crushed(engine, playerSprite, playerDecal);
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

void cPlayer::CheckDynamicFloor(float& fNewPosX, float& fNewPosY, float fElapsedTime, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->TopCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY + 0.8f) ||
			ptfm->TopCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY + 0.8f, fNewPosY + 0.8f))
		{
			HarmfulBloc(ptfm, engine, playerSprite, playerDecal);

			if (ptfm->IsIced())
				IcedDrag();
			else
				NormalDrag();

			if ((!bChargeDoubleJump && !bChargeJump && !bUsingFlyCmd || (ptfm->GetVY() <= 0.0f && ptfm->GetVY() < fVelY)) && ptfm->IsHarmfulblocTangible())
			{
				fNewPosY = ptfm->GetPY() - 0.8f;
				fNewPosX += ptfm->GetVX() * fElapsedTime;
			}

			// Check if a wall is there
			if (fNewPosX < fPosX)
			{
				CheckLeftWall(lvl, fNewPosX, engine, playerSprite, playerDecal);
			}
			if (fNewPosX > fPosX)
			{
				CheckRightWall(lvl, fNewPosX, engine, playerSprite, playerDecal);
			}

			if ((Ceiling(lvl, fNewPosX, fNewPosY, engine) && ptfm->GetVY() < 0.0f) || (DynamicCeiling(fNewPosX, fNewPosY, engine) && CeilingFloorCrushed(ptfm)))
			{
				Crushed(engine, playerSprite, playerDecal);
			}

			if (ptfm->IsHarmfulblocTangible())
			{
				fVelY = 0;
				bOnGround = true;
				bDoubleJump = true;
			}
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

void cPlayer::CheckSolidCeiling(cLevel* lvl, float fNewPosX, float& fNewPosY, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (Ceiling(lvl, fNewPosX, fNewPosY, engine))
	{
		fNewPosY = (int)fNewPosY + 1;
		fVelY = 0;

		for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
		{
			if (ptfm->GetVY() < 0 &&
				(ptfm->TopCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY + 1.0f) ||
				 ptfm->TopCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY + 1.0f, fNewPosY + 1.0f)))
			{
				Crushed(engine, playerSprite, playerDecal);
			}
		}
	}
}

void cPlayer::CheckDynamicCeiling(float fNewPosX, float& fNewPosY, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->BotCollision(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fNewPosY) ||
			ptfm->BotCollisionWithLag(fNewPosX + fCollisionLowerLimit, fNewPosX + fCollisionUpperLimit, fPosY, fNewPosY))
		{
			HarmfulBloc(ptfm, engine, playerSprite, playerDecal);

			if ((ptfm->GetVY() >= fVelY) && ptfm->IsHarmfulblocTangible())
				fNewPosY = ptfm->GetPY() + ptfm->GetNormalizedHeight();

			if (SolidFloor(lvl, fNewPosX, fNewPosY, engine) && ptfm->GetVY() > 0.0f ||
				SemiSolidFloor(lvl, fNewPosX, fNewPosY, engine) && ptfm->GetVY() > 0.0f ||
				DynamicFloor(fNewPosX, fNewPosY, engine) && FloorCeilingCrushed(ptfm))
			{
				Crushed(engine, playerSprite, playerDecal);
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

void cPlayer::Crushed(OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (!bDead)
		engine->PlaySample("kirboHit");
    if (IsAttackable())
        Damage(nullptr, engine, playerSprite, playerDecal);
}

bool cPlayer::DynamicFloor(float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine)
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

bool cPlayer::DynamicCeiling(float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine)
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

bool cPlayer::SemiSolidFloor(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine)
{
	// This little trick (fPosY + 1.0f < (float)((int)fNewPosY + 1.0f) + 0.1f) checks if the player's feets are above the top of the semi-solid Block.
	// Otherwise the player is moved to the top of the block when his feets reach the bottom of the block
	// "fPosY + 1.0f" is the feets Y position, "(float)((int)fNewPosY + 1.0f) + 0.1f" takes the top of the block at the feets position and add a 0.1 delta, 
	// if the feets are above this delta, the player is moved on top of the block.
	return (engine->IsSemiSolidTile(lvl->GetTile(fNewPosX + fCollisionLowerLimit, fNewPosY + 1.0f)) ||
			engine->IsSemiSolidTile(lvl->GetTile(fNewPosX + fCollisionUpperLimit, fNewPosY + 1.0f))) && fPosY + 1.0f < (float)((int)fNewPosY + 1.0f) + 0.1f;
}

bool cPlayer::SolidFloor(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine)
{
	return engine->IsSolidTile(lvl->GetTile(fNewPosX + fCollisionLowerLimit, fNewPosY + 1.0f)) ||
		   engine->IsSolidTile(lvl->GetTile(fNewPosX + fCollisionUpperLimit, fNewPosY + 1.0f));
}

bool cPlayer::Ceiling(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine)
{
	return engine->IsSolidTile(lvl->GetTile(fNewPosX + 0.0f,				 fNewPosY)) ||
		   engine->IsSolidTile(lvl->GetTile(fNewPosX + fCollisionUpperLimit, fNewPosY));
}

void cPlayer::CheckRightWall(cLevel* lvl, float& fNewPosX, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (engine->IsSolidTile(lvl->GetTile(fNewPosX + 1.0f, fPosY + 0.0f				  )) ||
		engine->IsSolidTile(lvl->GetTile(fNewPosX + 1.0f, fPosY + fCollisionUpperLimit)))
	{
		if (DynamicLeftWall(fNewPosX, engine) && fCrushingObjVX > 0.0f)
		{
			Crushed(engine, playerSprite, playerDecal);
		}

		fNewPosX = (int)fNewPosX;
		fVelX = 0;
	}

	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->LeftCollision(fPosY, fPosY + 1.0f, fNewPosX + 1.0f) ||
			ptfm->LeftCollisionWithLag(fPosY, fPosY + 1.0f, fPosX + 1.0f, fNewPosX + 1.0f))
		{
			if (DynamicLeftWall(fNewPosX, engine) && LeftRightCrushed(ptfm))
			{
				Crushed(engine, playerSprite, playerDecal);
			}

			HarmfulBloc(ptfm, engine, playerSprite, playerDecal);

			if ((ptfm->GetVX() <= fVelX || fVelX == 0) && ptfm->IsHarmfulblocTangible())
			{
				if (ptfm->GetVX() != 0.0f)
					fNewPosX = ptfm->GetPX() - 1.0f;
				else
					fNewPosX = fPosX;
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

bool cPlayer::DynamicLeftWall(float fNewPosX, OneLoneCoder_Platformer* engine)
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

void cPlayer::CheckLeftWall(cLevel* lvl, float& fNewPosX, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (engine->IsSolidTile(lvl->GetTile(fNewPosX + 0.0f, fPosY + 0.0f				  )) ||
		engine->IsSolidTile(lvl->GetTile(fNewPosX + 0.0f, fPosY + fCollisionUpperLimit)))
	{
		if (DynamicRightWall(fNewPosX, engine) && fCrushingObjVX < 0.0f)
		{
			Crushed(engine, playerSprite, playerDecal);
		}

		fNewPosX = (int)fNewPosX + 1;
		fVelX = 0;
	}

	for (auto& ptfm : engine->GetClosePlatforms(fPosX, fPosY))
	{
		if (ptfm->RightCollision(fPosY, fPosY + 1.0f, fNewPosX) ||
			ptfm->RightCollisionWithLag(fPosY, fPosY + 1.0f, fPosX, fNewPosX))
		{
			if (DynamicRightWall(fNewPosX, engine) && RightLeftCrushed(ptfm))
			{
				Crushed(engine, playerSprite, playerDecal);
			}

			HarmfulBloc(ptfm, engine, playerSprite, playerDecal);

			if ((ptfm->GetVX() >= fVelX || fVelX == 0) && ptfm->IsHarmfulblocTangible())
			{
				if (ptfm->GetVX() != 0.0f)
					fNewPosX = ptfm->GetPX() + ptfm->GetNormalizedWidth();
				else
					fNewPosX = fPosX;
			}
		}
	}
}

void cPlayer::HarmfulBloc(cDynamicMovingPlatform*& ptfm, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (!ptfm->IsFriendly() && bIsAttackable)
	{
		Damage(ptfm, engine, playerSprite, playerDecal);
	}
}

bool cPlayer::RightLeftCrushed(cDynamicMovingPlatform*& ptfm)
{
	return fCrushingObjVX > 0.0f && ptfm->GetVX() < 0.0f ||
		   fCrushingObjVX > 0.0f && ptfm->GetVX() > 0.0f && fCrushingObjVX < ptfm->GetVX() ||
		   fCrushingObjVX < 0.0f && ptfm->GetVX() < 0.0f && fCrushingObjVX < ptfm->GetVX();
}

bool cPlayer::DynamicRightWall(float fNewPosX, OneLoneCoder_Platformer* engine)
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

void cPlayer::UpdateInvulnerability(float fElapsedTime, OneLoneCoder_Platformer* engine)
{
	fInvulnerabilityTimer -= fElapsedTime;
	if (fInvulnerabilityTimer <= 0.0f)
	{
		if (bVacuumInvincible || bHasCandyPower)
		{
			SetAttackable(true);
			bVacuumInvincible = false;
		}

		if (bHasCandyPower)
		{
			bHasCandyPower = false;
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

		bInvincibleBlink = true;
	}
	else
	{
		if (bInvincibleBlink)
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
}

void cPlayer::DrawKirbo(float posx, float posy, float angle, float faceDir, OneLoneCoder_Platformer* engine, olc::Decal* playerDecal)
{
	if (bShowKirbo)
	{
		animPlayer->DrawSelf(posx, posy, angle, faceDir, engine, playerDecal);
	}
}

float cPlayer::GetHealth()
{
	return fHealth;
}

void cPlayer::Damage(cDynamic* object, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	engine->PlaySample("kirboHit");
	animPlayer->ChangeState("damaged", playerSprite, playerDecal);
	fInvulnerabilityTimer = cfInvulnerabilityFrame;
	bDamaged = true;
	bIsAttackable = false;
	if (object != nullptr)
	{
        fHealth -= round((float)(object->GetDamage() / (float)nDefBoost)); // Kirbo can't take 0 damage with defense boost
	}
	else
	{
		fHealth -= 3; // Crushed and hole damages
	}

	if (fHealth <= 0.0f)
	{
		Kill(playerSprite, playerDecal);
	}

	if (!bDead)
	{
		// Knockback the player out of the ennemy
		if ((object != nullptr && object->GetPX() < fPosX) || (object == nullptr && GetFaceDir() < 0.0f))
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

void cPlayer::Kill(olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	fHealth = 0.0f;
	bDead = true;
	animPlayer->ChangeState("dead", playerSprite, playerDecal);
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

void cPlayer::VacuumHitbox(float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine)
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

void cPlayer::UpdateHitbox(float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine)
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

void cPlayer::Vacuum(cDynamicCreature* object, float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine)
{
	object->UpdateHitbox(cameraOffsetX, cameraOffsetY);
	VacuumHitbox(cameraOffsetX, cameraOffsetY, engine);

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

void cPlayer::EnemyCollision(cDynamic* object, float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	if (EnemyTouched(object, cameraOffsetX, cameraOffsetY))
	{
		if (!HasCandyPower())
		{
			Damage(object, engine, playerSprite, playerDecal);
			engine->HitStop();
		}
		else
		{
			Attack((cDynamicCreature*)object, GetCandyDmg());
		}
	}
}

bool cPlayer::EnemyTouched(cDynamic* object, float cameraOffsetX, float cameraOffsetY)
{
	object->UpdateHitbox(cameraOffsetX, cameraOffsetY);

	return cHitbox::ShapeOverlap_DIAG(object->GetHitbox(), GetHitbox());
}

void cPlayer::PickupGrabbed(cDynamicProjectile* object, float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine)
{
	object->UpdateHitbox(cameraOffsetX, cameraOffsetY);

	if (cHitbox::ShapeOverlap_DIAG(object->GetHitbox(), GetHitbox()))
	{
		cItemMinorHeal pickup = cItemMinorHeal();
		pickup.PickItem(engine);
		object->SetRedundant(true);
	}
}

void cPlayer::SetGrabbedByEnnemy(bool grabbed)
{
	bIsGrabbedByEnnemy = grabbed;
}

void cPlayer::ChangeAnimation(std::string animation, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	animPlayer->ChangeState(animation, playerSprite, playerDecal);
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

void cPlayer::DrawDoorCommand(cLevel* lvl, OneLoneCoder_Platformer* engine, float cameraOffsetX, float cameraOffsetY)
{
	if (lvl->GetTile(fPosX + 0.5f, fPosY + 0.5f) == L'w' && bOnGround)
	{
		DoorCommandPosition((int)(fPosX + 0.5f), (int)(fPosY + 0.5f), engine, cameraOffsetX, cameraOffsetY);
	}

	for (auto& TP : engine->GetCloseTeleport(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetHitbox()) && fWaitBeforeReenterDoor <= 0.0f)
		{
			DoorCommandPosition((int)(TP->GetPX()), (int)(TP->GetPY() + 1), engine, cameraOffsetX, cameraOffsetY);
		}
	}

	for (auto& TP : engine->GetCloseTeleportDest(fPosX, fPosY))
	{
		if (cHitbox::ShapeOverlap_DIAG(hitbox, TP->GetDestHitbox()) && fWaitBeforeReenterDoor <= 0.0f)
		{
			DoorCommandPosition((int)(TP->GetDestX()), (int)(TP->GetDestY() + 1), engine, cameraOffsetX, cameraOffsetY);
		}
	}
}

void cPlayer::DoorCommandPosition(int posX, int posY, OneLoneCoder_Platformer* engine, float cameraOffsetX, float cameraOffsetY)
{
	olc::vf2d sprPos;
	sprPos.x = (posX - cameraOffsetX) * engine->GetTileWidth();
	sprPos.y = (posY - cameraOffsetY) * engine->GetTileHeight() - engine->GetTileHeight();
	engine->DrawDecal(sprPos, engine->GetLoadedDecal("doorUp"));
}

void cPlayer::SetProjected(bool projected)
{
	bProjected = projected;
}

bool cPlayer::IsCollectibleItem(wchar_t c)
{
	return c == L't' || c == L'c' || c == L's' || c == L'd' || c == L'x';
}

void cPlayer::SelectItem(wchar_t item, OneLoneCoder_Platformer* engine)
{
	cItem* pickedItem;

	switch (item)
	{
		case L'c': pickedItem = new cItemCandy();	  break;
		case L's': pickedItem = new cItemDamage();	  break;
		case L'd': pickedItem = new cItemDefense();   break;
		case L'x': pickedItem = new cItemMinorHeal(); break;
		case L't': pickedItem = new cItemTomato();	  break;
		default:   pickedItem = new cItem();		  break;
	}

	pickedItem->PickItem(engine);
}

void cPlayer::MaxHeal()
{
	fHealth = cfMaxHealth;
}

void cPlayer::Heal(int hp)
{
	fHealth += hp;
	if (fHealth >= cfMaxHealth) fHealth = cfMaxHealth;
}

void cPlayer::SetInvincible(float time, bool blink)
{
	fInvulnerabilityTimer = time;
	bInvincibleBlink = blink;
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
