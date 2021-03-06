#include "Platformer_TitleScreen.h"

cTitleScreen::cTitleScreen(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;

	fTheta = 0.0f;

	sprstars = new olc::Sprite("assets/gfx/stars.png");
}

bool cTitleScreen::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	DrawNewStar(gfx, p1x, p1y, +fTheta);
	DrawNewStar(gfx, p2x, p2y, -fTheta);

	fTheta += cfStarsRotSpeed * fElapsedTime;

	return false;
}

void cTitleScreen::DrawNewStar(olc::PixelGameEngine* gfx, float px, float py, float theta)
{
	olc::GFX2D::Transform2D t1;
	t1.Translate(-cnTileWidth / 2.0f, -cnTileHeight / 2.0f);
	t1.Rotate(theta);
	t1.Translate(px, py);

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	olc::GFX2D::DrawSprite(sprstars, t1);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}
