#ifndef DEF_ANIMATOR
#define DEF_ANIMATOR

#pragma once
#include "olcPixelGameEngine.h"

class cAnimator
{
public:
	std::map<std::string, std::vector<std::string>> mapStates;
	std::string sCurrentState;
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.033f;
	float fTimeCounter = 0.0f;

	void ChangeState(std::string s, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void Update(float fElapsedTime, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void DrawSelf(float posx, float posy, float angle, float faceDir, olc::PixelGameEngine* gfx, olc::Decal* playerDecal);
	void LoadAnimations();
};

#endif // !DEF_ANIMATOR
