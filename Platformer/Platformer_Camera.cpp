#include "Platformer_Camera.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cCamera::engine = nullptr;

cCamera::cCamera()
{
}

void cCamera::ClampCameraOffset()
{
	if (fCameraLookingDown <= cfCameraLowerPos) fCameraLookingDown = cfCameraLowerPos;
	if (fCameraLookingDown >= cfCameraUpperPos) fCameraLookingDown = cfCameraUpperPos;
}

void cCamera::SetPositions(float fPlayerPosX, float fPlayerPosY)
{
	fCameraPosX = fPlayerPosX;
	fCameraPosY = fPlayerPosY;
}

void cCamera::DrawLevel(cLevel* level, float fElapsedTime)
{
	int nVisibleTilesX = engine->ScreenWidth() / engine->GetTileWidth();
	int nVisibleTilesY = engine->ScreenHeight() / engine->GetTileHeight();

	// Calculate Top-Left most visible tile
	fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
	fOffsetY = fCameraPosY - (float)nVisibleTilesY * fCameraLookingDown;

	// Clamp camera to game boundaries
	if (fOffsetX < 1) fOffsetX = 1;
	if (fOffsetY < 1) fOffsetY = 1;
	if (fOffsetX > level->GetWidth() - nVisibleTilesX - 1) fOffsetX = level->GetWidth() - nVisibleTilesX - 1;
	if (fOffsetY > level->GetHeight() - nVisibleTilesY - 1) fOffsetY = level->GetHeight() - nVisibleTilesY - 1;

	if (bShake)
	{
		fShakeTimerChange += fElapsedTime;

		if (fShakeTimerChange >= cfShakeEffectChangeFrequency)
		{
			fShakeEffectX = ((float)(rand() % nShakeAmplitudeX) - 100.0f) / cfShakeAttenuation;
			fShakeEffectY = ((float)(rand() % nShakeAmplitudeY) - 100.0f) / cfShakeAttenuation;
		}

		fOffsetX += fShakeEffectX;
		fOffsetY += fShakeEffectY;
	}

	DrawLevelBackground(level);

	level->DrawTiles(nVisibleTilesX, nVisibleTilesY, fOffsetX, fOffsetY);
}

float cCamera::GetOffsetX()
{
	return fOffsetX;
}

float cCamera::GetOffsetY()
{
	return fOffsetY;
}

void cCamera::LowerCameraPosition()
{
	fCameraLookingDown -= cfCameraMoveSpeed;
}

void cCamera::RaiseCameraPosition()
{
	fCameraLookingDown += cfCameraMoveSpeed;
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

void cCamera::DrawLevelBackground(cLevel* level)
{
	// Calculate parallax
	float fBackgroundOffsetX = fOffsetX * engine->GetTileWidth() * ((float)(engine->GetBackGround()->width - engine->ScreenWidth()) / (float)(level->GetWidth() * engine->GetTileWidth() - engine->ScreenWidth()));
	float fBackgroundOffsetY = fOffsetY * engine->GetTileHeight() * ((float)(engine->GetBackGround()->height - engine->ScreenHeight()) / (float)(level->GetWidth() * engine->GetTileHeight() - engine->ScreenHeight()));

	// Threading the process
	int nSectionWidth = engine->ScreenWidth() / nMaxThreads;

	std::thread t[nMaxThreads];
	for (size_t i = 0; i < nMaxThreads; i++)
	{
		t[i] = std::thread(&cCamera::DrawBackground, this, nSectionWidth * i, 0, fBackgroundOffsetX + (nSectionWidth * i), fBackgroundOffsetY, nSectionWidth, engine->ScreenHeight());
	}

	for (size_t i = 0; i < nMaxThreads; i++)
	{
		t[i].join();
	}
}

void cCamera::DrawBackground(int ox, int oy, float fBackgroundOffsetX, float fBackgroundOffsetY, float width, float height)
{
	engine->DrawPartialSprite(ox, oy, engine->GetBackGround(), fBackgroundOffsetX, fBackgroundOffsetY, width, height);
}