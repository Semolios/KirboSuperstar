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

void cDynamicProjectile::DrawSelf(float ox, float oy)
{
	// check from which corner we draw the sprite
	float sprPosX = (nCornerSpr == 0 || nCornerSpr == 3) ? 0.0f : mapStates[nCurrentFrame]->width;
	float sprPosY = (nCornerSpr == 0 || nCornerSpr == 1) ? 0.0f : -mapStates[nCurrentFrame]->height;

	engine->SetPixelMode(olc::Pixel::ALPHA);
	olc::GFX2D::Transform2D t;
	t.Translate(-fDynWidth / 2.0f, -fDynHeight / 2.0f);
	t.Rotate(atan2f(-vy, vx));
	t.Translate((px - ox + ((fDynWidth / 64.0f) / 2.0f)) * 64.0f + sprPosX, (py - oy + ((fDynHeight / 64.0f) / 2.0f)) * 64.0f + sprPosY);
	olc::GFX2D::DrawSprite(mapStates[nCurrentFrame], t);
	engine->SetPixelMode(olc::Pixel::NORMAL);
}

void cDynamicProjectile::Update(float fElapsedTime, float playerX, float playerY)
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

void cDynamicProjectile::Collision(float fElapsedTime, cLevel* level)
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
			if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + fBorder, py + fBorder)) ||
				engine->IsSolidTile(level->GetTile(fNewObjectPosX + fBorder, py + (fDynHeight / engine->GetTileHeight()) - fBorder)))
			{
				bRedundant = true;
			}
		}
		else // Moving Right
		{
			if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + fBorder)) ||
				engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + (fDynHeight / engine->GetTileHeight()) - fBorder)))
			{
				bRedundant = true;
			}
		}

		if (vy <= 0) // Moving Up
		{
			if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY)) ||
				engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY)))
			{
				bRedundant = true;
			}
		}
		else // Moving Down
		{
			if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSemiSolidTile(level->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSemiSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))))
			{
				bRedundant = true;
			}
		}
	}

	px = fNewObjectPosX;
	py = fNewObjectPosY;
}

cHitbox cDynamicProjectile::Hitbox(float cameraOffsetX, float cameraOffsetY)
{
	cHitbox sAOE;

	// check from which corner we draw the sprite
	float sprPosX = (nCornerSpr == 0 || nCornerSpr == 3) ? 0.0f : fDynWidth;
	float sprPosY = (nCornerSpr == 0 || nCornerSpr == 1) ? 0.0f : -fDynHeight;

	sAOE.pos = {
		(px - cameraOffsetX) * engine->GetTileWidth() + (fDynWidth / 2.0f) + sprPosX,
		(py - cameraOffsetY) * engine->GetTileHeight() + (fDynHeight / 2.0f) + sprPosY
	};
	sAOE.angle = atan2f(vy, vx);
	sAOE.o.push_back({ -fDynWidth / 2.0f, -fDynHeight / 2.0f });
	sAOE.o.push_back({ -fDynWidth / 2.0f, +fDynHeight / 2.0f });
	sAOE.o.push_back({ +fDynWidth / 2.0f, +fDynHeight / 2.0f });
	sAOE.o.push_back({ +fDynWidth / 2.0f, -fDynHeight / 2.0f });
	sAOE.p.resize(4);

	for (int i = 0; i < sAOE.o.size(); i++)
	{
		sAOE.p[i] =
		{	// 2D Rotation Transform + 2D Translation
			(sAOE.o[i].x * cosf(sAOE.angle)) - (sAOE.o[i].y * sinf(sAOE.angle)) + sAOE.pos.x,
			(sAOE.o[i].x * sinf(sAOE.angle)) + (sAOE.o[i].y * cosf(sAOE.angle)) + sAOE.pos.y,
		};
	}

	// debug AOE
	//sAOE.Draw(engine, olc::RED);

	return sAOE;
}

void cDynamicProjectile::SoundEffect()
{
	if (!soundEffect.empty())
		olc::SOUND::PlaySample(engine->GetSound(soundEffect));
}

std::map<std::string, std::vector<olc::Sprite*>> cDynamicProjectile::LoadProjectilesSprites()
{
	std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles;
	mapProjectiles["jesuscross"].push_back(new olc::Sprite("assets/gfx/jesuscross.png"));

	mapProjectiles["slapAOE"].push_back(new olc::Sprite("assets/gfx/slapAOE.png"));

	mapProjectiles["swordDownAOE"].push_back(new olc::Sprite("assets/gfx/swordDownAOE00.png"));
	mapProjectiles["swordDownAOE"].push_back(new olc::Sprite("assets/gfx/swordDownAOE01.png"));
	mapProjectiles["swordDownAOE"].push_back(new olc::Sprite("assets/gfx/swordDownAOE02.png"));

	mapProjectiles["swordUpAOE"].push_back(new olc::Sprite("assets/gfx/swordUpAOE.png"));

	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion00.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion01.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion02.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion03.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion04.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion05.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion06.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion07.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion08.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion09.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion10.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion11.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion12.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion13.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion14.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion15.png"));
	mapProjectiles["explosion"].push_back(new olc::Sprite("assets/gfx/bossExplosion16.png"));

	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround00.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround01.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround02.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround03.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround04.png"));

	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root00.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root02.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root03.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root04.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root05.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root06.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root07.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root04.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root03.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root02.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root01.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root00.png"));

	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple00.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple01.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple02.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple03.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple04.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple05.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple06.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple07.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple08.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple09.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple10.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple11.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple12.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple13.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple14.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple15.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple16.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple17.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple18.png"));

	mapProjectiles["blow"].push_back(new olc::Sprite("assets/gfx/blow.png"));

	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava00.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava01.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava02.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava03.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava04.png"));

	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma00.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma01.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma02.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma03.png"));

	mapProjectiles["magmaBoulder"].push_back(new olc::Sprite("assets/gfx/magmaBoulder.png"));

	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser00.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser01.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser02.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser03.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser04.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser05.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser06.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser07.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser08.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser09.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser10.png"));
	mapProjectiles["chargeLaser"].push_back(new olc::Sprite("assets/gfx/chargeLaser11.png"));

	mapProjectiles["laser"].push_back(new olc::Sprite("assets/gfx/laser00.png"));
	mapProjectiles["laser"].push_back(new olc::Sprite("assets/gfx/laser01.png"));

	return mapProjectiles;
}
