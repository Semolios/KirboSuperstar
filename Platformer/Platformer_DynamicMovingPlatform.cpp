#include "Platformer_DynamicMovingPlatform.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicMovingPlatform::engine = nullptr;

cDynamicMovingPlatform::cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Decal*> map, std::wstring iced) : cDynamic("movingPlatform")
{
	px = ox;
	py = oy;
	mapStates = map;
	bFriendly = true;
	bIced = (iced == L"1");
}

cDynamicMovingPlatform::cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Decal*> map) : cDynamic("scenery")
{
	px = ox;
	py = oy;
	mapStates = map;
	bFriendly = true;
	bSolidTop = false;
}

cDynamicMovingPlatform::~cDynamicMovingPlatform()
{
	delete hitbox;
}

void cDynamicMovingPlatform::DrawSwitch(float cameraX, float cameraY)
{
	// Only used for walls
}

void cDynamicMovingPlatform::DrawSelf(float ox, float oy)
{
    olc::vf2d pos;
	pos.x = (px - ox + (GetNormalizedWidth()  / 2.0f)) * engine->GetTileWidth()  - fDynWidth  / 2.0f;
	pos.y = (py - oy + (GetNormalizedHeight() / 2.0f)) * engine->GetTileHeight() - fDynHeight / 2.0f;
	engine->DrawDecal(pos, mapStates[nCurrentFrame]);
}

void cDynamicMovingPlatform::Update(float fElapsedTime, float playerX, float playerY)
{
	fPrevPosX = px;
	fPrevPosY = py;

	px += vx * fElapsedTime;
	py += vy * fElapsedTime;

	fTimeCounter += fElapsedTime;
	if (fTimeCounter >= fTimeBetweenFrames)
	{
		fTimeCounter -= fTimeBetweenFrames;
		nCurrentFrame++;
		if (nCurrentFrame >= mapStates.size())
			nCurrentFrame = 0;

		fDynWidth  = mapStates[nCurrentFrame]->sprite->width;
		fDynHeight = mapStates[nCurrentFrame]->sprite->height;
	}

	Behaviour(fElapsedTime, playerX, playerY);
}

void cDynamicMovingPlatform::UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
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

	hitbox->ClearO();

	// debug AOE
	//hitbox->Draw(engine, olc::MAGENTA);
}

bool cDynamicMovingPlatform::TopCollision(float leftCornerX, float rightCornerX, float bottomSpriteY)
{
	olc::vf2d playerLeftCorner(leftCornerX, bottomSpriteY);
	olc::vf2d playerRightCorner(rightCornerX, bottomSpriteY);

	olc::vf2d ptfmTopLeftCorner(px, py - fHitboxThickness);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()), py + fHitboxThickness);

	return bSolidTop && (
		((playerLeftCorner.x  >= ptfmTopLeftCorner.x) && (playerLeftCorner.x  <= ptfmBotRightCorner.x) && (playerLeftCorner.y  >= ptfmTopLeftCorner.y) && (playerLeftCorner.y  <= ptfmBotRightCorner.y)) ||
		((playerRightCorner.x >= ptfmTopLeftCorner.x) && (playerRightCorner.x <= ptfmBotRightCorner.x) && (playerRightCorner.y >= ptfmTopLeftCorner.y) && (playerRightCorner.y <= ptfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::TopCollisionWithLag(float leftCornerX, float rightCornerX, float fPlayerPosY, float fNewPlayerPosY)
{
	olc::vf2d ptfmTopLeftCorner(px, py - fHitboxThickness);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()), py + fHitboxThickness);

	olc::vf2d prevPtfmTopLeftCorner(fPrevPosX, fPrevPosY - fHitboxThickness);
	olc::vf2d prevPtfmBotRightCorner(fPrevPosX + (fDynWidth / (float)engine->GetTileWidth()), fPrevPosY + fHitboxThickness);

	return bSolidTop && (
		// playerPY is above platform and newPlayerPY is below platform
		((leftCornerX  >= ptfmTopLeftCorner.x) && (leftCornerX  <= ptfmBotRightCorner.x) && (fPlayerPosY <= ptfmTopLeftCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY <= ptfmTopLeftCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		// playerPY is in the platform and newPlayerPY is below platform
		((leftCornerX  >= ptfmTopLeftCorner.x) && (leftCornerX  <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		// playerPY is in the Previous platform position
		((leftCornerX  >= prevPtfmTopLeftCorner.x) && (leftCornerX  <= prevPtfmBotRightCorner.x) && (fPlayerPosY >= prevPtfmTopLeftCorner.y) && (fPlayerPosY <= prevPtfmBotRightCorner.y)) ||
		((rightCornerX >= prevPtfmTopLeftCorner.x) && (rightCornerX <= prevPtfmBotRightCorner.x) && (fPlayerPosY >= prevPtfmTopLeftCorner.y) && (fPlayerPosY <= prevPtfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::TopCollisionOneCorner(float CornerX, float cornerY)
{
	olc::vf2d dynCorner(CornerX, cornerY);

	olc::vf2d ptfmTopLeftCorner(px, py - fHitboxThickness);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()), py + fHitboxThickness);

	return (dynCorner.x >= ptfmTopLeftCorner.x) && (dynCorner.x <= ptfmBotRightCorner.x) && (dynCorner.y >= ptfmTopLeftCorner.y) && (dynCorner.y <= ptfmBotRightCorner.y);
}

bool cDynamicMovingPlatform::LeftCollision(float topCornerY, float botCornerY, float rightSpriteX)
{
	olc::vf2d playerTopCorner(rightSpriteX, topCornerY);
	olc::vf2d playerBotCorner(rightSpriteX, botCornerY);

	olc::vf2d ptfmTopLeftCorner(px - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	return bSolidLeft && (
		((playerTopCorner.x >= ptfmTopLeftCorner.x) && (playerTopCorner.x <= ptfmBotRightCorner.x) && (playerTopCorner.y >= ptfmTopLeftCorner.y) && (playerTopCorner.y <= ptfmBotRightCorner.y)) ||
		((playerBotCorner.x >= ptfmTopLeftCorner.x) && (playerBotCorner.x <= ptfmBotRightCorner.x) && (playerBotCorner.y >= ptfmTopLeftCorner.y) && (playerBotCorner.y <= ptfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::LeftCollisionWithLag(float topCornerY, float botCornerY, float fPlayerPosX, float fNewPlayerPosX)
{
	olc::vf2d ptfmTopLeftCorner(px - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	olc::vf2d prevPtfmTopLeftCorner(fPrevPosX - fHitboxThickness, fPrevPosY);
	olc::vf2d prevPtfmBotRightCorner(fPrevPosX + fHitboxThickness, fPrevPosY + (fDynHeight / (float)engine->GetTileHeight()));

	return bSolidLeft && (
		// playerPX is left to the platform and newPlayerPY is right to the platform
		((fPlayerPosX <= ptfmTopLeftCorner.x) && (fNewPlayerPosX >= ptfmBotRightCorner.x) && (topCornerY >= ptfmTopLeftCorner.y) && (topCornerY <= ptfmBotRightCorner.y)) ||
		((fPlayerPosX <= ptfmTopLeftCorner.x) && (fNewPlayerPosX >= ptfmBotRightCorner.x) && (botCornerY >= ptfmTopLeftCorner.y) && (botCornerY <= ptfmBotRightCorner.y)) ||
		// playerPX is in the platform and newPlayerPY is right to the platform
		((fPlayerPosX >= ptfmTopLeftCorner.x) && (fPlayerPosX <= ptfmBotRightCorner.x) && (fNewPlayerPosX >= ptfmBotRightCorner.x) && (topCornerY >= ptfmTopLeftCorner.y) && (topCornerY <= ptfmBotRightCorner.y)) ||
		((fPlayerPosX >= ptfmTopLeftCorner.x) && (fPlayerPosX <= ptfmBotRightCorner.x) && (fNewPlayerPosX >= ptfmBotRightCorner.x) && (botCornerY >= ptfmTopLeftCorner.y) && (botCornerY <= ptfmBotRightCorner.y)) ||
		// playerPX is in the Previous platform position
		((fPlayerPosX >= prevPtfmTopLeftCorner.x) && (fPlayerPosX <= prevPtfmBotRightCorner.x) && (topCornerY >= prevPtfmTopLeftCorner.y) && (topCornerY <= prevPtfmBotRightCorner.y)) ||
		((fPlayerPosX >= prevPtfmTopLeftCorner.x) && (fPlayerPosX <= prevPtfmBotRightCorner.x) && (botCornerY >= prevPtfmTopLeftCorner.y) && (botCornerY <= prevPtfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::LeftCollisionOneCorner(float CornerX, float cornerY)
{
	olc::vf2d dynCorner(CornerX, cornerY);

	olc::vf2d ptfmTopLeftCorner(px - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	return (dynCorner.x >= ptfmTopLeftCorner.x) && (dynCorner.x <= ptfmBotRightCorner.x) && (dynCorner.y >= ptfmTopLeftCorner.y) && (dynCorner.y <= ptfmBotRightCorner.y);
}

bool cDynamicMovingPlatform::RightCollision(float topCornerY, float botCornerY, float leftSpriteX)
{
	olc::vf2d playerTopCorner(leftSpriteX, topCornerY);
	olc::vf2d playerBotCorner(leftSpriteX, botCornerY);

	olc::vf2d ptfmTopLeftCorner(px + (fDynWidth / (float)engine->GetTileWidth()) - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()) + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	return bSolidRight && (
		((playerTopCorner.x >= ptfmTopLeftCorner.x) && (playerTopCorner.x <= ptfmBotRightCorner.x) && (playerTopCorner.y >= ptfmTopLeftCorner.y) && (playerTopCorner.y <= ptfmBotRightCorner.y)) ||
		((playerBotCorner.x >= ptfmTopLeftCorner.x) && (playerBotCorner.x <= ptfmBotRightCorner.x) && (playerBotCorner.y >= ptfmTopLeftCorner.y) && (playerBotCorner.y <= ptfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::RightCollisionWithLag(float topCornerY, float botCornerY, float fPlayerPosX, float fNewPlayerPosX)
{
	olc::vf2d ptfmTopLeftCorner(px + (fDynWidth / (float)engine->GetTileWidth()) - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()) + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	olc::vf2d prevPtfmTopLeftCorner(fPrevPosX + (fDynWidth / (float)engine->GetTileWidth()) - fHitboxThickness, fPrevPosY);
	olc::vf2d prevPtfmBotRightCorner(fPrevPosX + (fDynWidth / (float)engine->GetTileWidth()) + fHitboxThickness, fPrevPosY + (fDynHeight / (float)engine->GetTileHeight()));

	return bSolidRight && (
		// playerPX is right to the platform and newPlayerPY is left to the platform
		((fPlayerPosX >= ptfmTopLeftCorner.x) && (fNewPlayerPosX <= ptfmBotRightCorner.x) && (topCornerY >= ptfmTopLeftCorner.y) && (topCornerY <= ptfmBotRightCorner.y)) ||
		((fPlayerPosX >= ptfmTopLeftCorner.x) && (fNewPlayerPosX <= ptfmBotRightCorner.x) && (botCornerY >= ptfmTopLeftCorner.y) && (botCornerY <= ptfmBotRightCorner.y)) ||
		// playerPX is in the platform and newPlayerPY is left to the platform
		((fPlayerPosX >= ptfmTopLeftCorner.x) && (fPlayerPosX <= ptfmBotRightCorner.x) && (fNewPlayerPosX <= ptfmBotRightCorner.x) && (topCornerY >= ptfmTopLeftCorner.y) && (topCornerY <= ptfmBotRightCorner.y)) ||
		((fPlayerPosX >= ptfmTopLeftCorner.x) && (fPlayerPosX <= ptfmBotRightCorner.x) && (fNewPlayerPosX <= ptfmBotRightCorner.x) && (botCornerY >= ptfmTopLeftCorner.y) && (botCornerY <= ptfmBotRightCorner.y)) ||
		// playerPX is in the Previous platform position
		((fPlayerPosX >= prevPtfmTopLeftCorner.x) && (fPlayerPosX <= prevPtfmBotRightCorner.x) && (topCornerY >= prevPtfmTopLeftCorner.y) && (topCornerY <= prevPtfmBotRightCorner.y)) ||
		((fPlayerPosX >= prevPtfmTopLeftCorner.x) && (fPlayerPosX <= prevPtfmBotRightCorner.x) && (botCornerY >= prevPtfmTopLeftCorner.y) && (botCornerY <= prevPtfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::RightCollisionOneCorner(float CornerX, float cornerY)
{
	olc::vf2d dynCorner(CornerX, cornerY);

	olc::vf2d ptfmTopLeftCorner(px + (fDynWidth / (float)engine->GetTileWidth()) - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()) + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	return (dynCorner.x >= ptfmTopLeftCorner.x) && (dynCorner.x <= ptfmBotRightCorner.x) && (dynCorner.y >= ptfmTopLeftCorner.y) && (dynCorner.y <= ptfmBotRightCorner.y);
}

bool cDynamicMovingPlatform::BotCollision(float leftCornerX, float rightCornerX, float topSpriteY)
{
	olc::vf2d playerLeftCorner(leftCornerX, topSpriteY);
	olc::vf2d playerRightCorner(rightCornerX, topSpriteY);

	olc::vf2d ptfmTopLeftCorner(px, py + (fDynHeight / (float)engine->GetTileHeight()) - fHitboxThickness);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()), py + (fDynHeight / (float)engine->GetTileHeight()) + fHitboxThickness);

	return bSolidBot && (
		((playerLeftCorner.x  >= ptfmTopLeftCorner.x) && (playerLeftCorner.x  <= ptfmBotRightCorner.x) && (playerLeftCorner.y  >= ptfmTopLeftCorner.y) && (playerLeftCorner.y  <= ptfmBotRightCorner.y)) ||
		((playerRightCorner.x >= ptfmTopLeftCorner.x) && (playerRightCorner.x <= ptfmBotRightCorner.x) && (playerRightCorner.y >= ptfmTopLeftCorner.y) && (playerRightCorner.y <= ptfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::BotCollisionWithLag(float leftCornerX, float rightCornerX, float fPlayerPosY, float fNewPlayerPosY)
{
	olc::vf2d ptfmTopLeftCorner(px, py + (fDynHeight / (float)engine->GetTileHeight()) - fHitboxThickness);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()), py + (fDynHeight / (float)engine->GetTileHeight()) + fHitboxThickness);

	olc::vf2d prevPtfmTopLeftCorner(fPrevPosX, fPrevPosY + (fDynHeight / (float)engine->GetTileHeight()) - fHitboxThickness);
	olc::vf2d prevPtfmBotRightCorner(fPrevPosX + (fDynWidth / (float)engine->GetTileWidth()), fPrevPosY + (fDynHeight / (float)engine->GetTileHeight()) + fHitboxThickness);

	return bSolidBot && (
		// playerPY is below platform and newPlayerPY is above platform
		((leftCornerX  >= ptfmTopLeftCorner.x) && (leftCornerX  <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmTopLeftCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmTopLeftCorner.y)) ||
		// playerPY is in the platform and newPlayerPY is above platform
		((leftCornerX  >= ptfmTopLeftCorner.x) && (leftCornerX  <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmBotRightCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmBotRightCorner.y)) ||
		// playerPY is in the Previous platform position
		((leftCornerX  >= prevPtfmTopLeftCorner.x) && (leftCornerX  <= prevPtfmBotRightCorner.x) && (fPlayerPosY >= prevPtfmTopLeftCorner.y) && (fPlayerPosY <= prevPtfmBotRightCorner.y)) ||
		((rightCornerX >= prevPtfmTopLeftCorner.x) && (rightCornerX <= prevPtfmBotRightCorner.x) && (fPlayerPosY >= prevPtfmTopLeftCorner.y) && (fPlayerPosY <= prevPtfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::BotCollisionOneCorner(float CornerX, float cornerY)
{
	olc::vf2d dynCorner(CornerX, cornerY);

	olc::vf2d ptfmTopLeftCorner(px, py + (fDynHeight / (float)engine->GetTileHeight()) - fHitboxThickness);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()), py + (fDynHeight / (float)engine->GetTileHeight()) + fHitboxThickness);

	return (dynCorner.x >= ptfmTopLeftCorner.x) && (dynCorner.x <= ptfmBotRightCorner.x) && (dynCorner.y >= ptfmTopLeftCorner.y) && (dynCorner.y <= ptfmBotRightCorner.y);
}

std::map<std::string, std::vector<olc::Sprite*>> cDynamicMovingPlatform::LoadMovingPlatformsSprites()
{
	std::map<std::string, std::vector<olc::Sprite*>> mapPlatforms;

#pragma region Platforms and walls
	mapPlatforms["basic"].push_back(new olc::Sprite("assets/gfx/basicPlatform.png"));
	mapPlatforms["iceBasic"].push_back(new olc::Sprite("assets/gfx/iceBasicPlatform.png"));
	mapPlatforms["marbleBasic"].push_back(new olc::Sprite("assets/gfx/marbleBasicPlatform.png"));
	mapPlatforms["halberdBasic"].push_back(new olc::Sprite("assets/gfx/halberdBasicPlatform.png"));
	mapPlatforms["fountainBasic"].push_back(new olc::Sprite("assets/gfx/fountainBasicPlatform.png"));

	mapPlatforms["ice"].push_back(new olc::Sprite("assets/gfx/icePlatform.png"));

	mapPlatforms["wall"].push_back(new olc::Sprite("assets/gfx/wall.png"));
	mapPlatforms["iceWall"].push_back(new olc::Sprite("assets/gfx/iceWall.png"));
	mapPlatforms["marbleWall"].push_back(new olc::Sprite("assets/gfx/marbleWall.png"));
	mapPlatforms["marbleWallLong"].push_back(new olc::Sprite("assets/gfx/marbleWallLong.png"));
	mapPlatforms["fountainWall"].push_back(new olc::Sprite("assets/gfx/fountainWall.png"));

	mapPlatforms["toLeft"].push_back(new olc::Sprite("assets/gfx/toLeft.png"));
	mapPlatforms["toRight"].push_back(new olc::Sprite("assets/gfx/toRight.png"));

	mapPlatforms["marbleCeiling"].push_back(new olc::Sprite("assets/gfx/marbleBasicCeiling.png"));
	mapPlatforms["fountainCeiling"].push_back(new olc::Sprite("assets/gfx/fountainBasicCeiling.png"));
#pragma endregion

#pragma region Sceneries
	// lvl 1
	mapPlatforms["arrowPanel"].push_back(new olc::Sprite("assets/gfx/arrowPanel.png"));
	mapPlatforms["tree"].push_back(new olc::Sprite("assets/gfx/tree.png"));
	mapPlatforms["bush"].push_back(new olc::Sprite("assets/gfx/bush.png"));
	mapPlatforms["mojo"].push_back(new olc::Sprite("assets/gfx/mojo.png"));
	mapPlatforms["caterpillar"].push_back(new olc::Sprite("assets/gfx/caterpillar.png"));
	mapPlatforms["happyShroom"].push_back(new olc::Sprite("assets/gfx/happyShroom.png"));
	mapPlatforms["pirahnaPlant"].push_back(new olc::Sprite("assets/gfx/pirahnaPlant.png"));
	mapPlatforms["smilingShroom"].push_back(new olc::Sprite("assets/gfx/smilingShroom.png"));
	mapPlatforms["soldierSkeleton"].push_back(new olc::Sprite("assets/gfx/soldierSkeleton00.png"));
	mapPlatforms["soldierSkeleton"].push_back(new olc::Sprite("assets/gfx/soldierSkeleton01.png"));
	mapPlatforms["soldierSkeleton"].push_back(new olc::Sprite("assets/gfx/soldierSkeleton02.png"));
	mapPlatforms["bahbahbahbah"].push_back(new olc::Sprite("assets/gfx/bahbahbahbah00.png"));
	mapPlatforms["bahbahbahbah"].push_back(new olc::Sprite("assets/gfx/bahbahbahbah01.png"));
	mapPlatforms["bahbahbahbah"].push_back(new olc::Sprite("assets/gfx/bahbahbahbah02.png"));
	mapPlatforms["bahbahbahbah"].push_back(new olc::Sprite("assets/gfx/bahbahbahbah03.png"));
	mapPlatforms["kremling"].push_back(new olc::Sprite("assets/gfx/kremling.png"));
	mapPlatforms["lvl1Exit"].push_back(new olc::Sprite("assets/gfx/lvl1Exit.png"));
	// boss 1
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros00.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros01.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros02.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros03.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros04.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros05.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros06.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros07.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros08.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros09.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros10.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros11.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros12.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros13.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros14.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros15.png"));
	mapPlatforms["skullKidBush"].push_back(new olc::Sprite("assets/gfx/skullKidBush.png"));
	mapPlatforms["chikoritaBush"].push_back(new olc::Sprite("assets/gfx/chikoritaBush.png"));
	mapPlatforms["coolBugFact"].push_back(new olc::Sprite("assets/gfx/coolBugFact.png"));

	// lvl 2
	mapPlatforms["hillYellow"].push_back(new olc::Sprite("assets/gfx/hillYellow.png"));
	mapPlatforms["hillRed"].push_back(new olc::Sprite("assets/gfx/hillRed.png"));
	mapPlatforms["hillsRed"].push_back(new olc::Sprite("assets/gfx/hillsRed.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill00.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill01.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill02.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill03.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill04.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill05.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill06.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill07.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill08.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill09.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill10.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill11.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill12.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill13.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill14.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill15.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill16.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill17.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill18.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill19.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill20.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill21.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill22.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill23.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill24.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill25.png"));
    mapPlatforms["mustacheHill"].push_back(new olc::Sprite("assets/gfx/mustacheHill.png"));
    mapPlatforms["wryHill"].push_back(new olc::Sprite("assets/gfx/wryHill.png"));
    mapPlatforms["goomba"].push_back(new olc::Sprite("assets/gfx/goomba.png"));
    mapPlatforms["greenKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/greenKoopaTroopa.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa00.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa01.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa02.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa03.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill00.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill01.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill02.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill03.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill04.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill05.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill06.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill07.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill08.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill09.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill10.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill11.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill12.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill13.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill14.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill15.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill16.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill17.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill18.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill19.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill20.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill21.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill22.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill23.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill24.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill25.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill26.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill27.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill28.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill29.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill30.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill31.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill32.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill33.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill34.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill35.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill36.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill37.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill38.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill39.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill40.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill41.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill42.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill43.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill44.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill45.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill46.png"));
    mapPlatforms["rotatingHill"].push_back(new olc::Sprite("assets/gfx/rotatingHill47.png"));
    mapPlatforms["beaver"].push_back(new olc::Sprite("assets/gfx/beaver.png"));
	mapPlatforms["lvl2Exit"].push_back(new olc::Sprite("assets/gfx/lvl2Exit.png"));
	// boss 2
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy00.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy01.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy02.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy03.png"));
	mapPlatforms["burningCappy"].push_back(new olc::Sprite("assets/gfx/burningCappy00.png"));
	mapPlatforms["burningCappy"].push_back(new olc::Sprite("assets/gfx/burningCappy01.png"));

	// lvl 3
	mapPlatforms["snowman"].push_back(new olc::Sprite("assets/gfx/snowman.png"));
	mapPlatforms["snowtree"].push_back(new olc::Sprite("assets/gfx/snowtree.png"));
	mapPlatforms["scrat"].push_back(new olc::Sprite("assets/gfx/scrat.png"));
	mapPlatforms["frozenMeowthAndCleffa"].push_back(new olc::Sprite("assets/gfx/frozenMeowthAndCleffa.png"));
	mapPlatforms["sunkenShip"].push_back(new olc::Sprite("assets/gfx/sunkenShip.png"));
	mapPlatforms["eskimoPenguin"].push_back(new olc::Sprite("assets/gfx/eskimoPenguin.png"));
	mapPlatforms["frozenKingZora"].push_back(new olc::Sprite("assets/gfx/frozenKingZora.png"));
	mapPlatforms["RaphaelTheRaven"].push_back(new olc::Sprite("assets/gfx/RaphaelTheRaven.png"));
	mapPlatforms["frozenPoppyBros"].push_back(new olc::Sprite("assets/gfx/frozenPoppyBros.png"));
	mapPlatforms["speedrunnerLink"].push_back(new olc::Sprite("assets/gfx/speedrunnerLink.png"));
	mapPlatforms["lvl3Exit"].push_back(new olc::Sprite("assets/gfx/lvl3Exit.png"));
	// boss 3
	mapPlatforms["nipperPlant"].push_back(new olc::Sprite("assets/gfx/nipperPlant.png"));
	mapPlatforms["snowMount"].push_back(new olc::Sprite("assets/gfx/snowMount.png"));
	mapPlatforms["painter"].push_back(new olc::Sprite("assets/gfx/painter.png"));

	// lvl 4
	mapPlatforms["upArrow"].push_back(new olc::Sprite("assets/gfx/upArrowPanel.png"));
	mapPlatforms["upArrow2"].push_back(new olc::Sprite("assets/gfx/upArrowPanel2.png"));
	mapPlatforms["rightArrow"].push_back(new olc::Sprite("assets/gfx/rightArrowPanel.png"));
	mapPlatforms["rightArrow2"].push_back(new olc::Sprite("assets/gfx/rightArrowPanel2.png"));
	mapPlatforms["leftArrow"].push_back(new olc::Sprite("assets/gfx/leftArrowPanel.png"));
	mapPlatforms["leftArrow2"].push_back(new olc::Sprite("assets/gfx/leftArrowPanel2.png"));
	mapPlatforms["downArrow"].push_back(new olc::Sprite("assets/gfx/downArrowPanel.png"));
	mapPlatforms["downArrow2"].push_back(new olc::Sprite("assets/gfx/downArrowPanel2.png"));
	mapPlatforms["cloud"].push_back(new olc::Sprite("assets/gfx/cloud.png"));
	mapPlatforms["smileCloud"].push_back(new olc::Sprite("assets/gfx/smileCloud.png"));
	mapPlatforms["cloudConstruction1"].push_back(new olc::Sprite("assets/gfx/cloudConstruction1.png"));
	mapPlatforms["cloudConstruction2"].push_back(new olc::Sprite("assets/gfx/cloudConstruction2.png"));
	mapPlatforms["cloudConstruction3"].push_back(new olc::Sprite("assets/gfx/cloudConstruction3.png"));
	mapPlatforms["cloudConstruction4"].push_back(new olc::Sprite("assets/gfx/cloudConstruction4.png"));
	mapPlatforms["cloudConstruction5"].push_back(new olc::Sprite("assets/gfx/cloudConstruction5.png"));
	mapPlatforms["cloudConstruction6"].push_back(new olc::Sprite("assets/gfx/cloudConstruction6.png"));
	mapPlatforms["pit"].push_back(new olc::Sprite("assets/gfx/pit00.png"));
	mapPlatforms["pit"].push_back(new olc::Sprite("assets/gfx/pit01.png"));
	mapPlatforms["lvl4Exit"].push_back(new olc::Sprite("assets/gfx/lvl4Exit.png"));
	// boss 4
	mapPlatforms["pikachu"].push_back(new olc::Sprite("assets/gfx/pikachu.png"));
	mapPlatforms["electricalSocket"].push_back(new olc::Sprite("assets/gfx/electricalSocket.png"));

	// lvl 5
	mapPlatforms["chunLi"].push_back(new olc::Sprite("assets/gfx/chunLi00.png"));
	mapPlatforms["chunLi"].push_back(new olc::Sprite("assets/gfx/chunLi01.png"));
	mapPlatforms["chunLi"].push_back(new olc::Sprite("assets/gfx/chunLi02.png"));
	mapPlatforms["chunLi"].push_back(new olc::Sprite("assets/gfx/chunLi03.png"));
	mapPlatforms["chunLi"].push_back(new olc::Sprite("assets/gfx/chunLi04.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango00.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango01.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro00.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro01.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro02.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro03.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro04.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro05.png"));
	mapPlatforms["samusShip"].push_back(new olc::Sprite("assets/gfx/samusShip00.png"));
	mapPlatforms["samusShip"].push_back(new olc::Sprite("assets/gfx/samusShip01.png"));
	mapPlatforms["samusShip"].push_back(new olc::Sprite("assets/gfx/samusShip02.png"));
	mapPlatforms["samusShip"].push_back(new olc::Sprite("assets/gfx/samusShip03.png"));
	mapPlatforms["olimarShip"].push_back(new olc::Sprite("assets/gfx/olimarShip00.png"));
	mapPlatforms["olimarShip"].push_back(new olc::Sprite("assets/gfx/olimarShip01.png"));
	mapPlatforms["olimarShip"].push_back(new olc::Sprite("assets/gfx/olimarShip02.png"));
	mapPlatforms["olimarShip"].push_back(new olc::Sprite("assets/gfx/olimarShip03.png"));
	mapPlatforms["olimar"].push_back(new olc::Sprite("assets/gfx/olimar.png"));
	mapPlatforms["lvl5Exit"].push_back(new olc::Sprite("assets/gfx/lvl5Exit.png"));
	// boss 5
	mapPlatforms["MKSword"].push_back(new olc::Sprite("assets/gfx/MKSword.png"));
	mapPlatforms["fence"].push_back(new olc::Sprite("assets/gfx/fence.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee01.png"));
	mapPlatforms["sirKibble"].push_back(new olc::Sprite("assets/gfx/sirKibble.png"));
	mapPlatforms["spectateBladeKnight"].push_back(new olc::Sprite("assets/gfx/spectateBladeKnight.png"));
	mapPlatforms["waddleDoo"].push_back(new olc::Sprite("assets/gfx/waddleDoo.png"));

	// lvl 6
	mapPlatforms["elementWood"].push_back(new olc::Sprite("assets/gfx/elementWood.png"));
	mapPlatforms["elementEarth"].push_back(new olc::Sprite("assets/gfx/elementEarth.png"));
	mapPlatforms["elementMetal"].push_back(new olc::Sprite("assets/gfx/elementMetal.png"));
	mapPlatforms["elementWater"].push_back(new olc::Sprite("assets/gfx/elementWater.png"));
	mapPlatforms["elementFire"].push_back(new olc::Sprite("assets/gfx/elementFire.png"));
	mapPlatforms["deedeedeeStatue"].push_back(new olc::Sprite("assets/gfx/deedeedeeStatue.png"));
	mapPlatforms["deathPanel"].push_back(new olc::Sprite("assets/gfx/deathPanel.png"));
	mapPlatforms["greatTree"].push_back(new olc::Sprite("assets/gfx/greatTree.png"));
	mapPlatforms["greatBranch"].push_back(new olc::Sprite("assets/gfx/greatBranch.png"));
	mapPlatforms["groundGate"].push_back(new olc::Sprite("assets/gfx/groundGate.png"));
	mapPlatforms["cave"].push_back(new olc::Sprite("assets/gfx/cave.png"));
	mapPlatforms["diglett"].push_back(new olc::Sprite("assets/gfx/diglett.png"));
	mapPlatforms["groundMound"].push_back(new olc::Sprite("assets/gfx/groundMound.png"));
	mapPlatforms["carrots"].push_back(new olc::Sprite("assets/gfx/carrots.png"));
	mapPlatforms["leftGrdWall"].push_back(new olc::Sprite("assets/gfx/leftGrdWall.png"));
	mapPlatforms["rightGrdWall"].push_back(new olc::Sprite("assets/gfx/rightGrdWall.png"));
	mapPlatforms["midGrdWall"].push_back(new olc::Sprite("assets/gfx/midGrdWall.png"));
	mapPlatforms["miningKremling"].push_back(new olc::Sprite("assets/gfx/miningKremling00.png"));
	mapPlatforms["miningKremling"].push_back(new olc::Sprite("assets/gfx/miningKremling01.png"));
	mapPlatforms["miningKremling"].push_back(new olc::Sprite("assets/gfx/miningKremling02.png"));
	mapPlatforms["mukUsedToxic"].push_back(new olc::Sprite("assets/gfx/mukUsedToxic00.png"));
	mapPlatforms["mukUsedToxic"].push_back(new olc::Sprite("assets/gfx/mukUsedToxic01.png"));
	mapPlatforms["mukUsedToxic"].push_back(new olc::Sprite("assets/gfx/mukUsedToxic02.png"));
	mapPlatforms["mukUsedToxic"].push_back(new olc::Sprite("assets/gfx/mukUsedToxic03.png"));
	mapPlatforms["mukUsedToxic"].push_back(new olc::Sprite("assets/gfx/mukUsedToxic04.png"));
	mapPlatforms["metalVent"].push_back(new olc::Sprite("assets/gfx/metalVent.png"));
	mapPlatforms["abbath"].push_back(new olc::Sprite("assets/gfx/abbath.png"));
	mapPlatforms["mettaton"].push_back(new olc::Sprite("assets/gfx/mettaton00.png"));
	mapPlatforms["mettaton"].push_back(new olc::Sprite("assets/gfx/mettaton01.png"));
	mapPlatforms["waddlingRegisteel"].push_back(new olc::Sprite("assets/gfx/waddlingRegisteel00.png"));
	mapPlatforms["waddlingRegisteel"].push_back(new olc::Sprite("assets/gfx/waddlingRegisteel01.png"));
	mapPlatforms["waddlingRegisteel"].push_back(new olc::Sprite("assets/gfx/waddlingRegisteel02.png"));
	mapPlatforms["waddlingRegisteel"].push_back(new olc::Sprite("assets/gfx/waddlingRegisteel03.png"));
	mapPlatforms["A350"].push_back(new olc::Sprite("assets/gfx/A350.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall00.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall01.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall02.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow00.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow01.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow02.png"));
	mapPlatforms["frisk"].push_back(new olc::Sprite("assets/gfx/frisk.png"));
	mapPlatforms["kingZora"].push_back(new olc::Sprite("assets/gfx/kingZora.png"));
	mapPlatforms["enguardeTheSwordfish"].push_back(new olc::Sprite("assets/gfx/enguardeTheSwordfish00.png"));
	mapPlatforms["enguardeTheSwordfish"].push_back(new olc::Sprite("assets/gfx/enguardeTheSwordfish01.png"));
	mapPlatforms["enguardeTheSwordfish"].push_back(new olc::Sprite("assets/gfx/enguardeTheSwordfish02.png"));
	mapPlatforms["enguardeTheSwordfish"].push_back(new olc::Sprite("assets/gfx/enguardeTheSwordfish03.png"));
	mapPlatforms["fireStatueLeft"].push_back(new olc::Sprite("assets/gfx/fireStatueLeft.png"));
	mapPlatforms["fireStatueRight"].push_back(new olc::Sprite("assets/gfx/fireStatueRight.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire00.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire01.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire02.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire03.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire04.png"));
	mapPlatforms["burntLink"].push_back(new olc::Sprite("assets/gfx/burntLink.png"));
	mapPlatforms["torch"].push_back(new olc::Sprite("assets/gfx/torch.png"));
	mapPlatforms["torchLeft"].push_back(new olc::Sprite("assets/gfx/torchLeft.png"));
	mapPlatforms["torchRight"].push_back(new olc::Sprite("assets/gfx/torchRight.png"));
	mapPlatforms["charizardFamily"].push_back(new olc::Sprite("assets/gfx/charizardFamily.png"));
	mapPlatforms["gasCooker"].push_back(new olc::Sprite("assets/gfx/gasCooker.png"));
	mapPlatforms["entei"].push_back(new olc::Sprite("assets/gfx/entei00.png"));
	mapPlatforms["entei"].push_back(new olc::Sprite("assets/gfx/entei01.png"));
	mapPlatforms["elementsDirections"].push_back(new olc::Sprite("assets/gfx/elementsDirections.png"));
	mapPlatforms["lvl6Exit"].push_back(new olc::Sprite("assets/gfx/lvl6Exit.png"));
	// boss 6
	mapPlatforms["waterMark"].push_back(new olc::Sprite("assets/gfx/waterMark00.png"));
	mapPlatforms["waterMark"].push_back(new olc::Sprite("assets/gfx/waterMark01.png"));
	mapPlatforms["fountainFlow"].push_back(new olc::Sprite("assets/gfx/fountainFlow00.png"));
	mapPlatforms["fountainFlow"].push_back(new olc::Sprite("assets/gfx/fountainFlow01.png"));
	mapPlatforms["meleeFox"].push_back(new olc::Sprite("assets/gfx/meleeFox00.png"));
	mapPlatforms["meleeFox"].push_back(new olc::Sprite("assets/gfx/meleeFox01.png"));
	mapPlatforms["meleeFox"].push_back(new olc::Sprite("assets/gfx/meleeFox02.png"));
	mapPlatforms["meleeFox"].push_back(new olc::Sprite("assets/gfx/meleeFox03.png"));
	mapPlatforms["meleeMarth"].push_back(new olc::Sprite("assets/gfx/meleeMarth00.png"));
	mapPlatforms["meleeMarth"].push_back(new olc::Sprite("assets/gfx/meleeMarth01.png"));
	mapPlatforms["meleeMarth"].push_back(new olc::Sprite("assets/gfx/meleeMarth02.png"));
	mapPlatforms["meleeMarth"].push_back(new olc::Sprite("assets/gfx/meleeMarth03.png"));
	mapPlatforms["SSTierMK"].push_back(new olc::Sprite("assets/gfx/SSTierMK00.png"));
	mapPlatforms["SSTierMK"].push_back(new olc::Sprite("assets/gfx/SSTierMK01.png"));
	mapPlatforms["SSTierMK"].push_back(new olc::Sprite("assets/gfx/SSTierMK02.png"));
	mapPlatforms["SSTierMK"].push_back(new olc::Sprite("assets/gfx/SSTierMK03.png"));
	mapPlatforms["SSTierMK"].push_back(new olc::Sprite("assets/gfx/SSTierMK04.png"));
	mapPlatforms["BossBckGrd05_arena"].push_back(new olc::Sprite("assets/gfx/BossBckGrd05_arena.png"));
#pragma endregion

#pragma region Crushers
	mapPlatforms["downCrusher"].push_back(new olc::Sprite("assets/gfx/downCrusher.png"));
	mapPlatforms["fountainCrusher"].push_back(new olc::Sprite("assets/gfx/fountainCrusher.png"));
#pragma endregion

#pragma region HarmfulBlocs
	mapPlatforms["spikeBall"].push_back(new olc::Sprite("assets/gfx/spikeBall.png"));

	mapPlatforms["spikeDown"].push_back(new olc::Sprite("assets/gfx/spikeDown.png"));
	mapPlatforms["spikeUp"].push_back(new olc::Sprite("assets/gfx/spikeUp.png"));
	mapPlatforms["spikeLeft"].push_back(new olc::Sprite("assets/gfx/spikeLeft.png"));
	mapPlatforms["spikeRight"].push_back(new olc::Sprite("assets/gfx/spikeRight.png"));

	mapPlatforms["stalactite"].push_back(new olc::Sprite("assets/gfx/stalactite.png"));
	mapPlatforms["stalacleftite"].push_back(new olc::Sprite("assets/gfx/stalacleftite.png"));
	mapPlatforms["stalagmite"].push_back(new olc::Sprite("assets/gfx/stalagmite.png"));
	mapPlatforms["stalacrightite"].push_back(new olc::Sprite("assets/gfx/stalacrightite.png"));

	mapPlatforms["shipFlame"].push_back(new olc::Sprite("assets/gfx/shipFlame00.png"));
	mapPlatforms["shipFlame"].push_back(new olc::Sprite("assets/gfx/shipFlame01.png"));
	mapPlatforms["shipFlame"].push_back(new olc::Sprite("assets/gfx/shipFlame02.png"));

	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison00.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison01.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison02.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison03.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison04.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison05.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison06.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison07.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison08.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison09.png"));
#pragma endregion

	return mapPlatforms;
}

std::map<std::string, std::vector<olc::Decal*>> cDynamicMovingPlatform::LoadMovingPlatformsDecals(std::map<std::string, std::vector<olc::Sprite*>> mapPlatforms)
{
	std::map<std::string, std::vector<olc::Decal*>> mapDecPlatforms;

#pragma region Platforms and walls
	mapDecPlatforms["basic"].push_back(new olc::Decal(mapPlatforms["basic"][0]));
	mapDecPlatforms["iceBasic"].push_back(new olc::Decal(mapPlatforms["iceBasic"][0]));
	mapDecPlatforms["marbleBasic"].push_back(new olc::Decal(mapPlatforms["marbleBasic"][0]));
	mapDecPlatforms["halberdBasic"].push_back(new olc::Decal(mapPlatforms["halberdBasic"][0]));
	mapDecPlatforms["fountainBasic"].push_back(new olc::Decal(mapPlatforms["fountainBasic"][0]));

	mapDecPlatforms["ice"].push_back(new olc::Decal(mapPlatforms["ice"][0]));

	mapDecPlatforms["wall"].push_back(new olc::Decal(mapPlatforms["wall"][0]));
	mapDecPlatforms["iceWall"].push_back(new olc::Decal(mapPlatforms["iceWall"][0]));
	mapDecPlatforms["marbleWall"].push_back(new olc::Decal(mapPlatforms["marbleWall"][0]));
	mapDecPlatforms["marbleWallLong"].push_back(new olc::Decal(mapPlatforms["marbleWallLong"][0]));
	mapDecPlatforms["fountainWall"].push_back(new olc::Decal(mapPlatforms["fountainWall"][0]));

	mapDecPlatforms["toLeft"].push_back(new olc::Decal(mapPlatforms["toLeft"][0]));
	mapDecPlatforms["toRight"].push_back(new olc::Decal(mapPlatforms["toRight"][0]));

	mapDecPlatforms["marbleCeiling"].push_back(new olc::Decal(mapPlatforms["marbleCeiling"][0]));
	mapDecPlatforms["fountainCeiling"].push_back(new olc::Decal(mapPlatforms["fountainCeiling"][0]));
#pragma endregion

#pragma region Sceneries
	// lvl 1
	mapDecPlatforms["arrowPanel"].push_back(new olc::Decal(mapPlatforms["arrowPanel"][0]));
	mapDecPlatforms["tree"].push_back(new olc::Decal(mapPlatforms["tree"][0]));
	mapDecPlatforms["bush"].push_back(new olc::Decal(mapPlatforms["bush"][0]));
	mapDecPlatforms["mojo"].push_back(new olc::Decal(mapPlatforms["mojo"][0]));
	mapDecPlatforms["caterpillar"].push_back(new olc::Decal(mapPlatforms["caterpillar"][0]));
	mapDecPlatforms["happyShroom"].push_back(new olc::Decal(mapPlatforms["happyShroom"][0]));
	mapDecPlatforms["pirahnaPlant"].push_back(new olc::Decal(mapPlatforms["pirahnaPlant"][0]));
	mapDecPlatforms["smilingShroom"].push_back(new olc::Decal(mapPlatforms["smilingShroom"][0]));
	mapDecPlatforms["soldierSkeleton"].push_back(new olc::Decal(mapPlatforms["soldierSkeleton"][0]));
	mapDecPlatforms["soldierSkeleton"].push_back(new olc::Decal(mapPlatforms["soldierSkeleton"][1]));
	mapDecPlatforms["soldierSkeleton"].push_back(new olc::Decal(mapPlatforms["soldierSkeleton"][2]));
	mapDecPlatforms["bahbahbahbah"].push_back(new olc::Decal(mapPlatforms["bahbahbahbah"][0]));
	mapDecPlatforms["bahbahbahbah"].push_back(new olc::Decal(mapPlatforms["bahbahbahbah"][1]));
	mapDecPlatforms["bahbahbahbah"].push_back(new olc::Decal(mapPlatforms["bahbahbahbah"][2]));
	mapDecPlatforms["bahbahbahbah"].push_back(new olc::Decal(mapPlatforms["bahbahbahbah"][3]));
	mapDecPlatforms["kremling"].push_back(new olc::Decal(mapPlatforms["kremling"][0]));
	mapDecPlatforms["lvl1Exit"].push_back(new olc::Decal(mapPlatforms["lvl1Exit"][0]));
	// boss 1
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][0]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][0]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][0]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][1]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][1]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][1]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][2]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][2]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][2]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][3]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][3]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][3]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][4]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][4]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][4]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][5]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][5]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][5]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][6]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][6]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][6]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][7]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][7]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][7]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][8]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][8]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][8]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][9]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][9]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][9]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][10]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][10]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][10]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][11]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][11]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][11]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][12]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][12]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][12]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][13]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][13]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][13]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][14]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][14]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][14]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][15]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][15]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][15]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][8]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][8]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][8]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][9]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][9]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][9]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][10]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][10]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][10]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][11]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][11]));
	mapDecPlatforms["dancingPoppyBros"].push_back(new olc::Decal(mapPlatforms["dancingPoppyBros"][11]));
	mapDecPlatforms["skullKidBush"].push_back(new olc::Decal(mapPlatforms["skullKidBush"][0]));
	mapDecPlatforms["chikoritaBush"].push_back(new olc::Decal(mapPlatforms["chikoritaBush"][0]));
	mapDecPlatforms["coolBugFact"].push_back(new olc::Decal(mapPlatforms["coolBugFact"][0]));

	// lvl 2
	mapDecPlatforms["hillYellow"].push_back(new olc::Decal(mapPlatforms["hillYellow"][0]));
	mapDecPlatforms["hillRed"].push_back(new olc::Decal(mapPlatforms["hillRed"][0]));
	mapDecPlatforms["hillsRed"].push_back(new olc::Decal(mapPlatforms["hillsRed"][0]));
	// ------- very long animation -------
	for (int i = 0; i < 90; i++)
	{
		// wait 3 seconds before eating the little hill
		mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][0]));
	}
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][1]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][2]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][1]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][2]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][1]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][2]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][1]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][2]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][3]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][4]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][5]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][5]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][6]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][6]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][7]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][8]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][9]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][10]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][10]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][11]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][11]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][12]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][12]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][13]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][13]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][14]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][14]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][15]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][15]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][16]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][16]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][16]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][16]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][17]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][18]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][19]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][20]));
	for (int i = 0; i < 90; i++)
	{
		// wait 3 seconds before the hill respawn
		mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][21]));
	}
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][22]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][23]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][24]));
	mapDecPlatforms["cannibalHill"].push_back(new olc::Decal(mapPlatforms["cannibalHill"][25]));
	// ------- very long animation -------
	mapDecPlatforms["mustacheHill"].push_back(new olc::Decal(mapPlatforms["mustacheHill"][0]));
	mapDecPlatforms["wryHill"].push_back(new olc::Decal(mapPlatforms["wryHill"][0]));
	mapDecPlatforms["goomba"].push_back(new olc::Decal(mapPlatforms["goomba"][0]));
	mapDecPlatforms["greenKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["greenKoopaTroopa"][0]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][0]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][0]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][1]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][1]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][2]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][2]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][2]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][3]));
	mapDecPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Decal(mapPlatforms["flyingRedKoopaTroopa"][3]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][0]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][1]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][2]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][3]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][4]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][5]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][6]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][7]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][8]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][9]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][10]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][11]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][12]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][13]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][14]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][15]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][16]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][17]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][18]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][19]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][20]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][21]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][22]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][23]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][24]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][25]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][26]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][27]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][28]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][29]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][30]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][31]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][32]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][33]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][34]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][35]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][36]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][37]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][38]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][39]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][40]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][41]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][42]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][43]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][44]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][45]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][46]));
	mapDecPlatforms["rotatingHill"].push_back(new olc::Decal(mapPlatforms["rotatingHill"][47]));
	mapDecPlatforms["beaver"].push_back(new olc::Decal(mapPlatforms["beaver"][0]));
	mapDecPlatforms["lvl2Exit"].push_back(new olc::Decal(mapPlatforms["lvl2Exit"][0]));
	// boss 2
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][0]));
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][0]));
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][1]));
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][1]));
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][2]));
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][2]));
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][3]));
	mapDecPlatforms["cappy"].push_back(new olc::Decal(mapPlatforms["cappy"][3]));
	mapDecPlatforms["burningCappy"].push_back(new olc::Decal(mapPlatforms["burningCappy"][0]));
	mapDecPlatforms["burningCappy"].push_back(new olc::Decal(mapPlatforms["burningCappy"][1]));

	// lvl 3
	mapDecPlatforms["snowman"].push_back(new olc::Decal(mapPlatforms["snowman"][0]));
	mapDecPlatforms["snowtree"].push_back(new olc::Decal(mapPlatforms["snowtree"][0]));
	mapDecPlatforms["scrat"].push_back(new olc::Decal(mapPlatforms["scrat"][0]));
	mapDecPlatforms["frozenMeowthAndCleffa"].push_back(new olc::Decal(mapPlatforms["frozenMeowthAndCleffa"][0]));
	mapDecPlatforms["sunkenShip"].push_back(new olc::Decal(mapPlatforms["sunkenShip"][0]));
	mapDecPlatforms["eskimoPenguin"].push_back(new olc::Decal(mapPlatforms["eskimoPenguin"][0]));
	mapDecPlatforms["frozenKingZora"].push_back(new olc::Decal(mapPlatforms["frozenKingZora"][0]));
	mapDecPlatforms["RaphaelTheRaven"].push_back(new olc::Decal(mapPlatforms["RaphaelTheRaven"][0]));
	mapDecPlatforms["frozenPoppyBros"].push_back(new olc::Decal(mapPlatforms["frozenPoppyBros"][0]));
	mapDecPlatforms["speedrunnerLink"].push_back(new olc::Decal(mapPlatforms["speedrunnerLink"][0]));
	mapDecPlatforms["lvl3Exit"].push_back(new olc::Decal(mapPlatforms["lvl3Exit"][0]));
	// boss 3
	mapDecPlatforms["nipperPlant"].push_back(new olc::Decal(mapPlatforms["nipperPlant"][0]));
	mapDecPlatforms["snowMount"].push_back(new olc::Decal(mapPlatforms["snowMount"][0]));
	mapDecPlatforms["painter"].push_back(new olc::Decal(mapPlatforms["painter"][0]));

	// lvl 4
	mapDecPlatforms["upArrow"].push_back(new olc::Decal(mapPlatforms["upArrow"][0]));
	mapDecPlatforms["upArrow2"].push_back(new olc::Decal(mapPlatforms["upArrow2"][0]));
	mapDecPlatforms["rightArrow"].push_back(new olc::Decal(mapPlatforms["rightArrow"][0]));
	mapDecPlatforms["rightArrow2"].push_back(new olc::Decal(mapPlatforms["rightArrow2"][0]));
	mapDecPlatforms["leftArrow"].push_back(new olc::Decal(mapPlatforms["leftArrow"][0]));
	mapDecPlatforms["leftArrow2"].push_back(new olc::Decal(mapPlatforms["leftArrow2"][0]));
	mapDecPlatforms["downArrow"].push_back(new olc::Decal(mapPlatforms["downArrow"][0]));
	mapDecPlatforms["downArrow2"].push_back(new olc::Decal(mapPlatforms["downArrow2"][0]));
	mapDecPlatforms["cloud"].push_back(new olc::Decal(mapPlatforms["cloud"][0]));
	mapDecPlatforms["smileCloud"].push_back(new olc::Decal(mapPlatforms["smileCloud"][0]));
	mapDecPlatforms["cloudConstruction1"].push_back(new olc::Decal(mapPlatforms["cloudConstruction1"][0]));
	mapDecPlatforms["cloudConstruction2"].push_back(new olc::Decal(mapPlatforms["cloudConstruction2"][0]));
	mapDecPlatforms["cloudConstruction3"].push_back(new olc::Decal(mapPlatforms["cloudConstruction3"][0]));
	mapDecPlatforms["cloudConstruction4"].push_back(new olc::Decal(mapPlatforms["cloudConstruction4"][0]));
	mapDecPlatforms["cloudConstruction5"].push_back(new olc::Decal(mapPlatforms["cloudConstruction5"][0]));
	mapDecPlatforms["cloudConstruction6"].push_back(new olc::Decal(mapPlatforms["cloudConstruction6"][0]));
	mapDecPlatforms["pit"].push_back(new olc::Decal(mapPlatforms["pit"][0]));
	mapDecPlatforms["pit"].push_back(new olc::Decal(mapPlatforms["pit"][0]));
	mapDecPlatforms["pit"].push_back(new olc::Decal(mapPlatforms["pit"][1]));
	mapDecPlatforms["pit"].push_back(new olc::Decal(mapPlatforms["pit"][1]));
	mapDecPlatforms["lvl4Exit"].push_back(new olc::Decal(mapPlatforms["lvl4Exit"][0]));
	// boss 4
	mapDecPlatforms["pikachu"].push_back(new olc::Decal(mapPlatforms["pikachu"][0]));
	mapDecPlatforms["electricalSocket"].push_back(new olc::Decal(mapPlatforms["electricalSocket"][0]));

	// lvl 5
	mapDecPlatforms["chunLi"].push_back(new olc::Decal(mapPlatforms["chunLi"][0]));
	mapDecPlatforms["chunLi"].push_back(new olc::Decal(mapPlatforms["chunLi"][1]));
	mapDecPlatforms["chunLi"].push_back(new olc::Decal(mapPlatforms["chunLi"][2]));
	mapDecPlatforms["chunLi"].push_back(new olc::Decal(mapPlatforms["chunLi"][3]));
	mapDecPlatforms["chunLi"].push_back(new olc::Decal(mapPlatforms["chunLi"][4]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][0]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][0]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][0]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][0]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][0]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][0]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][1]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][1]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][1]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][1]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][1]));
	mapDecPlatforms["guileAndRyuTango"].push_back(new olc::Decal(mapPlatforms["guileAndRyuTango"][1]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][0]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][0]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][1]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][1]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][2]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][2]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][3]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][3]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][4]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][4]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][5]));
	mapDecPlatforms["spyro"].push_back(new olc::Decal(mapPlatforms["spyro"][5]));
	mapDecPlatforms["samusShip"].push_back(new olc::Decal(mapPlatforms["samusShip"][0]));
	mapDecPlatforms["samusShip"].push_back(new olc::Decal(mapPlatforms["samusShip"][1]));
	mapDecPlatforms["samusShip"].push_back(new olc::Decal(mapPlatforms["samusShip"][2]));
	mapDecPlatforms["samusShip"].push_back(new olc::Decal(mapPlatforms["samusShip"][3]));
	mapDecPlatforms["olimarShip"].push_back(new olc::Decal(mapPlatforms["olimarShip"][0]));
	mapDecPlatforms["olimarShip"].push_back(new olc::Decal(mapPlatforms["olimarShip"][1]));
	mapDecPlatforms["olimarShip"].push_back(new olc::Decal(mapPlatforms["olimarShip"][2]));
	mapDecPlatforms["olimarShip"].push_back(new olc::Decal(mapPlatforms["olimarShip"][3]));
	mapDecPlatforms["olimar"].push_back(new olc::Decal(mapPlatforms["olimar"][0]));
	mapDecPlatforms["lvl5Exit"].push_back(new olc::Decal(mapPlatforms["lvl5Exit"][0]));
	// boss 5
	mapDecPlatforms["MKSword"].push_back(new olc::Decal(mapPlatforms["MKSword"][0]));
	mapDecPlatforms["fence"].push_back(new olc::Decal(mapPlatforms["fence"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][0]));
	mapDecPlatforms["soldierWaddleDee"].push_back(new olc::Decal(mapPlatforms["soldierWaddleDee"][1]));
	mapDecPlatforms["sirKibble"].push_back(new olc::Decal(mapPlatforms["sirKibble"][0]));
	mapDecPlatforms["spectateBladeKnight"].push_back(new olc::Decal(mapPlatforms["spectateBladeKnight"][0]));
	mapDecPlatforms["waddleDoo"].push_back(new olc::Decal(mapPlatforms["waddleDoo"][0]));

	// lvl 6
	mapDecPlatforms["elementWood"].push_back(new olc::Decal(mapPlatforms["elementWood"][0]));
	mapDecPlatforms["elementEarth"].push_back(new olc::Decal(mapPlatforms["elementEarth"][0]));
	mapDecPlatforms["elementMetal"].push_back(new olc::Decal(mapPlatforms["elementMetal"][0]));
	mapDecPlatforms["elementWater"].push_back(new olc::Decal(mapPlatforms["elementWater"][0]));
	mapDecPlatforms["elementFire"].push_back(new olc::Decal(mapPlatforms["elementFire"][0]));
	mapDecPlatforms["deedeedeeStatue"].push_back(new olc::Decal(mapPlatforms["deedeedeeStatue"][0]));
	mapDecPlatforms["deathPanel"].push_back(new olc::Decal(mapPlatforms["deathPanel"][0]));
	mapDecPlatforms["greatTree"].push_back(new olc::Decal(mapPlatforms["greatTree"][0]));
	mapDecPlatforms["greatBranch"].push_back(new olc::Decal(mapPlatforms["greatBranch"][0]));
	mapDecPlatforms["groundGate"].push_back(new olc::Decal(mapPlatforms["groundGate"][0]));
	mapDecPlatforms["cave"].push_back(new olc::Decal(mapPlatforms["cave"][0]));
	mapDecPlatforms["diglett"].push_back(new olc::Decal(mapPlatforms["diglett"][0]));
	mapDecPlatforms["groundMound"].push_back(new olc::Decal(mapPlatforms["groundMound"][0]));
	mapDecPlatforms["carrots"].push_back(new olc::Decal(mapPlatforms["carrots"][0]));
	mapDecPlatforms["leftGrdWall"].push_back(new olc::Decal(mapPlatforms["leftGrdWall"][0]));
	mapDecPlatforms["rightGrdWall"].push_back(new olc::Decal(mapPlatforms["rightGrdWall"][0]));
	mapDecPlatforms["midGrdWall"].push_back(new olc::Decal(mapPlatforms["midGrdWall"][0]));
	mapDecPlatforms["miningKremling"].push_back(new olc::Decal(mapPlatforms["miningKremling"][0]));
	mapDecPlatforms["miningKremling"].push_back(new olc::Decal(mapPlatforms["miningKremling"][0]));
	mapDecPlatforms["miningKremling"].push_back(new olc::Decal(mapPlatforms["miningKremling"][1]));
	mapDecPlatforms["miningKremling"].push_back(new olc::Decal(mapPlatforms["miningKremling"][1]));
	mapDecPlatforms["miningKremling"].push_back(new olc::Decal(mapPlatforms["miningKremling"][2]));
	mapDecPlatforms["miningKremling"].push_back(new olc::Decal(mapPlatforms["miningKremling"][2]));
	mapDecPlatforms["miningKremling"].push_back(new olc::Decal(mapPlatforms["miningKremling"][2]));
	mapDecPlatforms["mukUsedToxic"].push_back(new olc::Decal(mapPlatforms["mukUsedToxic"][0]));
	mapDecPlatforms["mukUsedToxic"].push_back(new olc::Decal(mapPlatforms["mukUsedToxic"][1]));
	mapDecPlatforms["mukUsedToxic"].push_back(new olc::Decal(mapPlatforms["mukUsedToxic"][2]));
	mapDecPlatforms["mukUsedToxic"].push_back(new olc::Decal(mapPlatforms["mukUsedToxic"][3]));
	mapDecPlatforms["mukUsedToxic"].push_back(new olc::Decal(mapPlatforms["mukUsedToxic"][4]));
	mapDecPlatforms["metalVent"].push_back(new olc::Decal(mapPlatforms["metalVent"][0]));
	mapDecPlatforms["abbath"].push_back(new olc::Decal(mapPlatforms["abbath"][0]));
	mapDecPlatforms["mettaton"].push_back(new olc::Decal(mapPlatforms["mettaton"][0]));
	mapDecPlatforms["mettaton"].push_back(new olc::Decal(mapPlatforms["mettaton"][0]));
	mapDecPlatforms["mettaton"].push_back(new olc::Decal(mapPlatforms["mettaton"][1]));
	mapDecPlatforms["mettaton"].push_back(new olc::Decal(mapPlatforms["mettaton"][1]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][0]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][0]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][1]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][1]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][2]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][2]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][3]));
	mapDecPlatforms["waddlingRegisteel"].push_back(new olc::Decal(mapPlatforms["waddlingRegisteel"][3]));
	mapDecPlatforms["A350"].push_back(new olc::Decal(mapPlatforms["A350"][0]));
	mapDecPlatforms["waterFall"].push_back(new olc::Decal(mapPlatforms["waterFall"][0]));
	mapDecPlatforms["waterFall"].push_back(new olc::Decal(mapPlatforms["waterFall"][0]));
	mapDecPlatforms["waterFall"].push_back(new olc::Decal(mapPlatforms["waterFall"][1]));
	mapDecPlatforms["waterFall"].push_back(new olc::Decal(mapPlatforms["waterFall"][1]));
	mapDecPlatforms["waterFall"].push_back(new olc::Decal(mapPlatforms["waterFall"][2]));
	mapDecPlatforms["waterFall"].push_back(new olc::Decal(mapPlatforms["waterFall"][2]));
	mapDecPlatforms["waterFlow"].push_back(new olc::Decal(mapPlatforms["waterFlow"][0]));
	mapDecPlatforms["waterFlow"].push_back(new olc::Decal(mapPlatforms["waterFlow"][0]));
	mapDecPlatforms["waterFlow"].push_back(new olc::Decal(mapPlatforms["waterFlow"][1]));
	mapDecPlatforms["waterFlow"].push_back(new olc::Decal(mapPlatforms["waterFlow"][1]));
	mapDecPlatforms["waterFlow"].push_back(new olc::Decal(mapPlatforms["waterFlow"][2]));
	mapDecPlatforms["waterFlow"].push_back(new olc::Decal(mapPlatforms["waterFlow"][2]));
	mapDecPlatforms["frisk"].push_back(new olc::Decal(mapPlatforms["frisk"][0]));
	mapDecPlatforms["kingZora"].push_back(new olc::Decal(mapPlatforms["kingZora"][0]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][0]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][0]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][0]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][1]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][1]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][1]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][2]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][2]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][2]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][3]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][3]));
	mapDecPlatforms["enguardeTheSwordfish"].push_back(new olc::Decal(mapPlatforms["enguardeTheSwordfish"][3]));
	mapDecPlatforms["fireStatueLeft"].push_back(new olc::Decal(mapPlatforms["fireStatueLeft"][0]));
	mapDecPlatforms["fireStatueRight"].push_back(new olc::Decal(mapPlatforms["fireStatueRight"][0]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][0]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][0]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][0]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][1]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][1]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][1]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][2]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][2]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][2]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][3]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][3]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][3]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][4]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][4]));
	mapDecPlatforms["fire"].push_back(new olc::Decal(mapPlatforms["fire"][4]));
	mapDecPlatforms["burntLink"].push_back(new olc::Decal(mapPlatforms["burntLink"][0]));
	mapDecPlatforms["torch"].push_back(new olc::Decal(mapPlatforms["torch"][0]));
	mapDecPlatforms["torchLeft"].push_back(new olc::Decal(mapPlatforms["torchLeft"][0]));
	mapDecPlatforms["torchRight"].push_back(new olc::Decal(mapPlatforms["torchRight"][0]));
	mapDecPlatforms["charizardFamily"].push_back(new olc::Decal(mapPlatforms["charizardFamily"][0]));
	mapDecPlatforms["gasCooker"].push_back(new olc::Decal(mapPlatforms["gasCooker"][0]));
	mapDecPlatforms["entei"].push_back(new olc::Decal(mapPlatforms["entei"][0]));
	mapDecPlatforms["entei"].push_back(new olc::Decal(mapPlatforms["entei"][0]));
	mapDecPlatforms["entei"].push_back(new olc::Decal(mapPlatforms["entei"][0]));
	mapDecPlatforms["entei"].push_back(new olc::Decal(mapPlatforms["entei"][1]));
	mapDecPlatforms["entei"].push_back(new olc::Decal(mapPlatforms["entei"][1]));
	mapDecPlatforms["entei"].push_back(new olc::Decal(mapPlatforms["entei"][1]));
	mapDecPlatforms["elementsDirections"].push_back(new olc::Decal(mapPlatforms["elementsDirections"][0]));
	mapDecPlatforms["lvl6Exit"].push_back(new olc::Decal(mapPlatforms["lvl6Exit"][0]));
	// boss 6
	mapDecPlatforms["waterMark"].push_back(new olc::Decal(mapPlatforms["waterMark"][0]));
	mapDecPlatforms["waterMark"].push_back(new olc::Decal(mapPlatforms["waterMark"][0]));
	mapDecPlatforms["waterMark"].push_back(new olc::Decal(mapPlatforms["waterMark"][1]));
	mapDecPlatforms["waterMark"].push_back(new olc::Decal(mapPlatforms["waterMark"][1]));
	mapDecPlatforms["fountainFlow"].push_back(new olc::Decal(mapPlatforms["fountainFlow"][0]));
	mapDecPlatforms["fountainFlow"].push_back(new olc::Decal(mapPlatforms["fountainFlow"][0]));
	mapDecPlatforms["fountainFlow"].push_back(new olc::Decal(mapPlatforms["fountainFlow"][1]));
	mapDecPlatforms["fountainFlow"].push_back(new olc::Decal(mapPlatforms["fountainFlow"][1]));
	mapDecPlatforms["meleeFox"].push_back(new olc::Decal(mapPlatforms["meleeFox"][0]));
	mapDecPlatforms["meleeFox"].push_back(new olc::Decal(mapPlatforms["meleeFox"][1]));
	mapDecPlatforms["meleeFox"].push_back(new olc::Decal(mapPlatforms["meleeFox"][2]));
	mapDecPlatforms["meleeFox"].push_back(new olc::Decal(mapPlatforms["meleeFox"][3]));
	mapDecPlatforms["meleeMarth"].push_back(new olc::Decal(mapPlatforms["meleeMarth"][0]));
	mapDecPlatforms["meleeMarth"].push_back(new olc::Decal(mapPlatforms["meleeMarth"][1]));
	mapDecPlatforms["meleeMarth"].push_back(new olc::Decal(mapPlatforms["meleeMarth"][2]));
	mapDecPlatforms["meleeMarth"].push_back(new olc::Decal(mapPlatforms["meleeMarth"][3]));
	mapDecPlatforms["SSTierMK"].push_back(new olc::Decal(mapPlatforms["SSTierMK"][0]));
	mapDecPlatforms["SSTierMK"].push_back(new olc::Decal(mapPlatforms["SSTierMK"][1]));
	mapDecPlatforms["SSTierMK"].push_back(new olc::Decal(mapPlatforms["SSTierMK"][2]));
	mapDecPlatforms["SSTierMK"].push_back(new olc::Decal(mapPlatforms["SSTierMK"][3]));
	mapDecPlatforms["SSTierMK"].push_back(new olc::Decal(mapPlatforms["SSTierMK"][4]));
	mapDecPlatforms["BossBckGrd05_arena"].push_back(new olc::Decal(mapPlatforms["BossBckGrd05_arena"][0]));
#pragma endregion

#pragma region Crushers
	mapDecPlatforms["downCrusher"].push_back(new olc::Decal(mapPlatforms["downCrusher"][0]));
	mapDecPlatforms["fountainCrusher"].push_back(new olc::Decal(mapPlatforms["fountainCrusher"][0]));
#pragma endregion

#pragma region HarmfulBlocs
	mapDecPlatforms["spikeBall"].push_back(new olc::Decal(mapPlatforms["spikeBall"][0]));

	mapDecPlatforms["spikeDown"].push_back(new olc::Decal(mapPlatforms["spikeDown"][0]));
	mapDecPlatforms["spikeUp"].push_back(new olc::Decal(mapPlatforms["spikeUp"][0]));
	mapDecPlatforms["spikeLeft"].push_back(new olc::Decal(mapPlatforms["spikeLeft"][0]));
	mapDecPlatforms["spikeRight"].push_back(new olc::Decal(mapPlatforms["spikeRight"][0]));

	mapDecPlatforms["stalactite"].push_back(new olc::Decal(mapPlatforms["stalactite"][0]));
	mapDecPlatforms["stalacleftite"].push_back(new olc::Decal(mapPlatforms["stalacleftite"][0]));
	mapDecPlatforms["stalagmite"].push_back(new olc::Decal(mapPlatforms["stalagmite"][0]));
	mapDecPlatforms["stalacrightite"].push_back(new olc::Decal(mapPlatforms["stalacrightite"][0]));

	mapDecPlatforms["shipFlame"].push_back(new olc::Decal(mapPlatforms["shipFlame"][0]));
	mapDecPlatforms["shipFlame"].push_back(new olc::Decal(mapPlatforms["shipFlame"][1]));
	mapDecPlatforms["shipFlame"].push_back(new olc::Decal(mapPlatforms["shipFlame"][2]));

	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][0]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][0]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][1]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][1]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][2]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][2]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][3]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][3]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][4]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][4]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][5]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][5]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][6]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][6]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][7]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][7]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][8]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][8]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][9]));
	mapDecPlatforms["poison"].push_back(new olc::Decal(mapPlatforms["poison"][9]));
#pragma endregion

	return mapDecPlatforms;
}

void cDynamicMovingPlatform::TriggerMovement()
{
	bTriggered = true;
}

void cDynamicMovingPlatform::UntriggerMovement()
{
	bTriggered = false;
}

float cDynamicMovingPlatform::GetNormalizedWidth()
{
	return fDynWidth / engine->GetTileWidth();
}

float cDynamicMovingPlatform::GetNormalizedHeight()
{
	return fDynHeight / engine->GetTileHeight();
}

bool cDynamicMovingPlatform::IsIced()
{
	return bIced;
}

bool cDynamicMovingPlatform::IsHarmfulblocTangible()
{
	return bTangible;
}

olc::Sprite* cDynamicMovingPlatform::GetCurrentSprite()
{
	return mapStates[nCurrentFrame]->sprite;
}

std::string cDynamicMovingPlatform::GetIdentifier()
{
	return sIdentifier;
}

void cDynamicMovingPlatform::SetIdentifier(std::string id)
{
	sIdentifier = id;
}

void cDynamicMovingPlatform::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	// Depends on the type of platform
	// 	   EXAMPLE FOR A CIRCULAR MOVEMENT PLATFORM
	//if (bTriggered)
	//{
	//	theta += fElapsedTime;
	//	vx = cosf(theta);
	//	vy = sinf(theta);
	//}
}

void cDynamicMovingPlatform::LinkPtfm(cDynamicMovingPlatform* ptfm)
{
	linkedPtfm = ptfm;
}

bool cDynamicMovingPlatform::IsLinkedToPreviousPtfm()
{
	return bLinkedToPreviousPtfm;
}
