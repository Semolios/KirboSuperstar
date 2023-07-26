#include "Platformer_DynamicProjectile.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicProjectile::engine = nullptr;

cDynamicProjectile::cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Decal*> map, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, std::string spriteName, int corner, bool breackableAgainstTiles, float drag, bool bouncy, std::string bounceSound, bool scenery) : cDynamic("projectile")
{
	fDynWidth = map[0]->sprite->width;
	fDynHeight = map[0]->sprite->height;
	fDuration = duration;
	px = ox;
	py = oy;
	vx = velx;
	vy = vely;
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
	bBouncy = bouncy;
	bounceSoundEffect = bounceSound;
	bScenery = scenery;
}

cDynamicProjectile::~cDynamicProjectile()
{
	delete hitbox;
}

void cDynamicProjectile::DrawSelf(float ox, float oy)
{
	// check from which corner we draw the sprite
	float sprPosX = (nCornerSpr == 0 || nCornerSpr == 3) ? 0.0f : mapStates[nCurrentFrame]->sprite->width;
	float sprPosY = (nCornerSpr == 0 || nCornerSpr == 1) ? 0.0f : -mapStates[nCurrentFrame]->sprite->height;

	olc::vf2d pos;
	pos.x = ((px - ox) * 64.0f) + (fDynWidth  / 2.0f) + sprPosX;
	pos.y = ((py - oy) * 64.0f) + (fDynHeight / 2.0f) + sprPosY;
	olc::vf2d center;
	center.x = mapStates[nCurrentFrame]->sprite->width  / 2.0f;
	center.y = mapStates[nCurrentFrame]->sprite->height / 2.0f;
	engine->DrawRotatedDecal(pos, mapStates[nCurrentFrame], atan2f(vy, vx), center);
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

		fDynWidth = mapStates[nCurrentFrame]->sprite->width;
		fDynHeight = mapStates[nCurrentFrame]->sprite->height;
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

	if (bFriendly && !bScenery)
		CheckBreakableBlocks(level, fNewObjectPosX, fBorder, fNewObjectPosY);

	if (bSolidVsMap)
	{
		if (vx <= 0) // Moving Left
		{
			LeftCollision(level, fNewObjectPosX, fBorder);
		}
		else // Moving Right
		{
			RightCollision(level, fNewObjectPosX, fBorder);
		}

		if (vy <= 0) // Moving Up
		{
			TopCollision(level, fNewObjectPosX, fBorder, fNewObjectPosY);
		}
		else // Moving Down
		{
			BottomCollision(level, fNewObjectPosX, fBorder, fNewObjectPosY, fElapsedTime);
		}
	}

	px = fNewObjectPosX;
	py = fNewObjectPosY;
}

void cDynamicProjectile::SideCollision()
{
	if (bBreaksAgainstTiles)
		bRedundant = true;
	else if (!bBouncy)
		vx = 0.0f;
	else
		vx = -vx;
}

void cDynamicProjectile::LeftCollision(cLevel* level, float fNewObjectPosX, float fBorder)
{
	if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + fBorder, py + ((fDynHeight / engine->GetTileHeight()) / 2.0f))))
	{
		SideCollision();
	}
}

void cDynamicProjectile::RightCollision(cLevel* level, float fNewObjectPosX, float fBorder)
{
	if (engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), py + ((fDynHeight / engine->GetTileHeight()) / 2.0f))))
	{
		SideCollision();
	}
}

void cDynamicProjectile::TopCollision(cLevel* level, float fNewObjectPosX, float fBorder, float fNewObjectPosY)
{
	if (engine->IsSolidTile(level->GetTile(fNewObjectPosX +											fBorder,  fNewObjectPosY)) ||
		engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY)))
	{
		if (bBreaksAgainstTiles)
			bRedundant = true;
		else
			vy = 0.0f;
	}
}

void cDynamicProjectile::BottomCollision(cLevel* level, float fNewObjectPosX, float fBorder, float fNewObjectPosY, float fElapsedTime)
{
	if (engine->IsSolidTile(level->GetTile(fNewObjectPosX +											fBorder,  fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))) ||
		engine->IsSolidTile(level->GetTile(fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()))))
	{
		BottomCollisionBehaviour(fElapsedTime);
	}
	else
	{
		for (auto& ptfm : engine->GetClosePlatforms(px, py))
		{
			if (ptfm->TopCollision(fNewObjectPosX, fNewObjectPosX + (fDynWidth / engine->GetTileWidth()), fNewObjectPosY + (fDynHeight / engine->GetTileHeight())))
			{
				BottomCollisionBehaviour(fElapsedTime);
			}
		}
	}
}

void cDynamicProjectile::BottomCollisionBehaviour(float fElapsedTime)
{
	if (bBreaksAgainstTiles && !bBouncy)
	{
		bRedundant = true;
	}
	else
	{
		if (!bBouncy)
		{
			vy = 0.0f;

			vx += fDrag * vx * fElapsedTime;
			if (fabs(vx) < cfMinGlideVX) vx = 0.0f;
		}
		else
		{
			if (!bounceSoundEffect.empty())
				engine->PlaySample(bounceSoundEffect);

			vy = -vy;
		}
	}
}

void cDynamicProjectile::CheckBreakableBlocks(cLevel* level, float fNewObjectPosX, float fBorder, float fNewObjectPosY)
{
	//
	// x-----x
	// |     |
	// |     |
	// |     |
	// x-----x
	//
	BreakTile(level, fNewObjectPosX + fBorder, fNewObjectPosY +											 fBorder);
	BreakTile(level, fNewObjectPosX + fBorder, fNewObjectPosY + (fDynHeight / engine->GetTileHeight()) - fBorder);
	BreakTile(level, fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY +										  fBorder);
	BreakTile(level, fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()) - fBorder);
	//
	//  --x-- 
	// |     |
	// x     x
	// |     |
	//  --x-- 
	//
	BreakTile(level, fNewObjectPosX +										  fBorder,  fNewObjectPosY + ((fDynHeight / engine->GetTileHeight()) / 2.0f));
	BreakTile(level, fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) - fBorder), fNewObjectPosY + ((fDynHeight / engine->GetTileHeight()) / 2.0f));
	BreakTile(level, fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) / 2.0f), fNewObjectPosY +										   fBorder);
	BreakTile(level, fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) / 2.0f), fNewObjectPosY + (fDynHeight / engine->GetTileHeight()) - fBorder);
	//
	//  ----- 
	// |     |
	// |  x  |
	// |     |
	//  ----- 
	//
	BreakTile(level, fNewObjectPosX + ((fDynWidth / engine->GetTileWidth()) / 2.0f), fNewObjectPosY + ((fDynHeight / engine->GetTileHeight()) / 2.0f));
}

void cDynamicProjectile::BreakTile(cLevel* level, float tileX, float tileY)
{
	for (auto& t : crossedTiles)
	{
		std::vector<olc::vf2d>::iterator it;
		olc::vf2d tile((int)tileX, (int)tileY);
		it = find(crossedTiles.begin(), crossedTiles.end(), tile);
		if (it != crossedTiles.end())
			return;
	}

	if (level->GetTile(tileX, tileY) == L'H') ChangeTile(level, tileX, tileY, L'x');
	if (level->GetTile(tileX, tileY) == L'P') ChangeTile(level, tileX, tileY, L'H');
	if (level->GetTile(tileX, tileY) == L'B') ChangeTile(level, tileX, tileY, L'P');
}

void cDynamicProjectile::ChangeTile(cLevel* level, float tileX, float tileY, wchar_t t)
{
	level->SetTile(tileX, tileY, t);
	olc::vf2d tile((int)tileX, (int)tileY);
	crossedTiles.push_back(tile);
	PlaySoundEffect();
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
		engine->PlaySample(soundEffect);
}

void cDynamicProjectile::SpawnEffect(float posX, float posY)
{
	if (!effect.empty())
		engine->AddPreparedProjectile(posX, posY, true, 0.0f, 0.0f, fEffectDuration, effect, false, 0, false, true, 0, false, 0.0f, "", false, "", true);
}

std::map<std::string, std::vector<olc::Sprite*>> cDynamicProjectile::LoadProjectilesSprites()
{
	std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles;

	mapProjectiles["jesuscross"].push_back(new olc::Sprite("assets/gfx/jesuscross.png"));

	mapProjectiles["slapAOE"].push_back(new olc::Sprite("assets/gfx/slapAOE.png"));

	mapProjectiles["kirboFart"].push_back(new olc::Sprite("assets/gfx/kirboFart00.png"));
	mapProjectiles["kirboFart"].push_back(new olc::Sprite("assets/gfx/kirboFart01.png"));
	mapProjectiles["kirboFart"].push_back(new olc::Sprite("assets/gfx/kirboFart02.png"));
	mapProjectiles["kirboFart"].push_back(new olc::Sprite("assets/gfx/kirboFart03.png"));
	mapProjectiles["kirboFart"].push_back(new olc::Sprite("assets/gfx/kirboFart04.png"));

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

	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround00.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround01.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround02.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround03.png"));
	mapProjectiles["movingGround"].push_back(new olc::Sprite("assets/gfx/movingGround04.png"));

	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root00.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root01.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root02.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root03.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root04.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root05.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root06.png"));
	mapProjectiles["root"].push_back(new olc::Sprite("assets/gfx/root07.png"));

	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple00.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple01.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple02.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple03.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple04.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple05.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple06.png"));
	mapProjectiles["apple"].push_back(new olc::Sprite("assets/gfx/apple07.png"));

	mapProjectiles["blow"].push_back(new olc::Sprite("assets/gfx/blow.png"));

	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava00.png"));
	mapProjectiles["movingGroundLava"].push_back(new olc::Sprite("assets/gfx/movingGroundLava01.png"));

	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma00.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma01.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma02.png"));
	mapProjectiles["magma"].push_back(new olc::Sprite("assets/gfx/magma03.png"));

	mapProjectiles["magmaBoulder"].push_back(new olc::Sprite("assets/gfx/magmaBoulder.png"));

	mapProjectiles["stars"].push_back(new olc::Sprite("assets/gfx/stars.png"));

	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam00.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam01.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam02.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam03.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam04.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam05.png"));
	mapProjectiles["chargeBeam"].push_back(new olc::Sprite("assets/gfx/chargeBeam06.png"));

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

	mapProjectiles["swordAttack"].push_back(new olc::Sprite("assets/gfx/swordAttack.png"));

	mapProjectiles["tornado"].push_back(new olc::Sprite("assets/gfx/tornado00.png"));
	mapProjectiles["tornado"].push_back(new olc::Sprite("assets/gfx/tornado01.png"));
	mapProjectiles["tornado"].push_back(new olc::Sprite("assets/gfx/tornado02.png"));
	mapProjectiles["tornado"].push_back(new olc::Sprite("assets/gfx/tornado03.png"));
	mapProjectiles["tornado"].push_back(new olc::Sprite("assets/gfx/tornado04.png"));

	mapProjectiles["SSTierMKTPAttack"].push_back(new olc::Sprite("assets/gfx/SSTierMKTPAttack.png"));

	mapProjectiles["downTilt"].push_back(new olc::Sprite("assets/gfx/downTilt.png"));

	mapProjectiles["kingDDDUpSmash"].push_back(new olc::Sprite("assets/gfx/kingDDDUpSmash.png"));
	mapProjectiles["kingDDDLeftSideSmash"].push_back(new olc::Sprite("assets/gfx/kingDDDLeftSideSmash.png"));
	mapProjectiles["kingDDDRightSideSmash"].push_back(new olc::Sprite("assets/gfx/kingDDDRightSideSmash.png"));
	mapProjectiles["kingDDDDownSmash"].push_back(new olc::Sprite("assets/gfx/kingDDDDownSmash.png"));
	mapProjectiles["kingDDDForwardAir"].push_back(new olc::Sprite("assets/gfx/kingDDDForwardAir.png"));
	mapProjectiles["kingDDDBackwardAir"].push_back(new olc::Sprite("assets/gfx/kingDDDBackwardAir.png"));
	mapProjectiles["kingDDDDownAir"].push_back(new olc::Sprite("assets/gfx/kingDDDDownAir.png"));

	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike00.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike01.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike02.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike03.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike04.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike05.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike06.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike07.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike08.png"));
	mapProjectiles["spike"].push_back(new olc::Sprite("assets/gfx/spike09.png"));

	mapProjectiles["doorDebris1"].push_back(new olc::Sprite("assets/gfx/doorDebris1.png"));

	mapProjectiles["doorDebris2"].push_back(new olc::Sprite("assets/gfx/doorDebris2.png"));

	mapProjectiles["doorDebris3"].push_back(new olc::Sprite("assets/gfx/doorDebris3.png"));

	mapProjectiles["halberdCloud"].push_back(new olc::Sprite("assets/gfx/halberdCloud.png"));
	
	mapProjectiles["speedrunnerWahoo"].push_back(new olc::Sprite("assets/gfx/speedrunnerWahoo.png"));
	
	mapProjectiles["snow"].push_back(new olc::Sprite("assets/gfx/snow.png"));

	mapProjectiles["pickup"].push_back(new olc::Sprite("assets/gfx/pickup.png"));

	mapProjectiles["spawnWarning"].push_back(new olc::Sprite("assets/gfx/spawnWarning00.png"));
	mapProjectiles["spawnWarning"].push_back(new olc::Sprite("assets/gfx/spawnWarning01.png"));

	// Invisibles Projectiles (these ones are invisible because they are included in the ennemies animations
	mapProjectiles["SSTierMKHiyayaAOE"].push_back(new olc::Sprite("assets/gfx/SSTierMKHiyayaAOE.png"));
	mapProjectiles["kingDDDDownB"].push_back(new olc::Sprite("assets/gfx/kingDDDDownB.png"));
	mapProjectiles["kingDDDUpAir"].push_back(new olc::Sprite("assets/gfx/kingDDDUpAir.png"));

	return mapProjectiles;
}

std::map<std::string, std::vector<olc::Decal*>> cDynamicProjectile::LoadProjectilesDecals(std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles)
{
	std::map<std::string, std::vector<olc::Decal*>> mapDecProjectiles;

	mapDecProjectiles["jesuscross"].push_back(new olc::Decal(mapProjectiles["jesuscross"][0]));

	mapDecProjectiles["slapAOE"].push_back(new olc::Decal(mapProjectiles["slapAOE"][0]));

	mapDecProjectiles["kirboFart"].push_back(new olc::Decal(mapProjectiles["kirboFart"][0]));
	mapDecProjectiles["kirboFart"].push_back(new olc::Decal(mapProjectiles["kirboFart"][1]));
	mapDecProjectiles["kirboFart"].push_back(new olc::Decal(mapProjectiles["kirboFart"][2]));
	mapDecProjectiles["kirboFart"].push_back(new olc::Decal(mapProjectiles["kirboFart"][3]));
	mapDecProjectiles["kirboFart"].push_back(new olc::Decal(mapProjectiles["kirboFart"][4]));

	mapDecProjectiles["swordDownAOE"].push_back(new olc::Decal(mapProjectiles["swordDownAOE"][0]));
	mapDecProjectiles["swordDownAOE"].push_back(new olc::Decal(mapProjectiles["swordDownAOE"][1]));
	mapDecProjectiles["swordDownAOE"].push_back(new olc::Decal(mapProjectiles["swordDownAOE"][2]));

	mapDecProjectiles["swordUpAOE"].push_back(new olc::Decal(mapProjectiles["swordUpAOE"][0]));

	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][0]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][1]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][2]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][3]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][4]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][5]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][6]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][7]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][8]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][9]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][10]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][11]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][12]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][13]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][14]));
	mapDecProjectiles["explosion"].push_back(new olc::Decal(mapProjectiles["explosion"][15]));

	mapDecProjectiles["movingGround"].push_back(new olc::Decal(mapProjectiles["movingGround"][0]));
	mapDecProjectiles["movingGround"].push_back(new olc::Decal(mapProjectiles["movingGround"][1]));
	mapDecProjectiles["movingGround"].push_back(new olc::Decal(mapProjectiles["movingGround"][2]));
	mapDecProjectiles["movingGround"].push_back(new olc::Decal(mapProjectiles["movingGround"][3]));
	mapDecProjectiles["movingGround"].push_back(new olc::Decal(mapProjectiles["movingGround"][4]));

	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][0]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][2]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][3]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][4]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][5]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][6]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][7]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][4]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][3]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][2]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][1]));
	mapDecProjectiles["root"].push_back(new olc::Decal(mapProjectiles["root"][0]));

	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][0]));
	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][1]));
	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][2]));
	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][3]));
	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][4]));
	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][5]));
	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][6]));
	mapDecProjectiles["apple"].push_back(new olc::Decal(mapProjectiles["apple"][7]));

	mapDecProjectiles["blow"].push_back(new olc::Decal(mapProjectiles["blow"][0]));

	mapDecProjectiles["movingGroundLava"].push_back(new olc::Decal(mapProjectiles["movingGroundLava"][0]));
	mapDecProjectiles["movingGroundLava"].push_back(new olc::Decal(mapProjectiles["movingGroundLava"][0]));
	mapDecProjectiles["movingGroundLava"].push_back(new olc::Decal(mapProjectiles["movingGroundLava"][1]));
	mapDecProjectiles["movingGroundLava"].push_back(new olc::Decal(mapProjectiles["movingGroundLava"][1]));

	mapDecProjectiles["magma"].push_back(new olc::Decal(mapProjectiles["magma"][0]));
	mapDecProjectiles["magma"].push_back(new olc::Decal(mapProjectiles["magma"][1]));
	mapDecProjectiles["magma"].push_back(new olc::Decal(mapProjectiles["magma"][2]));
	mapDecProjectiles["magma"].push_back(new olc::Decal(mapProjectiles["magma"][3]));

	mapDecProjectiles["magmaBoulder"].push_back(new olc::Decal(mapProjectiles["magmaBoulder"][0]));

	mapDecProjectiles["stars"].push_back(new olc::Decal(mapProjectiles["stars"][0]));

	mapDecProjectiles["chargeBeam"].push_back(new olc::Decal(mapProjectiles["chargeBeam"][0]));
	mapDecProjectiles["chargeBeam"].push_back(new olc::Decal(mapProjectiles["chargeBeam"][1]));
	mapDecProjectiles["chargeBeam"].push_back(new olc::Decal(mapProjectiles["chargeBeam"][2]));
	mapDecProjectiles["chargeBeam"].push_back(new olc::Decal(mapProjectiles["chargeBeam"][3]));
	mapDecProjectiles["chargeBeam"].push_back(new olc::Decal(mapProjectiles["chargeBeam"][4]));
	mapDecProjectiles["chargeBeam"].push_back(new olc::Decal(mapProjectiles["chargeBeam"][5]));
	mapDecProjectiles["chargeBeam"].push_back(new olc::Decal(mapProjectiles["chargeBeam"][6]));

	mapDecProjectiles["beam"].push_back(new olc::Decal(mapProjectiles["beam"][0]));
	mapDecProjectiles["beam"].push_back(new olc::Decal(mapProjectiles["beam"][1]));

	mapDecProjectiles["tinyIceCube"].push_back(new olc::Decal(mapProjectiles["tinyIceCube"][0]));
	mapDecProjectiles["mediumIceCube"].push_back(new olc::Decal(mapProjectiles["mediumIceCube"][0]));
	mapDecProjectiles["bigIceCube"].push_back(new olc::Decal(mapProjectiles["bigIceCube"][0]));
	mapDecProjectiles["hugeIceCubeLeft"].push_back(new olc::Decal(mapProjectiles["hugeIceCubeLeft"][0]));
	mapDecProjectiles["hugeIceCubeRight"].push_back(new olc::Decal(mapProjectiles["hugeIceCubeRight"][0]));

	mapDecProjectiles["frostyWind"].push_back(new olc::Decal(mapProjectiles["frostyWind"][0]));
	mapDecProjectiles["frostyWind"].push_back(new olc::Decal(mapProjectiles["frostyWind"][1]));
	mapDecProjectiles["frostyWind"].push_back(new olc::Decal(mapProjectiles["frostyWind"][2]));
	mapDecProjectiles["frostyWind"].push_back(new olc::Decal(mapProjectiles["frostyWind"][3]));

	mapDecProjectiles["thunderShot"].push_back(new olc::Decal(mapProjectiles["thunderShot"][0]));
	mapDecProjectiles["thunderShot"].push_back(new olc::Decal(mapProjectiles["thunderShot"][1]));

	mapDecProjectiles["thunderUnder"].push_back(new olc::Decal(mapProjectiles["thunderUnder"][0]));

	mapDecProjectiles["aim"].push_back(new olc::Decal(mapProjectiles["aim"][0]));

	mapDecProjectiles["lightning"].push_back(new olc::Decal(mapProjectiles["lightning"][0]));

	mapDecProjectiles["chargeLightning"].push_back(new olc::Decal(mapProjectiles["chargeLightning"][0]));
	mapDecProjectiles["chargeLightning"].push_back(new olc::Decal(mapProjectiles["chargeLightning"][1]));
	mapDecProjectiles["chargeLightning"].push_back(new olc::Decal(mapProjectiles["chargeLightning"][2]));
	mapDecProjectiles["chargeLightning"].push_back(new olc::Decal(mapProjectiles["chargeLightning"][3]));

	mapDecProjectiles["lightningAround"].push_back(new olc::Decal(mapProjectiles["lightningAround"][0]));
	mapDecProjectiles["lightningAround"].push_back(new olc::Decal(mapProjectiles["lightningAround"][1]));
	mapDecProjectiles["lightningAround"].push_back(new olc::Decal(mapProjectiles["lightningAround"][2]));

	mapDecProjectiles["swordAttack"].push_back(new olc::Decal(mapProjectiles["swordAttack"][0]));

	mapDecProjectiles["tornado"].push_back(new olc::Decal(mapProjectiles["tornado"][0]));
	mapDecProjectiles["tornado"].push_back(new olc::Decal(mapProjectiles["tornado"][1]));
	mapDecProjectiles["tornado"].push_back(new olc::Decal(mapProjectiles["tornado"][2]));
	mapDecProjectiles["tornado"].push_back(new olc::Decal(mapProjectiles["tornado"][3]));
	mapDecProjectiles["tornado"].push_back(new olc::Decal(mapProjectiles["tornado"][4]));

	mapDecProjectiles["SSTierMKTPAttack"].push_back(new olc::Decal(mapProjectiles["SSTierMKTPAttack"][0]));

	mapDecProjectiles["downTilt"].push_back(new olc::Decal(mapProjectiles["downTilt"][0]));

	mapDecProjectiles["kingDDDUpSmash"].push_back(new olc::Decal(mapProjectiles["kingDDDUpSmash"][0]));
	mapDecProjectiles["kingDDDLeftSideSmash"].push_back(new olc::Decal(mapProjectiles["kingDDDLeftSideSmash"][0]));
	mapDecProjectiles["kingDDDRightSideSmash"].push_back(new olc::Decal(mapProjectiles["kingDDDRightSideSmash"][0]));
	mapDecProjectiles["kingDDDDownSmash"].push_back(new olc::Decal(mapProjectiles["kingDDDDownSmash"][0]));
	mapDecProjectiles["kingDDDForwardAir"].push_back(new olc::Decal(mapProjectiles["kingDDDForwardAir"][0]));
	mapDecProjectiles["kingDDDBackwardAir"].push_back(new olc::Decal(mapProjectiles["kingDDDBackwardAir"][0]));
	mapDecProjectiles["kingDDDDownAir"].push_back(new olc::Decal(mapProjectiles["kingDDDDownAir"][0]));

	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][0]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][1]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][2]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][3]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][4]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][5]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][6]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][7]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][8]));
	mapDecProjectiles["spike"].push_back(new olc::Decal(mapProjectiles["spike"][9]));

	mapDecProjectiles["doorDebris1"].push_back(new olc::Decal(mapProjectiles["doorDebris1"][0]));

	mapDecProjectiles["doorDebris2"].push_back(new olc::Decal(mapProjectiles["doorDebris2"][0]));

	mapDecProjectiles["doorDebris3"].push_back(new olc::Decal(mapProjectiles["doorDebris3"][0]));

	mapDecProjectiles["halberdCloud"].push_back(new olc::Decal(mapProjectiles["halberdCloud"][0]));

	mapDecProjectiles["speedrunnerWahoo"].push_back(new olc::Decal(mapProjectiles["speedrunnerWahoo"][0]));

	mapDecProjectiles["snow"].push_back(new olc::Decal(mapProjectiles["snow"][0]));

	mapDecProjectiles["pickup"].push_back(new olc::Decal(mapProjectiles["pickup"][0]));

	mapDecProjectiles["spawnWarning"].push_back(new olc::Decal(mapProjectiles["spawnWarning"][0]));
	mapDecProjectiles["spawnWarning"].push_back(new olc::Decal(mapProjectiles["spawnWarning"][0]));
	mapDecProjectiles["spawnWarning"].push_back(new olc::Decal(mapProjectiles["spawnWarning"][0]));
	mapDecProjectiles["spawnWarning"].push_back(new olc::Decal(mapProjectiles["spawnWarning"][1]));
	mapDecProjectiles["spawnWarning"].push_back(new olc::Decal(mapProjectiles["spawnWarning"][1]));
	mapDecProjectiles["spawnWarning"].push_back(new olc::Decal(mapProjectiles["spawnWarning"][1]));

	// Invisibles Projectiles (these ones are invisible because they are included in the ennemies animations
	mapDecProjectiles["SSTierMKHiyayaAOE"].push_back(new olc::Decal(mapProjectiles["SSTierMKHiyayaAOE"][0]));
	mapDecProjectiles["kingDDDDownB"].push_back(new olc::Decal(mapProjectiles["kingDDDDownB"][0]));
	mapDecProjectiles["kingDDDUpAir"].push_back(new olc::Decal(mapProjectiles["kingDDDUpAir"][0]));

	return mapDecProjectiles;
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

void cDynamicProjectile::SetEffect(std::string scn, float duration)
{
	effect = scn;
	fEffectDuration = duration;
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

bool cDynamicProjectile::IsScenery()
{
	return bScenery;
}

float cDynamicProjectile::GetNormalizedW()
{
	return fDynWidth / engine->GetTileWidth();
}

float cDynamicProjectile::GetNormalizedH()
{
	return fDynHeight / engine->GetTileHeight();
}

void cDynamicProjectile::SetPickable(bool pickable)
{
	bPickable = pickable;
}

bool cDynamicProjectile::IsPickable()
{
	return bPickable;
}

void cDynamicProjectile::UpdateTrajectory(float fElapsedTime)
{
	// Standard projectile has no particular trajectory, either straight or bell curved
}
