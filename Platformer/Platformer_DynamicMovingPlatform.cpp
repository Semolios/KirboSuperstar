#include "Platformer_DynamicMovingPlatform.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicMovingPlatform::engine = nullptr;

cDynamicMovingPlatform::cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Sprite*> map) : cDynamic("movingPlatform")
{
	px = ox;
	py = oy;
	mapStates = map;
}

cDynamicMovingPlatform::~cDynamicMovingPlatform()
{
	delete hitbox;
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
		((playerLeftCorner.x >= ptfmTopLeftCorner.x) &&
		 (playerLeftCorner.x <= ptfmBotRightCorner.x) &&
		 (playerLeftCorner.y >= ptfmTopLeftCorner.y) &&
		 (playerLeftCorner.y <= ptfmBotRightCorner.y)) ||
		((playerRightCorner.x >= ptfmTopLeftCorner.x) &&
		 (playerRightCorner.x <= ptfmBotRightCorner.x) &&
		 (playerRightCorner.y >= ptfmTopLeftCorner.y) &&
		 (playerRightCorner.y <= ptfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::TopCollisionWithLag(float leftCornerX, float rightCornerX, float fPlayerPosY, float fNewPlayerPosY)
{
	olc::vf2d ptfmTopLeftCorner(px, py - fHitboxThickness);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()), py + fHitboxThickness);

	return bSolidTop && (
		((leftCornerX >= ptfmTopLeftCorner.x) &&
		 (leftCornerX <= ptfmBotRightCorner.x) &&
		 (fPlayerPosY <= ptfmTopLeftCorner.y) &&
		 (fNewPlayerPosY >= ptfmBotRightCorner.y)) ||
		((rightCornerX >= ptfmTopLeftCorner.x) &&
		 (rightCornerX <= ptfmBotRightCorner.x) &&
		 (fPlayerPosY <= ptfmTopLeftCorner.y) &&
		 (fNewPlayerPosY >= ptfmBotRightCorner.y))
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
		((playerTopCorner.x >= ptfmTopLeftCorner.x) &&
		 (playerTopCorner.x <= ptfmBotRightCorner.x) &&
		 (playerTopCorner.y >= ptfmTopLeftCorner.y) &&
		 (playerTopCorner.y <= ptfmBotRightCorner.y)) ||
		((playerBotCorner.x >= ptfmTopLeftCorner.x) &&
		 (playerBotCorner.x <= ptfmBotRightCorner.x) &&
		 (playerBotCorner.y >= ptfmTopLeftCorner.y) &&
		 (playerBotCorner.y <= ptfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::LeftCollisionWithLag(float topCornerY, float botCornerY, float fPlayerPosX, float fNewPlayerPosX)
{
	olc::vf2d ptfmTopLeftCorner(px - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	return bSolidTop && (
		((fPlayerPosX <= ptfmTopLeftCorner.x) &&
		 (fNewPlayerPosX >= ptfmBotRightCorner.x) &&
		 (topCornerY >= ptfmTopLeftCorner.y) &&
		 (topCornerY <= ptfmBotRightCorner.y)) ||
		((fPlayerPosX <= ptfmTopLeftCorner.x) &&
		 (fNewPlayerPosX >= ptfmBotRightCorner.x) &&
		 (botCornerY >= ptfmTopLeftCorner.y) &&
		 (botCornerY <= ptfmBotRightCorner.y))
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
		((playerTopCorner.x >= ptfmTopLeftCorner.x) &&
		 (playerTopCorner.x <= ptfmBotRightCorner.x) &&
		 (playerTopCorner.y >= ptfmTopLeftCorner.y) &&
		 (playerTopCorner.y <= ptfmBotRightCorner.y)) ||
		((playerBotCorner.x >= ptfmTopLeftCorner.x) &&
		 (playerBotCorner.x <= ptfmBotRightCorner.x) &&
		 (playerBotCorner.y >= ptfmTopLeftCorner.y) &&
		 (playerBotCorner.y <= ptfmBotRightCorner.y))
		);
}

bool cDynamicMovingPlatform::RightCollisionWithLag(float topCornerY, float botCornerY, float fPlayerPosX, float fNewPlayerPosX)
{
	olc::vf2d ptfmTopLeftCorner(px + (fDynWidth / (float)engine->GetTileWidth()) - fHitboxThickness, py);
	olc::vf2d ptfmBotRightCorner(px + (fDynWidth / (float)engine->GetTileWidth()) + fHitboxThickness, py + (fDynHeight / (float)engine->GetTileHeight()));

	return bSolidTop && (
		((fPlayerPosX >= ptfmTopLeftCorner.x) &&
		 (fNewPlayerPosX <= ptfmBotRightCorner.x) &&
		 (topCornerY >= ptfmTopLeftCorner.y) &&
		 (topCornerY <= ptfmBotRightCorner.y)) ||
		((fPlayerPosX >= ptfmTopLeftCorner.x) &&
		 (fNewPlayerPosX <= ptfmBotRightCorner.x) &&
		 (botCornerY >= ptfmTopLeftCorner.y) &&
		 (botCornerY <= ptfmBotRightCorner.y))
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
	return false;
}

bool cDynamicMovingPlatform::BotCollisionWithLag(float leftCornerX, float rightCornerX, float fPlayerPosY, float fNewPlayerPosY)
{
	return false;
}

bool cDynamicMovingPlatform::BotCollisionOneCorner(float CornerX, float cornerY)
{
	return false;
}

std::map<std::string, std::vector<olc::Sprite*>> cDynamicMovingPlatform::LoadMovingPlatformsSprites()
{
	std::map<std::string, std::vector<olc::Sprite*>> mapPlatforms;

	mapPlatforms["basic"].push_back(new olc::Sprite("assets/gfx/basicPlatform.png"));

	mapPlatforms["wall"].push_back(new olc::Sprite("assets/gfx/wall.png"));

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
