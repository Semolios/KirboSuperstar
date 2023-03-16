#include "Platformer_DynamicWind.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicWind::engine = nullptr;

cDynamicWind::cDynamicWind(float ox, float oy, std::vector<olc::Decal*> map, std::wstring direction, float power) : cDynamic("wind")
{
	px = ox;
	py = oy;
	mapStates = map;
	bFriendly = true;
	fPower = power;
	sDirection = engine->ToStr(direction);
}

cDynamicWind::~cDynamicWind()
{
	delete hitbox;
}

void cDynamicWind::DrawSelf(float ox, float oy)
{
	olc::vf2d pos;
	pos.x = (px - ox) * engine->GetTileWidth();
	pos.y = (py - oy) * engine->GetTileHeight();
	engine->DrawDecal(pos, mapStates[nCurrentFrame]);
}

void cDynamicWind::Update(float fElapsedTime, float playerX, float playerY)
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

void cDynamicWind::UpdateHitbox(float cameraOffsetX, float cameraOffsetY)
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
	//hitbox->Draw(engine, olc::CYAN);
}

std::string cDynamicWind::GetDirection()
{
	return sDirection;
}

float cDynamicWind::GetPower()
{
	return fPower;
}

olc::Sprite* cDynamicWind::GetCurrentSprite()
{
	return mapStates[nCurrentFrame]->sprite;
}

std::map<std::string, std::vector<olc::Sprite*>> cDynamicWind::LoadWindSprites()
{
	std::map<std::string, std::vector<olc::Sprite*>> mapWinds;

	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind00.png"));
	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind01.png"));
	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind02.png"));
	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind03.png"));
	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind04.png"));
	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind05.png"));
	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind06.png"));
	mapWinds["down"].push_back(new olc::Sprite("assets/gfx/downWind07.png"));

	return mapWinds;
}

std::map<std::string, std::vector<olc::Decal*>> cDynamicWind::LoadWindDecals(std::map<std::string, std::vector<olc::Sprite*>> mapWinds)
{
	std::map<std::string, std::vector<olc::Decal*>> mapDecWinds;

	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][0]));
	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][1]));
	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][2]));
	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][3]));
	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][4]));
	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][5]));
	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][6]));
	mapDecWinds["down"].push_back(new olc::Decal(mapWinds["down"][7]));

	return mapDecWinds;
}
