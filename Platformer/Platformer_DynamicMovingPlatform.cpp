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
		((playerLeftCorner.x >= ptfmTopLeftCorner.x) && (playerLeftCorner.x <= ptfmBotRightCorner.x) && (playerLeftCorner.y >= ptfmTopLeftCorner.y) && (playerLeftCorner.y <= ptfmBotRightCorner.y)) ||
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
		((leftCornerX >= ptfmTopLeftCorner.x) && (leftCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY <= ptfmTopLeftCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY <= ptfmTopLeftCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		// playerPY is in the platform and newPlayerPY is below platform
		((leftCornerX >= ptfmTopLeftCorner.x) && (leftCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		// playerPY is in the Previous platform position
		((leftCornerX >= prevPtfmTopLeftCorner.x) && (leftCornerX <= prevPtfmBotRightCorner.x) && (fPlayerPosY >= prevPtfmTopLeftCorner.y) && (fPlayerPosY <= prevPtfmBotRightCorner.y)) ||
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

	// TODO TESTER AVEC LES CRUSHERS
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

	// TODO TESTER AVEC LES CRUSHERS
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
		((playerLeftCorner.x >= ptfmTopLeftCorner.x) && (playerLeftCorner.x <= ptfmBotRightCorner.x) && (playerLeftCorner.y >= ptfmTopLeftCorner.y) && (playerLeftCorner.y <= ptfmBotRightCorner.y)) ||
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
		((leftCornerX >= ptfmTopLeftCorner.x) && (leftCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmTopLeftCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmTopLeftCorner.y)) ||
		// playerPY is in the platform and newPlayerPY is above platform
		((leftCornerX >= ptfmTopLeftCorner.x) && (leftCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmBotRightCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) && (rightCornerX <= ptfmBotRightCorner.x) && (fPlayerPosY >= ptfmTopLeftCorner.y) && (fPlayerPosY <= ptfmBotRightCorner.y) && (fNewPlayerPosY <= ptfmBotRightCorner.y)) ||
		// playerPY is in the Previous platform position
		((leftCornerX >= prevPtfmTopLeftCorner.x) && (leftCornerX <= prevPtfmBotRightCorner.x) && (fPlayerPosY >= prevPtfmTopLeftCorner.y) && (fPlayerPosY <= prevPtfmBotRightCorner.y)) ||
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

	mapPlatforms["basic"].push_back(new olc::Sprite("assets/gfx/basicPlatform.png"));

	mapPlatforms["ice"].push_back(new olc::Sprite("assets/gfx/icePlatform.png"));

	mapPlatforms["wall"].push_back(new olc::Sprite("assets/gfx/wall.png"));

	mapPlatforms["ceiling"].push_back(new olc::Sprite("assets/gfx/ceiling.png"));

	mapPlatforms["downCrusher"].push_back(new olc::Sprite("assets/gfx/downCrusher.png"));

	mapPlatforms["spikeBloc"].push_back(new olc::Sprite("assets/gfx/spikeBloc.png"));

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
