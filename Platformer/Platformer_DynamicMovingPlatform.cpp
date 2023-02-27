#include "Platformer_DynamicMovingPlatform.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicMovingPlatform::engine = nullptr;

cDynamicMovingPlatform::cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring iced) : cDynamic("movingPlatform")
{
	px = ox;
	py = oy;
	mapStates = map;
	bFriendly = true;
	bIced = (iced == L"1");
}

cDynamicMovingPlatform::cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Sprite*> map) : cDynamic("scenery")
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
	engine->SetPixelMode(olc::Pixel::ALPHA);
    olc::vf2d pos;
	pos.x = (px - ox + (GetNormalizedWidth()  / 2.0f)) * engine->GetTileWidth()  - fDynWidth  / 2.0f;
	pos.y = (py - oy + (GetNormalizedHeight() / 2.0f)) * engine->GetTileHeight() - fDynHeight / 2.0f;
	engine->DrawSprite(pos, mapStates[nCurrentFrame]);
	engine->SetPixelMode(olc::Pixel::NORMAL);
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

		fDynWidth = mapStates[nCurrentFrame]->width;
		fDynHeight = mapStates[nCurrentFrame]->height;
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
	// boss 1
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros00.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros00.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros00.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros01.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros01.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros01.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros02.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros02.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros02.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros03.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros03.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros03.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros04.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros04.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros04.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros05.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros05.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros05.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros06.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros06.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros06.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros07.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros07.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros07.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros08.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros08.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros08.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros09.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros09.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros09.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros10.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros10.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros10.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros11.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros11.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros11.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros12.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros12.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros12.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros13.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros13.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros13.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros14.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros14.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros14.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros15.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros15.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros15.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros08.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros08.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros08.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros09.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros09.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros09.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros10.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros10.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros10.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros11.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros11.png"));
	mapPlatforms["dancingPoppyBros"].push_back(new olc::Sprite("assets/gfx/dancingPoppyBros11.png"));
	mapPlatforms["skullKidBush"].push_back(new olc::Sprite("assets/gfx/skullKidBush.png"));
	mapPlatforms["chikoritaBush"].push_back(new olc::Sprite("assets/gfx/chikoritaBush.png"));
	mapPlatforms["coolBugFact"].push_back(new olc::Sprite("assets/gfx/coolBugFact.png"));

	// lvl 2
	mapPlatforms["hillYellow"].push_back(new olc::Sprite("assets/gfx/hillYellow.png"));
	mapPlatforms["hillRed"].push_back(new olc::Sprite("assets/gfx/hillRed.png"));
	mapPlatforms["hillsRed"].push_back(new olc::Sprite("assets/gfx/hillsRed.png"));
	// ------- very long animation -------
	for (int i = 0; i < 90; i++)
	{
		// wait 3 seconds before eating the little hill
        mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill00.png"));
	}
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill01.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill02.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill01.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill02.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill01.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill02.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill01.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill02.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill03.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill04.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill05.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill05.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill06.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill06.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill07.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill08.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill09.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill10.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill10.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill11.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill11.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill12.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill12.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill13.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill13.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill14.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill14.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill15.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill15.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill16.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill16.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill16.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill16.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill17.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill18.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill19.png"));
	mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill20.png"));
	for (int i = 0; i < 90; i++)
	{
		// wait 3 seconds before the hill respawn
        mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill21.png"));
	}
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill22.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill23.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill24.png"));
    mapPlatforms["cannibalHill"].push_back(new olc::Sprite("assets/gfx/cannibalHill25.png"));
	// ------- very long animation -------
    mapPlatforms["mustacheHill"].push_back(new olc::Sprite("assets/gfx/mustacheHill.png"));
    mapPlatforms["wryHill"].push_back(new olc::Sprite("assets/gfx/wryHill.png"));
    mapPlatforms["goomba"].push_back(new olc::Sprite("assets/gfx/goomba.png"));
    mapPlatforms["greenKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/greenKoopaTroopa.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa00.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa00.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa01.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa01.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa02.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa02.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa02.png"));
    mapPlatforms["flyingRedKoopaTroopa"].push_back(new olc::Sprite("assets/gfx/flyingRedKoopaTroopa03.png"));
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
	// boss 2
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy00.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy00.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy01.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy01.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy02.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy02.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy03.png"));
	mapPlatforms["cappy"].push_back(new olc::Sprite("assets/gfx/cappy03.png"));
	mapPlatforms["burningCappy"].push_back(new olc::Sprite("assets/gfx/burningCappy00.png"));
	mapPlatforms["burningCappy"].push_back(new olc::Sprite("assets/gfx/burningCappy01.png"));

	// lvl 3
	mapPlatforms["snowman"].push_back(new olc::Sprite("assets/gfx/snowman.png"));
	mapPlatforms["snowtree"].push_back(new olc::Sprite("assets/gfx/snowtree.png"));
	mapPlatforms["scrat"].push_back(new olc::Sprite("assets/gfx/scrat.png"));
	mapPlatforms["frozenMeowthAndCleffa"].push_back(new olc::Sprite("assets/gfx/frozenMeowthAndCleffa.png"));
	mapPlatforms["sunkenShip0003"].push_back(new olc::Sprite("assets/gfx/sunkenShip0003.png"));
	mapPlatforms["sunkenShip0004"].push_back(new olc::Sprite("assets/gfx/sunkenShip0004.png"));
	mapPlatforms["sunkenShip0006"].push_back(new olc::Sprite("assets/gfx/sunkenShip0006.png"));
	mapPlatforms["sunkenShip0007"].push_back(new olc::Sprite("assets/gfx/sunkenShip0007.png"));
	mapPlatforms["sunkenShip0009"].push_back(new olc::Sprite("assets/gfx/sunkenShip0009.png"));
	mapPlatforms["sunkenShip0010"].push_back(new olc::Sprite("assets/gfx/sunkenShip0010.png"));
	mapPlatforms["sunkenShip0011"].push_back(new olc::Sprite("assets/gfx/sunkenShip0011.png"));
	mapPlatforms["sunkenShip0012"].push_back(new olc::Sprite("assets/gfx/sunkenShip0012.png"));
	mapPlatforms["sunkenShip0013"].push_back(new olc::Sprite("assets/gfx/sunkenShip0013.png"));
	mapPlatforms["sunkenShip0014"].push_back(new olc::Sprite("assets/gfx/sunkenShip0014.png"));
	mapPlatforms["sunkenShip0015"].push_back(new olc::Sprite("assets/gfx/sunkenShip0015.png"));
	mapPlatforms["sunkenShip0018"].push_back(new olc::Sprite("assets/gfx/sunkenShip0018.png"));
	mapPlatforms["sunkenShip0019"].push_back(new olc::Sprite("assets/gfx/sunkenShip0019.png"));
	mapPlatforms["sunkenShip0102"].push_back(new olc::Sprite("assets/gfx/sunkenShip0102.png"));
	mapPlatforms["sunkenShip0103"].push_back(new olc::Sprite("assets/gfx/sunkenShip0103.png"));
	mapPlatforms["sunkenShip0104"].push_back(new olc::Sprite("assets/gfx/sunkenShip0104.png"));
	mapPlatforms["sunkenShip0106"].push_back(new olc::Sprite("assets/gfx/sunkenShip0106.png"));
	mapPlatforms["sunkenShip0107"].push_back(new olc::Sprite("assets/gfx/sunkenShip0107.png"));
	mapPlatforms["sunkenShip0108"].push_back(new olc::Sprite("assets/gfx/sunkenShip0108.png"));
	mapPlatforms["sunkenShip0110"].push_back(new olc::Sprite("assets/gfx/sunkenShip0110.png"));
	mapPlatforms["sunkenShip0112"].push_back(new olc::Sprite("assets/gfx/sunkenShip0112.png"));
	mapPlatforms["sunkenShip0113"].push_back(new olc::Sprite("assets/gfx/sunkenShip0113.png"));
	mapPlatforms["sunkenShip0114"].push_back(new olc::Sprite("assets/gfx/sunkenShip0114.png"));
	mapPlatforms["sunkenShip0115"].push_back(new olc::Sprite("assets/gfx/sunkenShip0115.png"));
	mapPlatforms["sunkenShip0116"].push_back(new olc::Sprite("assets/gfx/sunkenShip0116.png"));
	mapPlatforms["sunkenShip0117"].push_back(new olc::Sprite("assets/gfx/sunkenShip0117.png"));
	mapPlatforms["sunkenShip0118"].push_back(new olc::Sprite("assets/gfx/sunkenShip0118.png"));
	mapPlatforms["sunkenShip0119"].push_back(new olc::Sprite("assets/gfx/sunkenShip0119.png"));
	mapPlatforms["sunkenShip0200"].push_back(new olc::Sprite("assets/gfx/sunkenShip0200.png"));
	mapPlatforms["sunkenShip0201"].push_back(new olc::Sprite("assets/gfx/sunkenShip0201.png"));
	mapPlatforms["sunkenShip0202"].push_back(new olc::Sprite("assets/gfx/sunkenShip0202.png"));
	mapPlatforms["sunkenShip0203"].push_back(new olc::Sprite("assets/gfx/sunkenShip0203.png"));
	mapPlatforms["sunkenShip0204"].push_back(new olc::Sprite("assets/gfx/sunkenShip0204.png"));
	mapPlatforms["sunkenShip0206"].push_back(new olc::Sprite("assets/gfx/sunkenShip0206.png"));
	mapPlatforms["sunkenShip0207"].push_back(new olc::Sprite("assets/gfx/sunkenShip0207.png"));
	mapPlatforms["sunkenShip0208"].push_back(new olc::Sprite("assets/gfx/sunkenShip0208.png"));
	mapPlatforms["sunkenShip0215"].push_back(new olc::Sprite("assets/gfx/sunkenShip0215.png"));
	mapPlatforms["sunkenShip0216"].push_back(new olc::Sprite("assets/gfx/sunkenShip0216.png"));
	mapPlatforms["sunkenShip0217"].push_back(new olc::Sprite("assets/gfx/sunkenShip0217.png"));
	mapPlatforms["sunkenShip0218"].push_back(new olc::Sprite("assets/gfx/sunkenShip0218.png"));
	mapPlatforms["sunkenShip0219"].push_back(new olc::Sprite("assets/gfx/sunkenShip0219.png"));
	mapPlatforms["sunkenShip0300"].push_back(new olc::Sprite("assets/gfx/sunkenShip0300.png"));
	mapPlatforms["sunkenShip0301"].push_back(new olc::Sprite("assets/gfx/sunkenShip0301.png"));
	mapPlatforms["sunkenShip0302"].push_back(new olc::Sprite("assets/gfx/sunkenShip0302.png"));
	mapPlatforms["sunkenShip0303"].push_back(new olc::Sprite("assets/gfx/sunkenShip0303.png"));
	mapPlatforms["sunkenShip0304"].push_back(new olc::Sprite("assets/gfx/sunkenShip0304.png"));
	mapPlatforms["sunkenShip0305"].push_back(new olc::Sprite("assets/gfx/sunkenShip0305.png"));
	mapPlatforms["sunkenShip0306"].push_back(new olc::Sprite("assets/gfx/sunkenShip0306.png"));
	mapPlatforms["sunkenShip0307"].push_back(new olc::Sprite("assets/gfx/sunkenShip0307.png"));
	mapPlatforms["sunkenShip0308"].push_back(new olc::Sprite("assets/gfx/sunkenShip0308.png"));
	mapPlatforms["sunkenShip0309"].push_back(new olc::Sprite("assets/gfx/sunkenShip0309.png"));
	mapPlatforms["sunkenShip0313"].push_back(new olc::Sprite("assets/gfx/sunkenShip0313.png"));
	mapPlatforms["sunkenShip0314"].push_back(new olc::Sprite("assets/gfx/sunkenShip0314.png"));
	mapPlatforms["sunkenShip0315"].push_back(new olc::Sprite("assets/gfx/sunkenShip0315.png"));
	mapPlatforms["sunkenShip0316"].push_back(new olc::Sprite("assets/gfx/sunkenShip0316.png"));
	mapPlatforms["sunkenShip0317"].push_back(new olc::Sprite("assets/gfx/sunkenShip0317.png"));
	mapPlatforms["sunkenShip0400"].push_back(new olc::Sprite("assets/gfx/sunkenShip0400.png"));
	mapPlatforms["sunkenShip0401"].push_back(new olc::Sprite("assets/gfx/sunkenShip0401.png"));
	mapPlatforms["sunkenShip0402"].push_back(new olc::Sprite("assets/gfx/sunkenShip0402.png"));
	mapPlatforms["sunkenShip0403"].push_back(new olc::Sprite("assets/gfx/sunkenShip0403.png"));
	mapPlatforms["sunkenShip0404"].push_back(new olc::Sprite("assets/gfx/sunkenShip0404.png"));
	mapPlatforms["sunkenShip0405"].push_back(new olc::Sprite("assets/gfx/sunkenShip0405.png"));
	mapPlatforms["sunkenShip0406"].push_back(new olc::Sprite("assets/gfx/sunkenShip0406.png"));
	mapPlatforms["sunkenShip0407"].push_back(new olc::Sprite("assets/gfx/sunkenShip0407.png"));
	mapPlatforms["sunkenShip0408"].push_back(new olc::Sprite("assets/gfx/sunkenShip0408.png"));
	mapPlatforms["sunkenShip0409"].push_back(new olc::Sprite("assets/gfx/sunkenShip0409.png"));
	mapPlatforms["sunkenShip0412"].push_back(new olc::Sprite("assets/gfx/sunkenShip0412.png"));
	mapPlatforms["sunkenShip0413"].push_back(new olc::Sprite("assets/gfx/sunkenShip0413.png"));
	mapPlatforms["sunkenShip0414"].push_back(new olc::Sprite("assets/gfx/sunkenShip0414.png"));
	mapPlatforms["sunkenShip0415"].push_back(new olc::Sprite("assets/gfx/sunkenShip0415.png"));
	mapPlatforms["sunkenShip0416"].push_back(new olc::Sprite("assets/gfx/sunkenShip0416.png"));
	mapPlatforms["sunkenShip0417"].push_back(new olc::Sprite("assets/gfx/sunkenShip0417.png"));
	mapPlatforms["sunkenShip0500"].push_back(new olc::Sprite("assets/gfx/sunkenShip0500.png"));
	mapPlatforms["sunkenShip0501"].push_back(new olc::Sprite("assets/gfx/sunkenShip0501.png"));
	mapPlatforms["sunkenShip0502"].push_back(new olc::Sprite("assets/gfx/sunkenShip0502.png"));
	mapPlatforms["sunkenShip0503"].push_back(new olc::Sprite("assets/gfx/sunkenShip0503.png"));
	mapPlatforms["sunkenShip0504"].push_back(new olc::Sprite("assets/gfx/sunkenShip0504.png"));
	mapPlatforms["sunkenShip0505"].push_back(new olc::Sprite("assets/gfx/sunkenShip0505.png"));
	mapPlatforms["sunkenShip0506"].push_back(new olc::Sprite("assets/gfx/sunkenShip0506.png"));
	mapPlatforms["sunkenShip0507"].push_back(new olc::Sprite("assets/gfx/sunkenShip0507.png"));
	mapPlatforms["sunkenShip0508"].push_back(new olc::Sprite("assets/gfx/sunkenShip0508.png"));
	mapPlatforms["sunkenShip0509"].push_back(new olc::Sprite("assets/gfx/sunkenShip0509.png"));
	mapPlatforms["sunkenShip0510"].push_back(new olc::Sprite("assets/gfx/sunkenShip0510.png"));
	mapPlatforms["sunkenShip0511"].push_back(new olc::Sprite("assets/gfx/sunkenShip0511.png"));
	mapPlatforms["sunkenShip0512"].push_back(new olc::Sprite("assets/gfx/sunkenShip0512.png"));
	mapPlatforms["sunkenShip0513"].push_back(new olc::Sprite("assets/gfx/sunkenShip0513.png"));
	mapPlatforms["sunkenShip0514"].push_back(new olc::Sprite("assets/gfx/sunkenShip0514.png"));
	mapPlatforms["sunkenShip0515"].push_back(new olc::Sprite("assets/gfx/sunkenShip0515.png"));
	mapPlatforms["sunkenShip0516"].push_back(new olc::Sprite("assets/gfx/sunkenShip0516.png"));
	mapPlatforms["sunkenShip0600"].push_back(new olc::Sprite("assets/gfx/sunkenShip0600.png"));
	mapPlatforms["sunkenShip0601"].push_back(new olc::Sprite("assets/gfx/sunkenShip0601.png"));
	mapPlatforms["sunkenShip0602"].push_back(new olc::Sprite("assets/gfx/sunkenShip0602.png"));
	mapPlatforms["sunkenShip0603"].push_back(new olc::Sprite("assets/gfx/sunkenShip0603.png"));
	mapPlatforms["sunkenShip0604"].push_back(new olc::Sprite("assets/gfx/sunkenShip0604.png"));
	mapPlatforms["sunkenShip0605"].push_back(new olc::Sprite("assets/gfx/sunkenShip0605.png"));
	mapPlatforms["sunkenShip0606"].push_back(new olc::Sprite("assets/gfx/sunkenShip0606.png"));
	mapPlatforms["sunkenShip0607"].push_back(new olc::Sprite("assets/gfx/sunkenShip0607.png"));
	mapPlatforms["sunkenShip0608"].push_back(new olc::Sprite("assets/gfx/sunkenShip0608.png"));
	mapPlatforms["sunkenShip0609"].push_back(new olc::Sprite("assets/gfx/sunkenShip0609.png"));
	mapPlatforms["sunkenShip0610"].push_back(new olc::Sprite("assets/gfx/sunkenShip0610.png"));
	mapPlatforms["sunkenShip0611"].push_back(new olc::Sprite("assets/gfx/sunkenShip0611.png"));
	mapPlatforms["sunkenShip0612"].push_back(new olc::Sprite("assets/gfx/sunkenShip0612.png"));
	mapPlatforms["sunkenShip0613"].push_back(new olc::Sprite("assets/gfx/sunkenShip0613.png"));
	mapPlatforms["sunkenShip0614"].push_back(new olc::Sprite("assets/gfx/sunkenShip0614.png"));
	mapPlatforms["sunkenShip0615"].push_back(new olc::Sprite("assets/gfx/sunkenShip0615.png"));
	mapPlatforms["sunkenShip0616"].push_back(new olc::Sprite("assets/gfx/sunkenShip0616.png"));
	mapPlatforms["eskimoPenguin"].push_back(new olc::Sprite("assets/gfx/eskimoPenguin.png"));
	mapPlatforms["frozenKingZora"].push_back(new olc::Sprite("assets/gfx/frozenKingZora.png"));
	mapPlatforms["RaphaelTheRaven"].push_back(new olc::Sprite("assets/gfx/RaphaelTheRaven.png"));
	mapPlatforms["frozenPoppyBros"].push_back(new olc::Sprite("assets/gfx/frozenPoppyBros.png"));
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
	mapPlatforms["pit"].push_back(new olc::Sprite("assets/gfx/pit00.png"));
	mapPlatforms["pit"].push_back(new olc::Sprite("assets/gfx/pit01.png"));
	mapPlatforms["pit"].push_back(new olc::Sprite("assets/gfx/pit01.png"));
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
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango00.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango00.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango00.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango00.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango00.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango01.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango01.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango01.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango01.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango01.png"));
	mapPlatforms["guileAndRyuTango"].push_back(new olc::Sprite("assets/gfx/guileAndRyuTango01.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro00.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro00.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro01.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro01.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro02.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro02.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro03.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro03.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro04.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro04.png"));
	mapPlatforms["spyro"].push_back(new olc::Sprite("assets/gfx/spyro05.png"));
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
	// boss 5
	mapPlatforms["MKSword"].push_back(new olc::Sprite("assets/gfx/MKSword.png"));
	mapPlatforms["fence"].push_back(new olc::Sprite("assets/gfx/fence.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
	mapPlatforms["soldierWaddleDee"].push_back(new olc::Sprite("assets/gfx/soldierWaddleDee00.png"));
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
	mapPlatforms["metalVent"].push_back(new olc::Sprite("assets/gfx/metalVent.png"));
	mapPlatforms["abbath"].push_back(new olc::Sprite("assets/gfx/abbath.png"));
	mapPlatforms["mettaton"].push_back(new olc::Sprite("assets/gfx/mettaton00.png"));
	mapPlatforms["mettaton"].push_back(new olc::Sprite("assets/gfx/mettaton00.png"));
	mapPlatforms["mettaton"].push_back(new olc::Sprite("assets/gfx/mettaton01.png"));
	mapPlatforms["mettaton"].push_back(new olc::Sprite("assets/gfx/mettaton01.png"));
	mapPlatforms["A350"].push_back(new olc::Sprite("assets/gfx/A350.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall00.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall00.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall01.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall01.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall02.png"));
	mapPlatforms["waterFall"].push_back(new olc::Sprite("assets/gfx/waterFall02.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow00.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow00.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow01.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow01.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow02.png"));
	mapPlatforms["waterFlow"].push_back(new olc::Sprite("assets/gfx/waterFlow02.png"));
	mapPlatforms["frisk"].push_back(new olc::Sprite("assets/gfx/frisk.png"));
	mapPlatforms["kingZora"].push_back(new olc::Sprite("assets/gfx/kingZora.png"));
	mapPlatforms["fireStatueLeft"].push_back(new olc::Sprite("assets/gfx/fireStatueLeft.png"));
	mapPlatforms["fireStatueRight"].push_back(new olc::Sprite("assets/gfx/fireStatueRight.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire00.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire00.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire00.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire01.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire01.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire01.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire02.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire02.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire02.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire03.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire03.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire03.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire04.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire04.png"));
	mapPlatforms["fire"].push_back(new olc::Sprite("assets/gfx/fire04.png"));
	mapPlatforms["burntLink"].push_back(new olc::Sprite("assets/gfx/burntLink.png"));
	mapPlatforms["torch"].push_back(new olc::Sprite("assets/gfx/torch.png"));
	mapPlatforms["torchLeft"].push_back(new olc::Sprite("assets/gfx/torchLeft.png"));
	mapPlatforms["torchRight"].push_back(new olc::Sprite("assets/gfx/torchRight.png"));
	mapPlatforms["charizardFamily"].push_back(new olc::Sprite("assets/gfx/charizardFamily.png"));
	mapPlatforms["gasCooker"].push_back(new olc::Sprite("assets/gfx/gasCooker.png"));
	// boss 6
	mapPlatforms["waterMark"].push_back(new olc::Sprite("assets/gfx/waterMark00.png"));
	mapPlatforms["waterMark"].push_back(new olc::Sprite("assets/gfx/waterMark00.png"));
	mapPlatforms["waterMark"].push_back(new olc::Sprite("assets/gfx/waterMark01.png"));
	mapPlatforms["waterMark"].push_back(new olc::Sprite("assets/gfx/waterMark01.png"));
	mapPlatforms["fountainFlow"].push_back(new olc::Sprite("assets/gfx/fountainFlow00.png"));
	mapPlatforms["fountainFlow"].push_back(new olc::Sprite("assets/gfx/fountainFlow00.png"));
	mapPlatforms["fountainFlow"].push_back(new olc::Sprite("assets/gfx/fountainFlow01.png"));
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
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison00.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison01.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison01.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison02.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison02.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison03.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison03.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison04.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison04.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison05.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison05.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison06.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison06.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison07.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison07.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison08.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison08.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison09.png"));
	mapPlatforms["poison"].push_back(new olc::Sprite("assets/gfx/poison09.png"));
#pragma endregion

	return mapPlatforms;
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
	return mapStates[nCurrentFrame];
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
