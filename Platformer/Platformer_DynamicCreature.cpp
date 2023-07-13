#include "Platformer_DynamicCreature.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreature::engine = nullptr;

cDynamicCreature::cDynamicCreature(std::string n, olc::Sprite* sprite, int framesPerSecond) : cDynamic(n)
{
    dynSprite = sprite;
    nHealth = 10;
    nHealthMax = 10;
    nGraphicState = STANDING;
    nGraphicCounter = 0;
    nFramesPerSecond = framesPerSecond;
    fTimer = 0.0f;
    bIsAttackable = true;
}

cDynamicCreature::~cDynamicCreature()
{
    delete hitbox;
    delete dynSprite;
    delete dynDecal;
    delete level;
}

void cDynamicCreature::InitDecal()
{
    dynDecal = new olc::Decal(dynSprite);
}

void cDynamicCreature::DrawSelf(float ox, float oy)
{
    int nSheetOffsetX = nGraphicCounter * fSpriteW;					// Same State of a sprite are stored in one line
    int nSheetOffsetY = (2 * nGraphicState + nFaceDir) * fSpriteH;	// 0 = Left Idle, 1 = Right Idle, 2 = Left Walking, 3 = Right Walking, 4 = Left Damaged, 5 = Right Damaged ...

    olc::vf2d pos;
    pos.x = (int)((px - ox) * 64.0f) + fSpriteOffsetX; // set integer values or the sprite will be messed a little when redered
    pos.y = (int)((py - oy) * 64.0f) + fSpriteOffsetY; // set integer values or the sprite will be messed a little when redered

    olc::vf2d sourcePos;
    sourcePos.x = nSheetOffsetX;
    sourcePos.y = nSheetOffsetY;
    
    olc::vf2d sourceSize;
    sourceSize.x = fSpriteW;
    sourceSize.y = fSpriteH;

    engine->DrawPartialDecal(pos, dynDecal, sourcePos, sourceSize);
}

void cDynamicCreature::Update(float fElapsedTime, float playerX, float playerY)
{
    fTimer += fElapsedTime;
    if (fTimer >= 1.0f / (float)nFramesPerSecond)
    {
        // loop animation only if bLoopAnimation is true
        if (bLoopAnimation || nGraphicCounter != nFramesPerSecond - 1)
        {
            fTimer -= 1.0f / (float)nFramesPerSecond;
            nGraphicCounter++;
            nGraphicCounter %= nFramesPerSecond;
        }
    }

    if (fKnockBackTimer > 0.0f)
    {
        if (bIsKnockable)
        {
            vx = fKnockBackDX * cfKnockBackDX;
            vy = fKnockBackDY * cfKnockBackDY;
        }
        bIsAttackable = false;
        fKnockBackTimer -= fElapsedTime;
        nGraphicState = DAMAGED;

        if (fKnockBackTimer <= 0.0f)
        {
            vx = fInitSpeed * signbit(vx) ? 1.0f : -1.0f; // The ennemy turns to kirbo
            if (nHealth <= 0)
            {
                if (bIsBoss)
                {
                    bBossKilled = true;
                }
                else
                {
                    GeneratePickUp();
                    nHealth = 0;
                    bDead = true;
                }
            }
            else
            {
                bIsAttackable = true;
            }
        }

        if (bCanBehaveWhileAttacked && !bVacuumed && !bBossKilled)
            Behaviour(fElapsedTime, playerX, playerY, engine);
        else if (sName == "rocky")
            engine->StopSample("rockyFall");

        if (bVacuumed)
        {
            vx = (playerX - px) * cfVacuumSpeed;
            vy = (playerY - py) * cfVacuumSpeed;

            // ennemies under cfSwallowableLimit is swallowed when swallowing animation occurs but once kirbo swallowed he stops vacuuming
            float fTargetX = playerX - px;
            float fTargetY = playerY - py;
            float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

            if (fDistance < cfSwallowableLimit)
            {
                GeneratePickUp();
                bSwallowable = true;
            }
        }
    }
    else if (!bBossKilled)
    {
        bSolidVsDyn = bSolidVsDynInitValue;

        if (fabs(vx) > 0)
            nGraphicState = WALKING;
        else
            nGraphicState = STANDING;

        if (vx < -0.1f) nFaceDir = 0;
        if (vx > 0.1f) nFaceDir = 1;

        Behaviour(fElapsedTime, playerX, playerY, engine);
    }
    else
    {
        ExplodeAndDie(fElapsedTime);
    }
}

void cDynamicCreature::GeneratePickUp()
{
    if (bCanGeneratePickUp)
    {
        int generatePickUp = rand() % 10;
        if (generatePickUp == 0 || bAlwaysGeneratePickUp)
        {
            engine->AddProjectile(px, py, true, 0.0f, 0.0f, 30, "pickup", true, 0, true, true, 0, false, 0.0f, "itemPicked", false, "", true, "", 0.0f, true);
        }
        bCanGeneratePickUp = false;
    }
}

void cDynamicCreature::KnockBack(float dx, float dy, float dist)
{
    fKnockBackDX = dx;
    fKnockBackDY = dy;
    fKnockBackTimer = dist;
    bSolidVsDyn = false;
    bIsAttackable = false;
}

void cDynamicCreature::TurnBack()
{
    vx = -vx;
}

void cDynamicCreature::Collision(float fElapsedTime)
{
    float fNewPosX = px + vx * fElapsedTime;
    float fNewPosY = py + vy * fElapsedTime;

    fNormalizedW = fDynWidth / engine->GetTileWidth();
    fNormalizedH = fDynHeight / engine->GetTileHeight();

    // Collision
    float fGap = 0.1f;

    float fLftCollision = fNewPosX + fGap;
    float fRgtCollision = fNewPosX + fNormalizedW - fGap;
    float fTopCollision = py + fGap;
    float fBotCollision = py + fNormalizedH - fGap;

    // Gravity
    if (bAffectedByGravity)
        vy += engine->GetGravityValue() * fElapsedTime;

    if (bSolidVsMap)
    {
        if (vx <= 0) // Moving Left
        {
            if (engine->IsSolidTile(level->GetTile(fLftCollision, fTopCollision)) ||
                engine->IsSolidTile(level->GetTile(fLftCollision, fBotCollision)))
            {
                fNewPosX = (int)fNewPosX + 1;
                vx = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->RightCollision(fNewPosY, fNewPosY + fNormalizedH, fLftCollision) ||
                     ptfm->RightCollisionWithLag(fNewPosY, fNewPosY + fNormalizedH, px, fNewPosX)) &&
                    ptfm->IsHarmfulblocTangible())
                {
                    fNewPosX = ptfm->GetPX() + ptfm->GetNormalizedWidth();
                    vx = 0;
                }
            }
        }
        else // Moving Right
        {
            if (engine->IsSolidTile(level->GetTile(fRgtCollision, fTopCollision)) ||
                engine->IsSolidTile(level->GetTile(fRgtCollision, fBotCollision)))
            {
                fNewPosX = (int)(fNewPosX + fNormalizedW) - fNormalizedW;
                vx = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->LeftCollision(fNewPosY, fNewPosY + fNormalizedH, fRgtCollision) ||
                     ptfm->LeftCollisionWithLag(fNewPosY, fNewPosY + fNormalizedH, px + fNormalizedW, fNewPosX + fNormalizedW)) &&
                    ptfm->IsHarmfulblocTangible())
                {
                    fNewPosX = ptfm->GetPX() - fNormalizedW;
                    vx = 0;
                }
            }
        }

        if (vy <= 0) // Moving Up
        {
            if (engine->IsSolidTile(level->GetTile(fLftCollision, fNewPosY)) ||
                engine->IsSolidTile(level->GetTile(fRgtCollision, fNewPosY)))
            {
                fNewPosY = (int)fNewPosY + 1;
                vy = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->BotCollision(fLftCollision, fRgtCollision, fNewPosY) ||
                     ptfm->BotCollisionWithLag(fLftCollision, fRgtCollision, py, fNewPosY)) &&
                    ptfm->IsHarmfulblocTangible())
                {
                    fNewPosY = ptfm->GetPY() + ptfm->GetNormalizedHeight();
                    vy = 0;
                }
            }
        }
        else // Moving Down
        {
            float fSemiSolidThickness = 0.1f;

            // Same process as kirbo crossing semi-solid platform
            if (engine->IsSolidTile(level->GetTile(fLftCollision, fNewPosY + fNormalizedH)) ||
                engine->IsSolidTile(level->GetTile(fRgtCollision, fNewPosY + fNormalizedH)) ||
                ((engine->IsSemiSolidTile(level->GetTile(fLftCollision, fNewPosY + fNormalizedH)) ||
                  engine->IsSemiSolidTile(level->GetTile(fRgtCollision, fNewPosY + fNormalizedH))) && py + fNormalizedH < (float)((int)(fNewPosY + fNormalizedH)) + fSemiSolidThickness))
            {
                fNewPosY = (int)(fNewPosY + fNormalizedH) - fNormalizedH + engine->GetGroundDynamicOverlay();
                vy = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->TopCollision(fLftCollision, fRgtCollision, fNewPosY + fNormalizedH) ||
                     ptfm->TopCollisionWithLag(fLftCollision, fRgtCollision, py + fNormalizedH, fNewPosY + fNormalizedH)) &&
                    ptfm->IsHarmfulblocTangible())
                {
                    fNewPosY = ptfm->GetPY() - fNormalizedH;
                    fNewPosX += ptfm->GetVX() * fElapsedTime;
                    vy = 0;
                }
            }
        }
    }

    px = fNewPosX;
    py = fNewPosY;
}

void cDynamicCreature::UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
{
    hitbox->SetPos(
        (px - cameraOffsetX) * engine->GetTileWidth() + (fDynWidth / 2.0f),
        (py - cameraOffsetY) * engine->GetTileHeight() + (fDynHeight / 2.0f)
    );
    hitbox->SetAngle(0.0f);
    hitbox->AddPoint(-fDynWidth / 2.0f, -fDynHeight / 2.0f);
    hitbox->AddPoint(-fDynWidth / 2.0f, +fDynHeight / 2.0f);
    hitbox->AddPoint(+fDynWidth / 2.0f, +fDynHeight / 2.0f);
    hitbox->AddPoint(+fDynWidth / 2.0f, -fDynHeight / 2.0f);
    hitbox->ResizeP(4);

    for (int i = 0; i < hitbox->GetOSize(); i++)
    {
        // 2D Rotation Transform + 2D Translation
        hitbox->SetP(i,
                     (hitbox->GetOIX(i) * cosf(hitbox->GetAngle())) - (hitbox->GetOIY(i) * sinf(hitbox->GetAngle())) + hitbox->GetPosX(),
                     (hitbox->GetOIX(i) * sinf(hitbox->GetAngle())) + (hitbox->GetOIY(i) * cosf(hitbox->GetAngle())) + hitbox->GetPosY()
        );
    }

    // Clear the vector o in the hitbox class or GetOSize() will not stop growing each frames
    hitbox->ClearO();

    // debug AOE
    //hitbox->Draw(engine, olc::YELLOW);
}

void cDynamicCreature::Vacuumed(bool vaccumedState)
{
    bSolidVsDyn = !vaccumedState;
    bVacuumed = vaccumedState;
    bIsKnockable = !vaccumedState;
}

int cDynamicCreature::GetHealth()
{
    return nHealth;
}

void cDynamicCreature::TakeDamage(int damage)
{
    nHealth -= damage;
}

int cDynamicCreature::GetGraphicCounter()
{
    return nGraphicCounter;
}

void cDynamicCreature::SetGraphicCounter(int counter)
{
    nGraphicCounter = counter;
}

bool cDynamicCreature::IsVacuumable()
{
    return bIsVacuumable;
}

void cDynamicCreature::SetVacuumable(bool vacuumable)
{
    bIsVacuumable = vacuumable;
}

bool cDynamicCreature::IsVacuumed()
{
    return bVacuumed;
}

bool cDynamicCreature::IsSwallowable()
{
    return bSwallowable;
}

void cDynamicCreature::SetSwallowable(bool swallowable)
{
    bSwallowable = swallowable;
}

bool cDynamicCreature::IsBoss()
{
    return bIsBoss;
}

void cDynamicCreature::SetBoss(bool boss)
{
    bIsBoss = boss;
}

bool cDynamicCreature::IsKnockable()
{
    return bIsKnockable;
}

void cDynamicCreature::SetKnockable(bool knockable)
{
    bIsKnockable = knockable;
}

bool cDynamicCreature::IsDead()
{
    return bDead;
}

float cDynamicCreature::GetNormalizedW()
{
    return fNormalizedW;
}

float cDynamicCreature::GetNormalizedH()
{
    return fNormalizedH;
}

void cDynamicCreature::AlwaysGeneratePickUp(bool alwaysGenerate)
{
    bAlwaysGeneratePickUp = alwaysGenerate;
}

void cDynamicCreature::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
    // No default behaviour
}

void cDynamicCreature::ExplodeAndDie(float fElapsedTime)
{
    fDeadTimer += fElapsedTime;

    vx = 0.0f;
    vy = 0.0f;

    engine->ActivateShakeEffect(true);

    if (bBossExplosionAvailable)
    {
        engine->BossHitStop();
        float explosionWidth = 142.0f;
        float explosionHeight = 200.0f;

        float centerOfBossX = ((fDynWidth - explosionWidth) / 2.0f) / engine->GetTileWidth();
        float centerOfBossY = ((fDynHeight - explosionHeight) / 2.0f) / engine->GetTileHeight();
        engine->AddProjectile(px + centerOfBossX, py + centerOfBossY, true, 0.0f, 0.0f, cfExplosionDuration, "explosion", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
        engine->PlaySample("explosion");

        bBossExplosionAvailable = false;
    }

    if (fDeadTimer >= cfExplosionDuration)
    {
        nHealth = 0;
        bDead = true;
        engine->ActivateShakeEffect(false);

        engine->NukeAllEnnemies();
    }
}
