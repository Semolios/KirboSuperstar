#ifndef DEF_ANIMATOR
#define DEF_ANIMATOR

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class cAnimator
{
public:
	std::map<std::string, std::vector<olc::Sprite*>> mapStates;
	std::string sCurrentState;
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.033f;
	float fTimeCounter = 0.0f;

	void ChangeState(std::string s);
	void Update(float fElapsedTime);
	void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t);
	void LoadAnimations();
};

#endif // !DEF_ANIMATOR
