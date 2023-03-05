#include "Platformer_LevelStart.h"

cAnimator* cLevelStart::animPlayer = nullptr;

cLevelStart::cLevelStart(olc::PixelGameEngine* gfx, olc::Sprite* background, cAnimator* anim)
{
	sprBackGround = background;
	animPlayer = anim;
}

bool cLevelStart::Update(olc::PixelGameEngine* gfx, float fElapsedTime, olc::Sprite* playerSprite, olc::Decal* playerDecal)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	animPlayer->Update(fElapsedTime, playerSprite, playerDecal);
    animPlayer->DrawSelf(gfx->ScreenWidth() * 0.7f + playerSprite->width / 2.0f, gfx->ScreenHeight() * 0.7f + playerSprite->height / 2.0f, 0.0f, 1.0f, gfx, playerDecal);

	return false;
}
