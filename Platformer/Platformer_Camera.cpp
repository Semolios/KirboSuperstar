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

	// Get offsets for smooth movement
	float fTileOffsetX = (fOffsetX - (int)fOffsetX) * engine->GetTileWidth();
	float fTileOffsetY = (fOffsetY - (int)fOffsetY) * engine->GetTileHeight();

	// Draw Visible tile map
	for (int x = -2; x < nVisibleTilesX + 2; x++)
	{
		for (int y = -2; y < nVisibleTilesY + 2; y++)
		{
			wchar_t sTileID = engine->GetTile(x + fOffsetX, y + fOffsetY);
			switch (sTileID)
			{
				case L'#': // Solid Block
					gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, engine->GetTilesSprites(), 0 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
					break;
				case L'G': // Ground Block
					DrawGroundTile(x, y, fTileOffsetX, fTileOffsetY, engine->GetGroundTiles(), sTileID, gfx);
					break;
				case L'B': // Brick Block
					gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, engine->GetTilesSprites(), 2 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
					break;
				case L'?': // Question Block
					gfx->SetPixelMode(olc::Pixel::ALPHA);
					gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, engine->GetTilesSprites(), 3 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
					gfx->SetPixelMode(olc::Pixel::NORMAL);
					break;
				case L'o': // Coin
					gfx->SetPixelMode(olc::Pixel::ALPHA);
					gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, engine->GetTilesSprites(), 4 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
					gfx->SetPixelMode(olc::Pixel::NORMAL);
					break;
				case L'w': // Door
					gfx->SetPixelMode(olc::Pixel::ALPHA);
					gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY + (engine->GetTileHeight() - engine->GetDoorSpr()->height) + 5.0f, engine->GetDoorSpr(), 0, 0, engine->GetDoorSpr()->width, engine->GetDoorSpr()->height);
					gfx->SetPixelMode(olc::Pixel::NORMAL);
					break;
					// TODO
					/*ADD HERE THE NEW TILES*/
			}
		}
	}
}

void cCamera::DrawGroundTile(int x, int y, float fTileOffsetX, float fTileOffsetY, olc::Sprite* spriteTiles, wchar_t tile, olc::PixelGameEngine* gfx)
{
	// Get all tiles around the current tile
	wchar_t tilesAround[3][3];
	tilesAround[0][0] = engine->GetTile(x - 1 + fOffsetX, y - 1 + fOffsetY); tilesAround[0][1] = engine->GetTile(x + 0 + fOffsetX, y - 1 + fOffsetY);	tilesAround[0][2] = engine->GetTile(x + 1 + fOffsetX, y - 1 + fOffsetY);
	tilesAround[1][0] = engine->GetTile(x - 1 + fOffsetX, y + 0 + fOffsetY); tilesAround[1][1] = tile;													tilesAround[1][2] = engine->GetTile(x + 1 + fOffsetX, y + 0 + fOffsetY);
	tilesAround[2][0] = engine->GetTile(x - 1 + fOffsetX, y + 1 + fOffsetY); tilesAround[2][1] = engine->GetTile(x + 0 + fOffsetX, y + 1 + fOffsetY);	tilesAround[2][2] = engine->GetTile(x + 1 + fOffsetX, y + 1 + fOffsetY);

	// Check the 47 configurations
	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 0 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 1 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 2 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 3 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 4 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 5 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 6 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 7 * engine->GetTileWidth(), 0 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 0 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 1 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 2 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 3 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 4 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 5 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 6 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 7 * engine->GetTileWidth(), 1 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 0 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 1 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 2 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 3 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 4 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 5 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 6 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 7 * engine->GetTileWidth(), 2 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 0 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 1 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 2 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 3 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 4 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 5 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 6 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 7 * engine->GetTileWidth(), 3 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 0 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 1 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 2 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 3 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 4 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 5 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 6 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 7 * engine->GetTileWidth(), 4 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 0 * engine->GetTileWidth(), 5 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 1 * engine->GetTileWidth(), 5 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 2 * engine->GetTileWidth(), 5 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 3 * engine->GetTileWidth(), 5 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 4 * engine->GetTileWidth(), 5 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 5 * engine->GetTileWidth(), 5 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}

	if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
		tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
		/*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawPartialSprite(x * engine->GetTileWidth() - fTileOffsetX, y * engine->GetTileHeight() - fTileOffsetY, spriteTiles, 6 * engine->GetTileWidth(), 5 * engine->GetTileHeight(), engine->GetTileWidth(), engine->GetTileHeight());
		gfx->SetPixelMode(olc::Pixel::NORMAL);
		return;
	}
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
