#include "Platformer_Animator.h"

void cAnimator::ChangeState(std::string s)
{
	if (s != sCurrentState)
	{
		sCurrentState = s;
		fTimeCounter = 0;
		nCurrentFrame = 0;
	}
}

void cAnimator::Update(float fElapsedTime)
{
	fTimeCounter += fElapsedTime;
	if (fTimeCounter >= fTimeBetweenFrames)
	{
		fTimeCounter -= fTimeBetweenFrames;
		nCurrentFrame++;
		if (nCurrentFrame >= mapStates[sCurrentState].size())
			nCurrentFrame = 0;
	}
}

void cAnimator::DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t)
{
	olc::GFX2D::DrawSprite(mapStates[sCurrentState][nCurrentFrame], t);
}
