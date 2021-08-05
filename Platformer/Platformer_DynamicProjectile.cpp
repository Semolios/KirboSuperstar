#include "Platformer_DynamicProjectile.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicProjectile::engine = nullptr;

cDynamicProjectile::cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner, bool breackableAgainstTiles, float drag) : cDynamic("projectile")
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
	bBreaksAgainstTiles = breackableAgainstTiles;
	bAffectedByGravity = affectedByGravity;
	bFriendly = bFriend;
	mapStates = map;
	nDamage = damage;
	bSolidVsMap = solidVSMap;
	bOneHit = oneHit;
	nCornerSpr = corner % 4;
	fDrag = drag;
}

cDynamicProjectile::~cDynamicProjectile()
{
	delete hitbox;
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

	UpdateTrajectory(fElapsedTime);
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
				if (bBreaksAgainstTiles)
					bRedundant = true;
				else
					vx = 0.0f;
			}
		}
		else // Moving Right
		{
			if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + fBorder)) ||
				engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + (fDynHeight / engine->GetTileHeight()) - fBorder)))
			{
				if (bBreaksAgainstTiles)
					bRedundant = true;
				else
					vx = 0.0f;
			}
		}

		if (vy <= 0) // Moving Up
		{
			if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY)) ||
				engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY)))
			{
				if (bBreaksAgainstTiles)
					bRedundant = true;
				else
					vy = 0.0f;
			}
		}
		else // Moving Down
		{
			if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSemiSolidTile(level->GetTile(fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
				engine->IsSemiSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))))
			{
				if (bBreaksAgainstTiles)
				{
					bRedundant = true;
				}
				else
				{
					vy = 0.0f;

					vx += fDrag * vx * fElapsedTime;
					if (fabs(vx) < cfMinGlideVX) vx = 0.0f;
				}
			}
		}
	}

	px = fNewObjectPosX;
	py = fNewObjectPosY;
}

void cDynamicProjectile::UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
{
	// check from which corner we draw the sprite
	float sprPosX = (nCornerSpr == 0 || nCornerSpr == 3) ? 0.0f : fDynWidth;
	float sprPosY = (nCornerSpr == 0 || nCornerSpr == 1) ? 0.0f : -fDynHeight;

	hitbox->SetPos(
		(px - cameraOffsetX) * engine->GetTileWidth() + (fDynWidth / 2.0f) + sprPosX,
		(py - cameraOffsetY) * engine->GetTileHeight() + (fDynHeight / 2.0f) + sprPosY
	);
	hitbox->SetAngle(atan2f(vy, vx));
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

	hitbox->ClearO();

	// debug AOE
	//hitbox->Draw(engine, olc::RED);
}

void cDynamicProjectile::PlaySoundEffect()
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

	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam00.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam01.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam02.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam03.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam04.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam05.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam06.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam07.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam08.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam09.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam10.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam11.png"));

	mapProjectiles["beam"].push_back(new olc::Sprite("assets/gfx/beam00.png"));
	mapProjectiles["beam"].push_back(new olc::Sprite("assets/gfx/beam01.png"));

	mapProjectiles["tinyIceCube"].push_back(new olc::Sprite("assets/gfx/tinyIceCube.png"));
	mapProjectiles["mediumIceCube"].push_back(new olc::Sprite("assets/gfx/mediumIceCube.png"));
	mapProjectiles["bigIceCube"].push_back(new olc::Sprite("assets/gfx/bigIceCube.png"));
	mapProjectiles["hugeIceCubeLeft"].push_back(new olc::Sprite("assets/gfx/hugeIceCubeLeft.png"));
	mapProjectiles["hugeIceCubeRight"].push_back(new olc::Sprite("assets/gfx/hugeIceCubeRight.png"));

	mapProjectiles["frostyWind"].push_back(new olc::Sprite("assets/gfx/frostyWind00.png"));
	mapProjectiles["frostyWind"].push_back(new olc::Sprite("assets/gfx/frostyWind01.png"));
	mapProjectiles["frostyWind"].push_back(new olc::Sprite("assets/gfx/frostyWind02.png"));
	mapProjectiles["frostyWind"].push_back(new olc::Sprite("assets/gfx/frostyWind03.png"));

	mapProjectiles["thunderShot"].push_back(new olc::Sprite("assets/gfx/thunderShot00.png"));
	mapProjectiles["thunderShot"].push_back(new olc::Sprite("assets/gfx/thunderShot01.png"));

	mapProjectiles["thunderUnder"].push_back(new olc::Sprite("assets/gfx/thunderUnder.png"));

	mapProjectiles["aim"].push_back(new olc::Sprite("assets/gfx/aim.png"));

	mapProjectiles["lightning"].push_back(new olc::Sprite("assets/gfx/lightning.png"));

	mapProjectiles["chargeLightning"].push_back(new olc::Sprite("assets/gfx/chargeLightning00.png"));
	mapProjectiles["chargeLightning"].push_back(new olc::Sprite("assets/gfx/chargeLightning01.png"));
	mapProjectiles["chargeLightning"].push_back(new olc::Sprite("assets/gfx/chargeLightning02.png"));
	mapProjectiles["chargeLightning"].push_back(new olc::Sprite("assets/gfx/chargeLightning03.png"));

	mapProjectiles["lightningAround"].push_back(new olc::Sprite("assets/gfx/lightningAround00.png"));
	mapProjectiles["lightningAround"].push_back(new olc::Sprite("assets/gfx/lightningAround01.png"));
	mapProjectiles["lightningAround"].push_back(new olc::Sprite("assets/gfx/lightningAround02.png"));

	return mapProjectiles;
}

bool cDynamicProjectile::IsOneHit()
{
	return bOneHit;
}

void cDynamicProjectile::SetOneHit(bool oneHit)
{
	bOneHit = oneHit;
}

std::string cDynamicProjectile::GetSoundEffect()
{
	return soundEffect;
}

void cDynamicProjectile::SetSoundEffect(std::string sndEffect)
{
	soundEffect = sndEffect;
}

bool cDynamicProjectile::IsRedundant()
{
	return bRedundant;
}

void cDynamicProjectile::SetRedundant(bool redundant)
{
	bRedundant = redundant;
}

bool cDynamicProjectile::BreaksAgainstTiles()
{
	return bBreaksAgainstTiles;
}

void cDynamicProjectile::SetBreakableAgainstTiles(bool breakable)
{
	bBreaksAgainstTiles = breakable;
}

void cDynamicProjectile::SetDrag(float drag)
{
	fDrag = drag;
}

void cDynamicProjectile::UpdateTrajectory(float fElapsedTime)
{
	// Standard projectile has no particular trajectory, either straight or bell curved
}
