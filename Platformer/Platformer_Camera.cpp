#include "Platformer_Camera.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cCamera::engine = nullptr;

cCamera::cCamera()
{
}

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
	int nVisibleTilesX = engine->ScreenWidth() / engine->GetTileWidth();
	int nVisibleTilesY = engine->ScreenHeight() / engine->GetTileHeight();

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

	// Draw Level background
	DrawBackground(level);

	level->DrawTiles(nVisibleTilesX, nVisibleTilesY, fOffsetX, fOffsetY);
}

void cCamera::DrawBackground(cLevel* level)
{
	float fBackgroundOffsetX = fOffsetX * engine->GetTileWidth() * ((float)(engine->GetBackGround()->width - engine->ScreenWidth()) / (float)(level->GetWidth() * engine->GetTileWidth() - engine->ScreenWidth()));
	float fBackgroundOffsetY = fOffsetY * engine->GetTileHeight() * ((float)(engine->GetBackGround()->height - engine->ScreenHeight()) / (float)(level->GetHeight() * engine->GetTileHeight() - engine->ScreenHeight()));

	int nSectionWidth = engine->ScreenWidth() / nMaxThreads;

	nWorkerComplete = 0;

	for (size_t i = 0; i < nMaxThreads; i++)
	{
		workers[i].Start(nSectionWidth * i, 0, fBackgroundOffsetX + (nSectionWidth * i), fBackgroundOffsetY, nSectionWidth, engine->ScreenHeight(), this);
	}

	while (nWorkerComplete < nMaxThreads) // Wait for all workers to complete
	{
	}
}

void cCamera::DrawBackgroundThread(int x, int y, float fBckgrdoffX, float fBckgrdoffY, int w, int h)
{
	engine->DrawPartialSprite(x, y, engine->GetBackGround(), fBckgrdoffX, fBckgrdoffY, w, h);
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

void cCamera::InitialiseThreadPool()
{
	for (int i = 0; i < nMaxThreads; i++)
	{
		workers[i].alive = true;
		workers[i].screen_width = engine->ScreenWidth();
		workers[i].thread = std::thread(&WorkerThread::DrawBackground, &workers[i]);
	}
}
