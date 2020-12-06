#include "Platformer_WorldMap.h"

cAnimator* cWorldMap::animPlayer = nullptr;

cWorldMap::cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;

	path.points.push_back({ 0.22f * gfx->ScreenWidth(), 0.75f * gfx->ScreenHeight() });	// Beginning level
	path.points.push_back({ 0.52f * gfx->ScreenWidth(), 0.60f * gfx->ScreenHeight() });	// Lake level
	path.points.push_back({ 0.70f * gfx->ScreenWidth(), 0.30f * gfx->ScreenHeight() });	// Ice level
	path.points.push_back({ 0.33f * gfx->ScreenWidth(), 0.50f * gfx->ScreenHeight() });	// Forest level
	path.points.push_back({ 0.33f * gfx->ScreenWidth(), 0.20f * gfx->ScreenHeight() });	// Volcano level
	path.points.push_back({ 0.77f * gfx->ScreenWidth(), 0.82f * gfx->ScreenHeight() });	// Beach level
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
	for (float t = 0; t < (float)path.points.size(); t += 0.0005f)
	{
		sPoint2D pos = path.GetSplinePoint(t, true);
		gfx->Draw(pos.x, pos.y);
	}

	// Draw Control Points
	for (int i = 0; i < path.points.size(); i++)
	{
		gfx->FillRect(path.points[i].x - 3, path.points[i].y - 3, 6, 6, (i <= nUnlockedLevels - 1) ? olc::YELLOW : olc::RED);
	}

	olc::GFX2D::Transform2D t;
	t.Translate(path.points[nSelectedLevel].x - (nKirboTileWidth / 2.0f), path.points[nSelectedLevel].y - (nKirboTileHeight / 2.0f));

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
