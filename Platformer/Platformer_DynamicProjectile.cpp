#include "Platformer_DynamicProjectile.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicProjectile::engine = nullptr;

cDynamicProjectile::cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner) : cDynamic("projectile")
{
	fDynWidth = map[0]->width;
	fDynHeight = map[0]->height;
	fDuration = duration;
	px = ox;
	py = oy;
	vx = velx;
	vy = vely;
	bSolidVsDyn = false;
	bSolidVsMap = true;
	bIsProjectile = true;
	bIsAttackable = false;
	bAffectedByGravity = affectedByGravity;
	bFriendly = bFriend;
	mapStates = map;
	nDamage = damage;
	bSolidVsMap = solidVSMap;
	bOneHit = oneHit;
	nCornerSpr = corner % 4;
}

void cDynamicProjectile::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	// check from which corner we draw the sprite
	float sprPosX = (nCornerSpr == 0 || nCornerSpr == 3) ? 0.0f : mapStates[nCurrentFrame]->width;
	float sprPosY = (nCornerSpr == 0 || nCornerSpr == 1) ? 0.0f : -mapStates[nCurrentFrame]->height;

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	olc::GFX2D::Transform2D t;
	t.Translate(-fDynWidth / 2.0f, -fDynHeight / 2.0f);
	t.Rotate(atan2f(-vy, vx));
	t.Translate((px - ox + ((fDynWidth / 64.0f) / 2.0f)) * 64.0f + sprPosX, (py - oy + ((fDynHeight / 64.0f) / 2.0f)) * 64.0f + sprPosY);
	olc::GFX2D::DrawSprite(mapStates[nCurrentFrame], t);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cDynamicProjectile::Update(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	fTimeCounter += fElapsedTime;
	if (fTimeCounter >= fTimeBetweenFrames)
	{
		fTimeCounter -= fTimeBetweenFrames;
		nCurrentFrame++;
		if (nCurrentFrame >= mapStates.size())
			nCurrentFrame = 0;

		fDynWidth = mapStates[nCurrentFrame]->width;
		fDynHeight = mapStates[nCurrentFrame]->height;
	}

	fDuration -= fElapsedTime;
	if (fDuration <= 0.0f)
		bRedundant = true;
}

void cDynamicProjectile::Collision(float fElapsedTime)
{
	float fNewObjectPosX = px + vx * fElapsedTime;
	float fNewObjectPosY = py + vy * fElapsedTime;

	// Collision
	float fBorder = 0.1f;

	// Gravity
	if (bAffectedByGravity)
		vy += engine->GetGravityValue() * fElapsedTime;

	if (bSolidVsMap)
	{
		if (vx <= 0) // Moving Left
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, py + fBorder)) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, py + (fDynHeight / engine->GetTileHeight()) - fBorder)))
			{
				bRedundant = true;
			}
		}
		else // Moving Right
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + fBorder)) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + (fDynHeight / engine->GetTileHeight()) - fBorder)))
			{
				bRedundant = true;
			}
		}

		if (vy <= 0) // Moving Up
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY)) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY)))
			{
				bRedundant = true;
			}
		}
		else // Moving Down
		{
			if (engine->IsSolidTile(engine->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) || 
				engine->IsSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSemiSolidTile(engine->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) || 
				engine->IsSemiSolidTile(engine->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))))
			{
				bRedundant = true;
			}
		}
	}

	px = fNewObjectPosX;
	py = fNewObjectPosY;
}
