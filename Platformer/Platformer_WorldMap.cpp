#include "Platformer_WorldMap.h"

cAnimator* cWorldMap::animPlayer = nullptr;

cWorldMap::cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* lock, cAnimator* anim)
{
	sprBackGround = background;
	sprLock = lock;

	path.push_back({ 0.30f * gfx->ScreenWidth(), 0.65f * gfx->ScreenHeight() }); // Forest level
	path.push_back({ 0.54f * gfx->ScreenWidth(), 0.55f * gfx->ScreenHeight() }); // Lake level
	path.push_back({ 0.72f * gfx->ScreenWidth(), 0.30f * gfx->ScreenHeight() }); // Ice level
	path.push_back({ 0.33f * gfx->ScreenWidth(), 0.40f * gfx->ScreenHeight() }); // Sky level
	path.push_back({ 0.37f * gfx->ScreenWidth(), 0.21f * gfx->ScreenHeight() }); // Halberd level
	path.push_back({ 0.82f * gfx->ScreenWidth(), 0.80f * gfx->ScreenHeight() }); // Fountain of dreams level

	animPlayer = anim;
}

bool cWorldMap::Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	animPlayer->Update(fElapsedTime, playerSprite, playerDecal);

	// Handle input
	if (gfx->GetKey(olc::RIGHT).bReleased || controller->GetButton(RIGHT).bPressed)
	{
		nSelectedLevel++;
		if (nSelectedLevel >= nUnlockedLevels)
			nSelectedLevel = 0;
	}

	if (gfx->GetKey(olc::LEFT).bReleased || controller->GetButton(LEFT).bPressed)
	{
		nSelectedLevel--;
		if (nSelectedLevel < 0)
			nSelectedLevel = nUnlockedLevels - 1;
	}

	// Draw locks on locked levels
	for (int i = nUnlockedLevels; i < path.size(); i++)
	{
		gfx->SetPixelMode(olc::Pixel::ALPHA);
		gfx->DrawSprite(path[i].x - (sprLock->width / 2), path[i].y - (sprLock->height / 2), sprLock);
		gfx->SetPixelMode(olc::Pixel::NORMAL);
	}

	animPlayer->DrawSelf(path[nSelectedLevel].x, path[nSelectedLevel].y, 0.0f, 1.0f, gfx, playerDecal);

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
