#include "Platformer_Hitbox.h"
#include "Platformer_Engine.h"

cHitbox::cHitbox()
{
}

bool cHitbox::ShapeOverlap_DIAG(cHitbox* r1, cHitbox* r2)
{
	cHitbox* poly1 = r1;
	cHitbox* poly2 = r2;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			poly1 = r2;
			poly2 = r1;
		}

		// Check diagonals of polygon...
		for (int p = 0; p < poly1->p.size(); p++)
		{
			vec2d line_r1s = poly1->pos;
			vec2d line_r1e = poly1->p[p];

			// ...against edges of the other
			for (int q = 0; q < poly2->p.size(); q++)
			{
				vec2d line_r2s = poly2->p[q];
				vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];

				// Standard "off the shelf" line segment intersection
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void cHitbox::Draw(olc::PixelGameEngine* gfx, olc::Pixel col)
{
	gfx->DrawLine(p[0].x, p[0].y, p[1].x, p[1].y, col);
	gfx->DrawLine(p[1].x, p[1].y, p[2].x, p[2].y, col);
	gfx->DrawLine(p[2].x, p[2].y, p[3].x, p[3].y, col);
	gfx->DrawLine(p[3].x, p[3].y, p[0].x, p[0].y, col);
}
