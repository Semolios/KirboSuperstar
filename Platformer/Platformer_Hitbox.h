#pragma once
#include "olcPixelGameEngine.h"

class cHitbox
{
public:
	cHitbox();

public:
	struct vec2d
	{
		float x;
		float y;
	};

	std::vector<vec2d> p;	// Transformed Points
	vec2d pos;				// Position of shape
	float angle;			// Direction of shape
	std::vector<vec2d> o;	// "Model" of shape							
	bool overlap = false;	// Flag to indicate if overlap has occurred

public:
	static bool ShapeOverlap_DIAG(cHitbox* r1, cHitbox* r2);

	void Draw(olc::PixelGameEngine* gfx, olc::Pixel col);
};
