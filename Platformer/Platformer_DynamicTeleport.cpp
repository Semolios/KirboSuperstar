#include "Platformer_DynamicTeleport.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicTeleport::engine = nullptr;

cDynamicTeleport::cDynamicTeleport(float ax, float ay, float bx, float by, std::vector<olc::Decal*> map) : cDynamic("teleport")
{
	px = ax;
	py = ay;
	fDestX = bx;
	fDestY = by;
	mapStates = map;
	destHitbox = new cHitbox();
}

cDynamicTeleport::~cDynamicTeleport()
{
	delete hitbox;
	delete destHitbox;
}

void cDynamicTeleport::DrawSelf(float ox, float oy)
{
	olc::vf2d pos;
	pos.x = (px - ox) * engine->GetTileWidth();
	pos.y = (py - oy) * engine->GetTileHeight();
	engine->DrawDecal(pos, mapStates[nCurrentFrame]);
}

void cDynamicTeleport::DrawDest(float ox, float oy)
{
	olc::vf2d pos;
	pos.x = (fDestX - ox) * engine->GetTileWidth();
	pos.y = (fDestY - oy) * engine->GetTileHeight();
	engine->DrawDecal(pos, mapStates[nCurrentFrame]);
}

void cDynamicTeleport::Update(float fElapsedTime, float playerX, float playerY)
{
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
}

void cDynamicTeleport::UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
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

void cDynamicTeleport::UpdateDestHitbox(float cameraOffsetX, float cameraOffsetY)
{
	destHitbox->SetPos(
		(fDestX - cameraOffsetX) * engine->GetTileWidth() + (fDynWidth / 2.0f),
		(fDestY - cameraOffsetY) * engine->GetTileHeight() + (fDynHeight / 2.0f)
	);
	destHitbox->SetAngle(0.0f);
	destHitbox->AddPoint(-fDynWidth / 2.0f, -fDynHeight / 2.0f);
	destHitbox->AddPoint(-fDynWidth / 2.0f, +fDynHeight / 2.0f);
	destHitbox->AddPoint(+fDynWidth / 2.0f, +fDynHeight / 2.0f);
	destHitbox->AddPoint(+fDynWidth / 2.0f, -fDynHeight / 2.0f);
	destHitbox->ResizeP(4);

	for (int i = 0; i < destHitbox->GetOSize(); i++)
	{
		// 2D Rotation Transform + 2D Translation
		destHitbox->SetP(i,
						 (destHitbox->GetOIX(i) * cosf(destHitbox->GetAngle())) - (destHitbox->GetOIY(i) * sinf(destHitbox->GetAngle())) + destHitbox->GetPosX(),
						 (destHitbox->GetOIX(i) * sinf(destHitbox->GetAngle())) + (destHitbox->GetOIY(i) * cosf(destHitbox->GetAngle())) + destHitbox->GetPosY()
		);
	}

	destHitbox->ClearO();

	// debug AOE
	//destHitbox->Draw(engine, olc::BLUE);
}

float cDynamicTeleport::GetNormalizedWidth()
{
	return fDynWidth / engine->GetTileWidth();
}

float cDynamicTeleport::GetNormalizedHeight()
{
	return fDynHeight / engine->GetTileHeight();
}

float cDynamicTeleport::GetDestX()
{
	return fDestX;
}

float cDynamicTeleport::GetDestY()
{
	return fDestY;
}

cHitbox* cDynamicTeleport::GetDestHitbox()
{
	return destHitbox;
}

olc::Sprite* cDynamicTeleport::GetCurrentSprite()
{
	return mapStates[nCurrentFrame]->sprite;
}

std::map<std::string, std::vector<olc::Sprite*>> cDynamicTeleport::LoadTeleportsSprites()
{
	std::map<std::string, std::vector<olc::Sprite*>> mapTeleports;

	mapTeleports["door"].push_back(new olc::Sprite("assets/gfx/door.png"));

	mapTeleports["iceDoor"].push_back(new olc::Sprite("assets/gfx/iceDoor.png"));

	return mapTeleports;
}

std::map<std::string, std::vector<olc::Decal*>> cDynamicTeleport::LoadTeleportsDecals(std::map<std::string, std::vector<olc::Sprite*>> mapTeleports)
{
	std::map<std::string, std::vector<olc::Decal*>> mapDecTeleports;

	mapDecTeleports["door"].push_back(new olc::Decal(mapTeleports["door"][0]));

	mapDecTeleports["iceDoor"].push_back(new olc::Decal(mapTeleports["iceDoor"][0]));

	return mapDecTeleports;
}
