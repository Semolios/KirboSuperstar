#include "Platformer_Camera.h"

cCamera::cCamera()
{}

void cCamera::ClampOffset()
{
	if (fLookingDownPos <= cfLowerPos) fLookingDownPos = cfLowerPos;
	if (fLookingDownPos >= cfUpperPos) fLookingDownPos = cfUpperPos;
}

void cCamera::SetPositions(float fPlayerPosX, float fPlayerPosY)
{
	fPosX = fPlayerPosX;
	fPosY = fPlayerPosY;
}

void cCamera::DrawLevel(cLevel* level, float fElapsedTime)
{
	level->DrawTiles(nVisibleTilesX, nVisibleTilesY, fOffsetX, fOffsetY);
}

void cCamera::CalculateFOV(cLevel* level, OneLoneCoder_Platformer* engine)
{
	nVisibleTilesX = engine->ScreenWidth() / engine->GetTileWidth();
	nVisibleTilesY = engine->ScreenHeight() / engine->GetTileHeight();

	// Calculate Top-Left most visible tile
	fOffsetX = fPosX - (float)nVisibleTilesX / 2.0f;
	fOffsetY = fPosY - (float)nVisibleTilesY * fLookingDownPos;

	// Clamp camera to game boundaries
	if (fOffsetX < 1) fOffsetX = 1;
	if (fOffsetY < 1) fOffsetY = 1;
	if (fOffsetX > level->GetWidth() - nVisibleTilesX - 1) fOffsetX = level->GetWidth() - nVisibleTilesX - 1;
	if (fOffsetY > level->GetHeight() - nVisibleTilesY - 1) fOffsetY = level->GetHeight() - nVisibleTilesY - 1;

	if (bShake)
	{
		fShakeEffectX = ((float)(rand() % nShakeAmplitudeX) - 100.0f) / cfShakeAttenuation;
		fShakeEffectY = ((float)(rand() % nShakeAmplitudeY) - 100.0f) / cfShakeAttenuation;

		fOffsetX += fShakeEffectX;
		fOffsetY += fShakeEffectY;
	}
}

void cCamera::DrawBackground(cLevel* level, OneLoneCoder_Platformer* engine)
{
	float fBackgroundOffsetX = fOffsetX * engine->GetTileWidth() * ((float)(engine->GetBackGround()->width - engine->ScreenWidth()) / (float)(level->GetWidth() * engine->GetTileWidth() - engine->ScreenWidth()));
	float fBackgroundOffsetY = fOffsetY * engine->GetTileHeight() * ((float)(engine->GetBackGround()->height - engine->ScreenHeight()) / (float)(level->GetHeight() * engine->GetTileHeight() - engine->ScreenHeight()));

	engine->DrawPartialSprite(0, 0, engine->GetBackGround(), fBackgroundOffsetX, fBackgroundOffsetY, engine->ScreenWidth(), engine->ScreenHeight());
}

float cCamera::GetOffsetX()
{
	return fOffsetX;
}

float cCamera::GetOffsetY()
{
	return fOffsetY;
}

void cCamera::LowerPosition()
{
	fLookingDownPos -= cfMoveSpeed;
}

void cCamera::RaisePosition()
{
	fLookingDownPos += cfMoveSpeed;
}

void cCamera::SetShake(bool shake)
{
	bShake = shake;
}

void cCamera::ActivateShakeEffect(bool activate, int shakeAmplitudeX, int shakeAmplitudeY)
{
	bShake = activate;
	nShakeAmplitudeX = shakeAmplitudeX;
	nShakeAmplitudeY = shakeAmplitudeY;
}

void cCamera::SpawnSceneries(cLevel* level, float fElapsedTime, OneLoneCoder_Platformer* engine)
{
#pragma region Iceberg

	if (level->GetCurrentLvl() == 2 && !engine->IsInBossLevel())
	{
		float fSnowX = GetOffsetX() + ((float)(rand() % cnObjectPosXRange) / 10.0f) - 4.0f;
		float fSnowY = GetOffsetY();

		engine->AddProjectile(fSnowX, fSnowY, true, cfSnowVelX, cfSnowVelY, 1.0f, "snow", false, 0, false, false, 0, false, 0.0f, "", false, "", true);

		fSnowX = GetOffsetX() + ((float)(rand() % cnObjectPosXRange) / 10.0f) - 4.0f;
		fSnowY = GetOffsetY() + (engine->ScreenHeight() / 64.0f);

		engine->AddProjectile(fSnowX, fSnowY, true, cfSnowVelX, cfSnowVelY, 1.0f, "snow", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
	}

#pragma endregion

#pragma region Halberd

	if (level->GetCurrentLvl() == 4 && !engine->IsInBossLevel())
	{
		fHalberdCloudSpawnTimer += fElapsedTime;
		if (fHalberdCloudSpawnTimer >= cfHalberdCloudSpawnTime)
		{
			fHalberdCloudSpawnTimer = 0.0f;
			float fCloudX = GetOffsetX() + (engine->ScreenWidth() / 64.0f);
			float fCloudY = GetOffsetY() + ((float)(rand() % cnObjectPosYRange) / 10.0f);

			engine->AddProjectile(fCloudX, fCloudY, true, cfHalberdCloudVelX, 0.0f, 1.0f, "halberdCloud", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
		}

		// Each frame, a random number is generated. If this number % 2 / fElapsedTime is equal to 0 then the event occurs
		int useRangedAttack = rand() % (int)(2 / fElapsedTime);
		if (useRangedAttack == 0)
		{
			float fWahooX = GetOffsetX() + (engine->ScreenWidth() / 64.0f);
			float fWahooY = GetOffsetY() + ((float)(rand() % cnObjectPosYRange) / 10.0f);

			engine->AddProjectile(fWahooX, fWahooY, true, cfWahooVelX, 0.0f, 1.0f, "speedrunnerWahoo", false, 0, false, false, 0, false, 0.0f, "", false, "", true);
		}
	}

#pragma endregion
}
