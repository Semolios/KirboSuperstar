#include "Platformer_WorldMap.h"

cAnimator* cWorldMap::animPlayer = nullptr;

cWorldMap::cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;
}

bool cWorldMap::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	animPlayer->Update(fElapsedTime);

	olc::GFX2D::Transform2D t;
	t.Translate(gfx->ScreenWidth() * 0.8f, gfx->ScreenHeight() * 0.8f);

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	animPlayer->DrawSelf(gfx, t);
	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return false;
}
