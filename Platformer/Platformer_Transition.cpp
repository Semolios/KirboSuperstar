#include "Platformer_Transition.h"

cAnimator* cTransition::animPlayer = nullptr;

cTransition::cTransition(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim)
{
	sprBackGround = background;
	animPlayer = anim;
}

bool cTransition::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	animPlayer->Update(fElapsedTime);

	olc::GFX2D::Transform2D t;
	t.Translate(gfx->ScreenWidth() * 0.7f, gfx->ScreenHeight() * 0.7f);

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	animPlayer->DrawSelf(gfx, t);
	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return false;
}
