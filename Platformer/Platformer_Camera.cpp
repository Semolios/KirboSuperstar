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

void cCamera::DrawLevel(olc::PixelGameEngine* gfx, cLevel* level, float fElapsedTime)
{
	int nVisibleTilesX = gfx->ScreenWidth() / engine->GetTileWidth();
	int nVisibleTilesY = gfx->ScreenHeight() / engine->GetTileHeight();

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

	// Draw Level background
	float fBackgroundOffsetX = fOffsetX * engine->GetTileWidth() * ((float)(engine->GetBackGround()->width - gfx->ScreenWidth()) / (float)(level->GetWidth() * engine->GetTileWidth() - gfx->ScreenWidth()));
	float fBackgroundOffsetY = fOffsetY * engine->GetTileHeight() * ((float)(engine->GetBackGround()->height - gfx->ScreenHeight()) / (float)(level->GetWidth() * engine->GetTileHeight() - gfx->ScreenHeight()));
	gfx->DrawPartialSprite(0, 0, engine->GetBackGround(), fBackgroundOffsetX, fBackgroundOffsetY, gfx->ScreenWidth(), gfx->ScreenHeight());

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
