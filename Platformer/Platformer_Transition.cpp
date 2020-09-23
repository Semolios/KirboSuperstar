#include "Platformer_Transition.h"

cAnimator* cTransition::animPlayer = nullptr;

bool cTransition::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->Clear(olc::BLACK);

	animPlayer->Update(fElapsedTime);

	olc::GFX2D::Transform2D t;
	t.Translate(gfx->ScreenWidth() * 0.8f, gfx->ScreenHeight() * 0.8f);
	animPlayer->DrawSelf(gfx, t);

	return false;
}
