#include "Platformer_TitleScreen.h"

cTitleScreen::cTitleScreen(olc::PixelGameEngine* gfx, olc::Sprite* background)
{
	sprBackGround = background;

	fMarker1 = 0.0f;
	fMarker2 = 0.0f;
	fTheta = 0.0f;

	path.points.push_back({ 0.5f * gfx->ScreenWidth(), 0.5f * gfx->ScreenHeight() });
	path.points.push_back({ 0.8f * gfx->ScreenWidth(), 0.2f * gfx->ScreenHeight() });
	path.points.push_back({ 0.7f * gfx->ScreenWidth(), 0.8f * gfx->ScreenHeight() });
	path.points.push_back({ 0.5f * gfx->ScreenWidth(), 0.5f * gfx->ScreenHeight() });
	path.points.push_back({ 0.2f * gfx->ScreenWidth(), 0.2f * gfx->ScreenHeight() });
	path.points.push_back({ 0.3f * gfx->ScreenWidth(), 0.8f * gfx->ScreenHeight() });

	sprstars = new olc::Sprite("assets/gfx/stars.png");
}

bool cTitleScreen::Update(olc::PixelGameEngine* gfx, float fElapsedTime)
{
	gfx->DrawSprite(0, 0, sprBackGround);

	path.fTotalSplineLength = 0.0f;

	// Draw Control Points
	for (int i = 0; i < path.points.size(); i++)
	{
		path.fTotalSplineLength += (path.points[i].length = path.CalculateSegmentLength(i, true));
	}

	fMarker1 += cfStarsMovSpeed * fElapsedTime;
	if (fMarker1 >= path.fTotalSplineLength) fMarker1 = 0.0f;

	fMarker2 -= cfStarsMovSpeed * fElapsedTime;
	if (fMarker2 < 0.0f) fMarker2 = path.fTotalSplineLength;

	// Draw agent to demonstrate gradient
	DrawNewStar(gfx, fMarker1);
	DrawNewStar(gfx, fMarker2);

	fTheta += cfStarsRotSpeed * fElapsedTime;

	return false;
}

void cTitleScreen::DrawNewStar(olc::PixelGameEngine* gfx, float fMarker)
{
	float fOffset1 = path.GetNormalisedOffset(fMarker);
	sPoint2D p1 = path.GetSplinePoint(fOffset1, true);

	olc::GFX2D::Transform2D t1;
	t1.Translate(-cnTileWidth / 2.0f, -cnTileHeight / 2.0f);
	t1.Rotate(fTheta);
	t1.Translate(p1.x, p1.y);

	gfx->SetPixelMode(olc::Pixel::ALPHA);
	DrawStar(gfx, t1);
	gfx->SetPixelMode(olc::Pixel::NORMAL);
}

void cTitleScreen::DrawStar(olc::PixelGameEngine* gfx, olc::GFX2D::Transform2D& t)
{
	olc::GFX2D::DrawSprite(sprstars, t);
}
