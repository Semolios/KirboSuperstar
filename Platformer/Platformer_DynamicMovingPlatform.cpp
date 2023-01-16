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
	olc::GFX2D::Transform2D t;
	t.Translate(-fDynWidth / 2.0f, -fDynHeight / 2.0f);
	t.Translate((px - ox + (GetNormalizedWidth() / 2.0f)) * engine->GetTileWidth(), (py - oy + (GetNormalizedHeight() / 2.0f)) * engine->GetTileHeight());
	olc::GFX2D::DrawSprite(mapStates[nCurrentFrame], t);
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
	mapPlatforms["arrowPanel"].push_back(new olc::Sprite("assets/gfx/arrowPanel.png"));
	mapPlatforms["tree"].push_back(new olc::Sprite("assets/gfx/tree.png"));
	mapPlatforms["bush"].push_back(new olc::Sprite("assets/gfx/bush.png"));
	mapPlatforms["mojo"].push_back(new olc::Sprite("assets/gfx/mojo.png"));

	mapPlatforms["hillYellow"].push_back(new olc::Sprite("assets/gfx/hillYellow.png"));
	mapPlatforms["hillRed"].push_back(new olc::Sprite("assets/gfx/hillRed.png"));
	mapPlatforms["hillsRed"].push_back(new olc::Sprite("assets/gfx/hillsRed.png"));

	mapPlatforms["snowman"].push_back(new olc::Sprite("assets/gfx/snowman.png"));
	mapPlatforms["snowtree"].push_back(new olc::Sprite("assets/gfx/snowtree.png"));

	mapPlatforms["upArrow"].push_back(new olc::Sprite("assets/gfx/upArrowPanel.png"));
	mapPlatforms["upArrow2"].push_back(new olc::Sprite("assets/gfx/upArrowPanel2.png"));
	mapPlatforms["rightArrow"].push_back(new olc::Sprite("assets/gfx/rightArrowPanel.png"));
	mapPlatforms["rightArrow2"].push_back(new olc::Sprite("assets/gfx/rightArrowPanel2.png"));
	mapPlatforms["leftArrow"].push_back(new olc::Sprite("assets/gfx/leftArrowPanel.png"));
	mapPlatforms["leftArrow2"].push_back(new olc::Sprite("assets/gfx/leftArrowPanel2.png"));
	mapPlatforms["downArrow"].push_back(new olc::Sprite("assets/gfx/downArrowPanel.png"));
	mapPlatforms["downArrow2"].push_back(new olc::Sprite("assets/gfx/downArrowPanel2.png"));

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
