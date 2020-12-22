#include "Platformer_DynamicProjectile.h"

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
