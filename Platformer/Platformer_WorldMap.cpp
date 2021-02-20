#include "Platformer_WorldMap.h"

cAnimator* cWorldMap::animPlayer = nullptr;

cWorldMap::cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim)
{
	sprBackGround = background;

	path.AddPoint(0.22f * gfx->ScreenWidth(), 0.75f * gfx->ScreenHeight());	// Beginning level
	path.AddPoint(0.52f * gfx->ScreenWidth(), 0.60f * gfx->ScreenHeight());	// Lake level
	path.AddPoint(0.70f * gfx->ScreenWidth(), 0.30f * gfx->ScreenHeight());	// Ice level
	path.AddPoint(0.33f * gfx->ScreenWidth(), 0.50f * gfx->ScreenHeight());	// Forest level
	path.AddPoint(0.33f * gfx->ScreenWidth(), 0.20f * gfx->ScreenHeight());	// Volcano level
	path.AddPoint(0.77f * gfx->ScreenWidth(), 0.82f * gfx->ScreenHeight());	// Beach level

	animPlayer = anim;
}

bool cWorldMap::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	animPlayer->Update(fElapsedTime);

	// Handle input
	if (gfx->GetKey(olc::RIGHT).bReleased)
	{
		nSelectedLevel++;
		if (nSelectedLevel >= nUnlockedLevels)
			nSelectedLevel = 0;
	}

	if (gfx->GetKey(olc::LEFT).bReleased)
	{
		nSelectedLevel--;
		if (nSelectedLevel < 0)
			nSelectedLevel = nUnlockedLevels - 1;
	}

	// Draw Spline
	for (float t = 0; t < (float)path.GetPointsSize(); t += 0.0005f)
	{
		sPoint2D pos = path.GetSplinePoint(t, true);
		gfx->Draw(pos.x, pos.y);
	}

	// Draw Control Points
	for (int i = 0; i < path.GetPointsSize(); i++)
	{
		gfx->FillRect(path.GetIX(i) - 3, path.GetIY(i) - 3, 6, 6, (i <= nUnlockedLevels - 1) ? olc::YELLOW : olc::RED);
	}

	olc::GFX2D::Transform2D t;
	t.Translate(path.GetIX(nSelectedLevel) - (cnKirboTileWidth / 2.0f), path.GetIY(nSelectedLevel) - (cnKirboTileHeight / 2.0f));

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	animPlayer->DrawSelf(gfx, t);
	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return false;
}

int cWorldMap::GetSelectedLevel()
{
	return nSelectedLevel;
}

void cWorldMap::SetUnlockedLevel(int unlockedLevels)
{
	nUnlockedLevels = unlockedLevels;
}
