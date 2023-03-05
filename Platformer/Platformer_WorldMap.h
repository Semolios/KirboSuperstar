#ifndef DEF_WORLD_MAP
#define DEF_WORLD_MAP

#pragma once

#include "Platformer_Animator.h"
#include "Zix_PGE_Controller.h"

class cWorldMap
{
public:
	cWorldMap(olc::PixelGameEngine* gfx, olc::Sprite* background, olc::Sprite* lock, cAnimator* anim);

	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller);
	int GetSelectedLevel();
	void SetUnlockedLevel(int unlockedLevels);

private:
	const int cnKirboTileWidth = 192;
	const int cnKirboTileHeight = 192;

	int nSelectedLevel = 0;
	int nUnlockedLevels = 0;

	static cAnimator* animPlayer;
	std::vector<olc::vf2d> path;
	olc::Sprite* sprBackGround;
	olc::Sprite* sprLock;
};

#endif // !DEF_WORLD_MAP
