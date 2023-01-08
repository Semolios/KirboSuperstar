#include "Platformer_DynamicCreature.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreature::engine = nullptr;

cDynamicCreature::cDynamicCreature(std::string n, olc::Sprite* sprite, int framesPerSecond) : cDynamic(n)
{
    sSprite = sprite;
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
    delete sSprite;
    delete level;
}

void cDynamicCreature::DrawSelf(float ox, float oy)
{
    int nSheetOffsetX = nGraphicCounter * fSpriteW;					// Same State of a sprite are stored in one line
    int nSheetOffsetY = (2 * nGraphicState + nFaceDir) * fSpriteH;	// 0 = Left Idle, 1 = Right Idle, 2 = Left Walking, 3 = Right Walking, 4 = Left Damaged, 5 = Right Damaged ...

    engine->SetPixelMode(olc::Pixel::ALPHA);
    engine->DrawPartialSprite((px - ox) * 64.0f + fSpriteOffsetX, (py - oy) * 64.0f + fSpriteOffsetY, sSprite, nSheetOffsetX, nSheetOffsetY, fSpriteW, fSpriteH);
    engine->SetPixelMode(olc::Pixel::NORMAL);
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
        else
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

    float fNormalizedW = fDynWidth / engine->GetTileWidth();
    float fNormalizedH = fDynHeight / engine->GetTileHeight();

    // Collision
    float fGap = 0.1f;

    float fLCollision = fNewPosX + fGap;
    float fRCollision = fNewPosX + fNormalizedW - fGap;

    // Gravity
    if (bAffectedByGravity)
        vy += engine->GetGravityValue() * fElapsedTime;

    if (bSolidVsMap)
    {
        if (vx <= 0) // Moving Left
        {
            if (engine->IsSolidTile(level->GetTile(fLCollision, py +                fGap)) ||
                engine->IsSolidTile(level->GetTile(fLCollision, py + fNormalizedH - fGap)))
            {
                fNewPosX = (int)fNewPosX + 1;
                vx = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->RightCollision(fNewPosY, fNewPosY + fNormalizedH, fLCollision) ||
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
            if (engine->IsSolidTile(level->GetTile(fRCollision, py +                fGap)) ||
                engine->IsSolidTile(level->GetTile(fRCollision, py + fNormalizedH - fGap)))
            {
                fNewPosX = (int)fNewPosX;
                vx = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->LeftCollision(fNewPosY, fNewPosY + fNormalizedH, fRCollision) ||
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
            if (engine->IsSolidTile(level->GetTile(fLCollision, fNewPosY)) ||
                engine->IsSolidTile(level->GetTile(fRCollision, fNewPosY)))
            {
                fNewPosY = (int)fNewPosY + 1;
                vy = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->BotCollision(fLCollision, fRCollision, fNewPosY) ||
                     ptfm->BotCollisionWithLag(fLCollision, fRCollision, py, fNewPosY)) &&
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
            if (engine->IsSolidTile(level->GetTile(fLCollision, fNewPosY + fNormalizedH)) ||
                engine->IsSolidTile(level->GetTile(fRCollision, fNewPosY + fNormalizedH)) ||
                ((engine->IsSemiSolidTile(level->GetTile(fLCollision, fNewPosY + fNormalizedH)) ||
                  engine->IsSemiSolidTile(level->GetTile(fRCollision, fNewPosY + fNormalizedH))) && py + fDynHeight < (float)((int)fNewPosY + fDynHeight) + fSemiSolidThickness))
            {
                fNewPosY = (int)fNewPosY + engine->GetGroundDynamicOverlay();
                vy = 0;
            }

            // Moving platforms collision
            for (auto& ptfm : engine->GetClosePlatforms(fNewPosX, fNewPosY))
            {
                if ((ptfm->TopCollision(fLCollision, fRCollision, fNewPosY + fNormalizedH) ||
                     ptfm->TopCollisionWithLag(fLCollision, fRCollision, py + fNormalizedH, fNewPosY + fNormalizedH)) &&
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
    hitbox->Draw(engine, olc::YELLOW);
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

        float centerOfBossX = ((fDynWidth  - explosionWidth)  / 2.0f) / engine->GetTileWidth();
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
    }
}
