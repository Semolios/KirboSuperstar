#include "Platformer_LevelStart.h"

cAnimator* cLevelStart::animPlayer = nullptr;

cLevelStart::cLevelStart(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim)
{
	sprBackGround = background;
	animPlayer = anim;
}

bool cLevelStart::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	animPlayer->Update(fElapsedTime);

	olc::GFX2D::Transform2D t;
	t.Translate(gfx->ScreenWidth() * 0.7f, gfx->ScreenHeight() * 0.7f);

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	animPlayer->DrawSelf(t);
	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return false;
}
