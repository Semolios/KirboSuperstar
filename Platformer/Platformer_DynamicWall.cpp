#include "Platformer_DynamicWall.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicWall::engine = nullptr;

cDynamicWall::cDynamicWall(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring leftSolid, std::wstring rightSolid, std::wstring trigger, float trgX, float trgY)
	: cDynamicMovingPlatform(ox, oy, map, L"0")
{
	bSolidTop = false;
	bSolidLeft = (leftSolid == L"1");
	bSolidRight = (rightSolid == L"1");
	bTriggerable = (trigger == L"1");

	fTriggerX = trgX;
	fTriggerY = trgY;

	fOriginY = oy;
}

cDynamicWall::~cDynamicWall()
{
}

void cDynamicWall::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	if (bTriggerable)
	{
		// If Kirbo is close to the trigger Point, open the door
		float fTargetX = playerX - fTriggerX;
		float fTargetY = playerY - fTriggerY;
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

		if (fDistance <= cfTriggerDist)
		{
			bTriggered = true;

			if (bSwitchSound)
			{
				engine->PlaySample("menuBip");
				bSwitchSound = false;
			}

			if (bGateOpenningSound)
			{
				if (fabs(playerX - (GetPX())) < ((((float)engine->ScreenWidth()) / 64.0f) * 1.5f) && fabs(playerY - (GetPY())) < ((((float)engine->ScreenHeight()) / 64.0f) * 1.5f) ||
					fabs(playerX - (GetPX() + GetCurrentSprite()->width / 64.0f)) < ((((float)engine->ScreenWidth()) / 64.0f) * 1.5f) && fabs(playerY - (GetPY())) < ((((float)engine->ScreenHeight()) / 64.0f) * 1.5f) ||
					fabs(playerX - (GetPX())) < ((((float)engine->ScreenWidth()) / 64.0f) * 1.5f) && fabs(playerY - (GetPY() + GetCurrentSprite()->height / 64.0f)) < ((((float)engine->ScreenHeight()) / 64.0f) * 1.5f) ||
					fabs(playerX - (GetPX() + GetCurrentSprite()->width / 64.0f)) < ((((float)engine->ScreenWidth()) / 64.0f) * 1.5f) && fabs(py - (GetPY() + GetCurrentSprite()->height / 64.0f)) < ((((float)engine->ScreenHeight()) / 64.0f) * 1.5f))
				{
					engine->PlaySample("gateOpening");
					bGateOpenningSound = false;
				}
			}
		}
	}

	if (bTriggered)
	{
		if (fabs(py - fOriginY) <= (mapStates[nCurrentFrame]->height / 64.0f))
		{
			vy = cfOpeningSpeed;
		}
		else
		{
			if (bGateOpennedSound)
			{
				engine->StopSample("gateOpening");
				engine->PlaySample("gateOpened");
				bGateOpennedSound = false;
			}
			vy = 0.0f;
		}
	}
}

void cDynamicWall::DrawSwitch(float cameraX, float cameraY)
{
	if (bTriggerable)
	{
		float fTargetX = cameraX - fTriggerX;
		float fTargetY = cameraY - fTriggerY;
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

		if (fDistance <= engine->ScreenWidth() / engine->GetTileWidth())
		{
			engine->SetPixelMode(olc::Pixel::ALPHA);
			olc::GFX2D::Transform2D t;
			t.Translate(-engine->GetTileWidth() / 2.0f, -engine->GetTileHeight() / 2.0f);
			t.Translate((fTriggerX - cameraX + 0.5f) * engine->GetTileWidth(), (fTriggerY - cameraY + 0.7f) * engine->GetTileHeight());
			olc::GFX2D::DrawSprite(engine->GetDoorSwitch(bTriggered), t);
			engine->SetPixelMode(olc::Pixel::NORMAL);
		}
	}
}
